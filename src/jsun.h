#include <vector>
#include <map>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "value.h"
#include "value_cpp.h"
#include <stdio.h>

namespace jsun
	{
	//-----------------------------------------------------------------------------------------//
	uint8_t* SkipWhiteSpace( uint8_t *head, uint8_t *foot )
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
	type::value_ptr_t Parse( const char *buf, size_t size, size_t *consumed )
		{
		if( buf == NULL || size == 0 )
			throw ParseException();

		const char *head = (uint8_t*)buf;
		const char *foot = head + size;
		head = SkipWhiteSpace( head, foot );
		if( head >= foot )
			throw ParseException();
		
		switch( *head )
			{
			case 'n':
				{
				if( foot - head < 4 || *(uint32_t*)head != *(int32_t*)"null" )
					throw ParseException();
				
				if( consumed != NULL )
					*consumed = 4;
				
				return type::null_value_ptr_t( new type::NullValue() );
				}

			case 't':
				{
				if( foot - head < 4 || *(uint32_t*)head != *(int32_t*)"true" )
					throw ParseException();
				
				if( consumed != NULL )
					*consumed = 4;
				
				return type::true_value_ptr_t( new type::TrueValue() );
				}
				
			case 'f':
				{
				if( foot - head < 5 || *(uint32_t*)(head+1) != *(int32_t*)"alse" )
					throw ParseException();
				
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
					throw ParseException();
				
				if( consumed != NULL )
					*consumed = endptr - (char*)head;
				
				return type::number_value_ptr_t( new type::NumberValue( val ) );
				}
				
			case '"':
				{
				const char *start = (const char*)head;
				while( ++head < foot )
					{
					if( *head == '"' && *(head-1) != '\\' )
						{
						size_t strSize = (char*)head - strStart;
						if( consumed != NULL ) *consumed = strSize + 2;
						return type::string_value_ptr_t( new type::StringValue( strStart, strSize ) );
						}

					++head;
					}

				throw ParseException();
				}

			case '[':
				{
				const char *arrayStart = (const char*)head;
				head = SkipWhiteSpace( head, foot );
				if( head >= foot )
					{
					throw ParseException();
					}
				
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
				
				throw ParseException();
				}

			case '{':
				{
				const char *start = (const char*)head;
				head = SkipWhiteSpace( head, foot );
				if( head >= foot )
					{
					throw ParseException();
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

				throw ParseException();
				}
			}
		
		throw ParseException();
		}
	}
