#include <vector>
#include <map>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "value.h"
#include "value_cpp.h"
#include <stdio.h>

namespace trijson
	{
	//-----------------------------------------------------------------------------------------//
	struct InputRange
		{
			inline InputRange( const char *h, const char *f )
				:begin( h ), end( f ), cur( h ), lineCount( 0 ){};
			inline bool IsValid(){ return cur < end; };
			inline bool SkipWhiteSpace()
				{
				while( cur <end &&
					   ( *cur == '\x20' ||
						 *cur == '\t'   ||
						 *cur == '\r'   ||
						 *cur == '\xb'  ||
						 *cur == '\xc'  ||
						 ( *cur == '\n' && ++lineCount ) ) )
					++cur;

				return IsValid();
				}
			inline size_t GetCurrentSize(){ return end - cur; };
			inline size_t GetConsumedSize(){ return cur - begin; };
			inline void Forward( size_t s ){ cur +=s; };

			const char *begin;
			const char *end;
			const char *cur;
			size_t lineCount;
		};
	
	//-----------------------------------------------------------------------------------------//
	inline bool ParseEscapeString( char in, char *out )
	{
	switch( in )
		{
		case 'b':
			*out = '\b';
			break;
		case 'f':
			*out = '\f';
			break;
		case 'n':
			*out = '\n';
			break;
		case 'r':
			*out = '\r';
			break;

		case 't':
			*out = '\t';
			break;
			
		case '"':
		case '/':
		case '\\':
			*out = in;
			break;

		default:
			return false;
		}

	return true;
	}

	//-----------------------------------------------------------------------------------------//
	type::value_ptr_t ParseImpl( InputRange &input )
		{
		if( input.SkipWhiteSpace() == false )
			throw ParseException( "Malformed JSON", 0 );
		
		switch( *input.cur )
			{
			case 'n':
				{
				if( input.GetCurrentSize() < 4 || *(uint32_t*)input.cur != *(int32_t*)"null" )
					throw ParseException( "null?", input.lineCount );

				input.Forward( 4 );
				return type::null_value_ptr_t( new type::NullValue() );
				}

			case 't':
				{
				if( input.GetCurrentSize() < 4 || *(uint32_t*)input.cur != *(int32_t*)"true" )
					throw ParseException( "true?", input.lineCount );

				input.Forward( 4 );
				return type::true_value_ptr_t( new type::TrueValue() );
				}
				
			case 'f':
				{
				if( input.GetCurrentSize() < 5 || *(uint32_t*)(input.cur+1) != *(int32_t*)"alse" )
					throw ParseException( "false?", input.lineCount );

				input.Forward( 5 );
				return type::false_value_ptr_t( new type::FalseValue() );
				}
				
			case '-':case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
				{
				char *endptr = NULL;
				double val = strtod( input.cur, &endptr );
				if( ( val == 0 && endptr == (char*)input.cur ) ||   // convertion didn't operated.
					( ( ( val == HUGE_VALF || val == HUGE_VALL ) || // overflow/underflow happend.
						( val == 0 ) ) && errno == ERANGE ) ||      // converted over buf range.
					( endptr > (char*)input.end ) )
					throw ParseException( "Invalid number", input.lineCount );

				input.Forward( endptr - input.cur );
				return type::number_value_ptr_t( new type::NumberValue( val ) );
				}
				
			case '"':
				{
				const char *parseStart = input.cur;
				const char *copyStart = (char*)++input.cur;
				type::string_t str;
				
				while( input.IsValid() )
					{
					if( *(input.cur-1) != '\\' && *head == '"' )
						{
						size_t strSize = input.cur - copyStart;
						if( strSize > 0 )
							str.append( copyStart, strSize );
						
							*consumed = ++head - parseStart;
						return type::string_value_ptr_t( new type::StringValue( str ) );
						}
					else if( *head == '\\' )
						{
						if( ++head > foot )
							break;

						size_t strSize = head - copyStart;
						if( strSize > 0 )
							str.append( copyStart, strSize );

						char c;
						if( ParseEscapeString( *head, &c ) == false )
							throw ParseException( head, foot - head );
						str.push_back( c );

						copyStart = head + 1;
						}

					++head;
					}

				throw ParseException( "Insufficient JSON" );
				}

				/*
			case '[':
				{
				const char *parseStart = head;
				type::array_value_ptr_t arrayVal( new type::ArrayValue() );
				
				while( head < foot )
					{
					++head;
					head = SkipWhiteSpace( head, foot, &lineCount );
					if( head == NULL )
						throw ParseException( "Insufficient JSON" );

					size_t elemConsumed = 0;
					type::value_ptr_t val = Parse( head, foot - head, &elemConsumed );
					arrayVal->Append( val );
					head +=elemConsumed;
					head = SkipWhiteSpace( head, foot, &lineCount );
					
					if( head == NULL )
						throw ParseException( "Insufficient JSON" );
					
					if( *head == ']' )
						{
						if( consumed != NULL )
							*consumed = (++head - parseStart);
						return arrayVal;
						}
					else if( *head == ',' )
						continue;
					else
						throw ParseException( "Invalid JSON" );
					}

				throw ParseException( "Invalid JSON" );
				}

			case '{':
				{
				const char *parseStart = head;
				type::object_t object;

				while( head < foot )
					{
					++head;
					head = SkipWhiteSpace( head, foot, &lineCount );
					if( head == NULL )
						throw ParseException( "Invalid JSON" );
					
					size_t keyConsumed = 0;
					type::value_ptr_t key = Parse( head, foot - head, &keyConsumed );
					type::string_t strKey;
					if( key->Get( strKey ) == false )
						throw ParseException( "Invalid JSON" );
					head += keyConsumed;
					head = SkipWhiteSpace( head, foot, &lineCount );
					
					if( head == NULL || *head != ':' )
						throw ParseException( "Invalid JSON" );
					
					size_t valConsumed = 0;
					type::value_ptr_t val = Parse( head, foot - head, &valConsumed );
					head += valConsumed;
					head = SkipWhiteSpace( head, foot, &lineCount );

					if( head == NULL )
						throw ParseException( "Invalid JSON" );

					object.insert( std::make_pair( strKey, val ) );

					if( *head == '}' )
						{
						if( consumed != NULL )
							*consumed = (++head - parseStart);
						return type::object_value_ptr_t( new type::ObjectValue( object ) );
						}
					else if( *head == ',' )
						continue;
					else
						throw ParseException( "Invalid JSON" );
					}
					
				throw ParseException( "Invalid JSON" );
				}
			}
				*/
		throw ParseException( "Invalid JSON" );
		}
	}

	//-----------------------------------------------------------------------------------------//
	type::value_ptr_t Parse( const char *buf, size_t size, size_t *consumed )
		{
		InputRange input( buf, buf + size );
		type::value_ptr_t ret = ParseImpl( input );
		if( consumed != NULL )
			*consumed = input.GetConsumedSize();

		printf( "%02x %d\n", *input.cur, input.GetConsumedSize() );
		
		return ret;
		}
	}
