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
	inline bool ParseEscapeString( char in, char &out )
	{
	switch( in )
		{
		case 'b':
			out = '\b';
			break;
		case 'f':
			out = '\f';
			break;
		case 'n':
			out = '\n';
			break;
		case 'r':
			out = '\r';
			break;

		case 't':
			out = '\t';
			break;
			
		case '"':
		case '/':
		case '\\':
			out = in;
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
				++input.cur;
				const char *copyStart = input.cur;
				type::string_t str;
				
				while( input.IsValid() )
					{
					if( *input.cur == '\\' )
						{
						++input.cur;
						if( input.IsValid() == false )
							throw ParseException( "Malformed string", input.lineCount );
						
						char c = '\0';
						if( ParseEscapeString( *input.cur, c ) == false )
							throw ParseException( "Malformed escape string", input.lineCount );
						
						if( input.cur - copyStart > 0 )
							str.append( copyStart, input.cur - copyStart );
						str.push_back( c );

						copyStart = input.cur + 1;
						}
					else if( *input.cur == '"' )
						{
						if( input.cur - copyStart > 0 )
							str.append( copyStart, input.cur - copyStart );
						++input.cur; 
						
						return type::string_value_ptr_t( new type::StringValue( str ) );
						}

					++input.cur;
					}

				throw ParseException( "Invalid string", input.lineCount );
				}
				
			case '[':
				{
				++input.cur;
				type::array_t array;
				
				while( input.SkipWhiteSpace() )
					{
					array.push_back( ParseImpl( input ) );
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Malformed array", input.lineCount );
					if( *input.cur == ',' && ++input.cur )
						continue;
					else if( *input.cur == ']' && ++input.cur )
						break;
					else
						throw ParseException( "Malformed array", input.lineCount );
					}
				
				return type::array_value_ptr_t( new type::ArrayValue( array ) );
				}

			case '{':
				{
				++input.cur;
				type::object_t object;
				
				while( input.SkipWhiteSpace() )
					{
					
					type::value_ptr_t k = ParseImpl( input );
					if( k->GetType() != type::string_type )
						throw ParseException( "Malformed object1" );
					if( input.SkipWhiteSpace() == false || *input.cur != ':' )
						throw ParseException( "Malformed object2" );
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Malformed object3" );
					type::value_ptr_t v = ParseImpl( input );
					
					type::string_t kstr;
					k->Get( kstr );
					object[kstr] = v;

					
					
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Malformed object4" );
					if( *input.cur == ',' )
						continue;
					if( *input.cur == '}' )
						break;
					}

				return type::object_value_ptr_t( new type::ObjectValue( object ) );
				}
			}
		throw ParseException( "Invalid JSON" );
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
