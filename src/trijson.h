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
	inline const char* SkipWhiteSpace( const char *head, const char *foot )
		{
		while( head < foot &&
			   ( *head == '\x20' ||
				 *head == '\t'   ||
				 *head == '\r'   ||
				 *head == '\n'   ||
				 *head == '\xb'  ||
				 *head == '\xc' ) )
			++head;
		
		return head;
		}

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
	type::value_ptr_t Parse( const char *buf, size_t size, size_t *consumed )
		{
		if( buf == NULL || size == 0 )
			throw ParseException( "No buffer to parse" );

		const char *head = (const char*)buf;
		const char *foot = head + size;
		
		head = SkipWhiteSpace( head, foot );
		if( head >= foot )
			throw ParseException( "No data to parse" );
		
		switch( *head )
			{
			case 'n':
				{
				if( foot - head < 4 || *(uint32_t*)head != *(int32_t*)"null" )
					throw ParseException( head, foot - head );
				
				if( consumed != NULL )
					*consumed = 4;
				return type::null_value_ptr_t( new type::NullValue() );
				}

			case 't':
				{
				if( foot - head < 4 || *(uint32_t*)head != *(int32_t*)"true" )
					throw ParseException( head, foot - head );

				if( consumed != NULL )
					*consumed = 4;				
				return type::true_value_ptr_t( new type::TrueValue() );
				}
				
			case 'f':
				{
				if( foot - head < 5 || *(uint32_t*)(head+1) != *(int32_t*)"alse" )
					throw ParseException( head, foot - head );

				if( consumed != NULL )
					*consumed = 5;
				return type::false_value_ptr_t( new type::FalseValue() );
				}

			case '-':case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
				{
				char *endptr = NULL;
				double val = strtod( head, &endptr );
				if( ( val == 0 && endptr == (char*)head ) ||        // convertion didn't operated.
					( ( ( val == HUGE_VALF || val == HUGE_VALL ) || // overflow/underflow happend.
						( val == 0 ) ) && errno == ERANGE ) ||      // converted over buf range.
					( endptr > (char*)foot ) )
					throw ParseException( head, foot - head );

				if( consumed != NULL )
					*consumed = endptr - (char*)head;
				return type::number_value_ptr_t( new type::NumberValue( val ) );
				}
				
			case '"':
				{
				const char *parseStart = head;
				const char *copyStart = (char*)++head;
				type::string_t str;
				
				while( head < foot )
					{
					if( *(head-1) != '\\' && *head == '"' )
						{
						size_t strSize = head - copyStart;
						for( size_t i = 0; i < strSize; ++i )
							{
							printf( "%c\n", *(copyStart+i) );
							}
						
						if( strSize > 0 )
							str.append( head, strSize );
						
						if( consumed != NULL )
							*consumed = ++head - parseStart;
						return type::string_value_ptr_t( new type::StringValue( str ) );
						}
					else if( *head == '\\' )
						{
						if( ++head > foot )
							break;

						char c;
						if( ParseEscapeString( *head, &c ) == false )
							throw ParseException( head, foot - head );

						size_t strSize = head - copyStart;
						for( size_t i = 0; i < strSize; ++i )
							{
							printf( "%c\n", *(copyStart+i) );
							}
						if( strSize > 0 )
							str.append( head, strSize );

						copyStart = head + 1;
						}

					++head;
					}

				throw ParseException( "Insufficient buffer" );
				}

				/*
			case '[':
				{
				const char *start = (const char*)head;
				head = SkipWhiteSpace( head, foot );
				if( head >= foot )
					throw ParseException( "Invalid JSON" );
				
				type::array_value_ptr_t array( new type::ArrayValue() );
				while( ++head < foot )
					{
					size_t innerConsumed = 0;
					type::value_ptr_t v = Parse( (const char*)head, foot - head, &innerConsumed );
					array->Append( v );
					head += innerConsumed;
					assert( head <= foot );

					head = SkipWhiteSpace( head, foot );
					
					if( head >= foot )
						{
						break;
						}
					else if( *head == ']' )
						{
						if( consumed != NULL ) *consumed = (const char*)++head - arrayStart;
						return array;
						}
					else if( *head == ',' )
						{
						continue;
						}
					else
						{
						break;
						}
					}
				
				throw ParseException( "Invalid JSON" );
				}

			case '{':
				{
				const char *start = (const char*)head;
				head = SkipWhiteSpace( head, foot );
				if( head >= foot )
					{
					throw ParseException( "Invalid JSON" );
					}

				type::object_value_ptr_t object( new type::ObjectValue() );
				
				while( ++head < foot )
					{
					size_t innerConsumed = 0;
					type::value_ptr_t k = Parse( (const char*)head, foot - head, &innerConsumed );
					head += innerConsumed;
					assert( head <= foot );

					head = SkipWhiteSpace( head, foot );
					
					if( head >= foot )
						{
						break;
						}
					if( *head != ':' )
						{
						break;
						}

					head = SkipWhiteSpace( head, foot );
					
					if( head >= foot )
						{
						break;
						}
					
					innerConsumed = 0;
					type::value_ptr_t v = Parse( (const char*)head, foot - head, &innerConsumed );
					head += innerConsumed;
					assert( head <= foot );

					object->Insert( k, v );

					head = SkipWhiteSpace( head, foot ); 
					
					if( head >= foot )
						{
						break;
						}
					
					if( *head == '}' )
						{
						if( consumed != NULL ) *consumed = (const char*)++head - start;
						return object;
						}

					if( *head != ',' )
						{
						break;
						}
					}

				throw ParseException( "Invalid JSON" );
				}
				*/
			}
		
		throw ParseException( "Invalid JSON" );
		}
	}
