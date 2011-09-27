#include <vector>
#include <map>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <stdio.h>
#include "inputrange.h"
#include "value.h"
#include "value_cpp.h"

namespace trijson
	{
	//-----------------------------------------------------------------------------------------//
	type::value_ptr_t ParseImpl( InputRange &input )
		{
		if( input.SkipWhiteSpace() == false )
			throw ParseException( "Malformed JSON", 0 );
		
		switch( *input.cur )
			{
			case 'n':
				{
				if( input.GetRemainingSize() < 4 || *(uint32_t*)input.cur != *(uint32_t*)"null" )
					throw ParseException( "null?", input.lineCount );

				input.Forward( 4 );
				return type::null_value_ptr_t( new type::NullValue() );
				}

			case 't':
				{
				if( input.GetRemainingSize() < 4 || *(uint32_t*)input.cur != *(uint32_t*)"true" )
					throw ParseException( "true?", input.lineCount );

				input.Forward( 4 );
				return type::true_value_ptr_t( new type::TrueValue() );
				}
				
			case 'f':
				{
				if( input.GetRemainingSize() < 5 || *(uint32_t*)(input.cur+1) != *(uint32_t*)"alse" )
					throw ParseException( "false?", input.lineCount );

				input.Forward( 5 );
				return type::false_value_ptr_t( new type::FalseValue() );
				}
				
			case '-':case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
				{
				char *endptr = NULL;
				double val = strtod( input.cur, &endptr );
				if( ( val == 0 && endptr == (char*)input.cur )      // convertion didn't operate.
					|| ( ( ( val == HUGE_VALF || val == HUGE_VALL ) // overflow/underflow happend.
				    || ( val == 0 ) ) && errno == ERANGE )
					|| ( endptr > (char*)input.end ) )              // converted over buf range.
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

						if( *input.cur != 'u' )
							{
							char c = '\0';
							if( ParseEscapeString( *input.cur, c ) == false )
								throw ParseException( "Malformed escape string", input.lineCount );
							
							if( input.cur - copyStart > 0 )
							str.append( copyStart, input.cur - copyStart );
							str.push_back( c );

							copyStart = input.cur + 1;
							}
						else
							{
							input.Foward( 1 );
							if( input.GetRemainingSize() < 4 )
								throw ParseException( "Insufficient 4-hex-digits" );

							uint32_t codepoint = 0;
							if( StrToUint32( input.cur, 4, &codepoint ) != 4
								|| 0xdc00 <= codepoint && codepoint <= 0xdfff )
								throw ParseException( "Invalid 4-hex-digits" );
							input.Foward( 4 );
							
							if( 0x8d00 <= codepoint && codepoint <= 0xdbff )
								{
								uint32_t second = 0;
								if( StrToUint32( input.cur, 4, &second ) != 4 ||
									!( 0xdc00 <= second && second <= 0xdfff ) )
									throw ParseException( "Invalid 4-hex-digits" );
								input.Forward( 4 );
								
								codepoint = 0x10000
									| ( ( codepoint - 0xd8000 ) << 0xa )
									| ( second - 0xdc00 );
								}
							else if( 0xdc00 <= second && second <= 0xdfff )
								throw ParseException( "Invalid 4-hex-digits" );

							uint8_t buf[4];
							size_t l = EncodeFromUTF16ToUTF8( codepoint, buf );
							if( l == 0 )
								throw ParseException( "Invalid 4-hex-digits" );
							str.append( (char*)buf, l );
							}
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

				throw ParseException( "Insufficient string", input.lineCount );
				}
				
			case '[':
				{
				++input.cur;
				type::array_t array;
				
				while( input.SkipWhiteSpace() )
					{
					array.push_back( ParseImpl( input ) );
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Insufficient array", input.lineCount );
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
						throw ParseException( "Key type is not string" );
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Insufficient object" );
					if( *input.cur != ':' )
						throw ParseException( "Malformed object" );
					++input.cur;
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Insufficient object" );
					type::value_ptr_t v = ParseImpl( input );
					
					type::string_t kstr;
					k->Get( kstr );
					object[ kstr ] = v;
					
					if( input.SkipWhiteSpace() == false )
						throw ParseException( "Insufficient object" );
					if( *input.cur == ',' )
						continue;
					if( *input.cur == '}' )
						break;
					}

				return type::object_value_ptr_t( new type::ObjectValue( object ) );
				}
			}
		throw ParseException( "Insufficient object" );
		}
	
	//-----------------------------------------------------------------------------------------//
	type::value_ptr_t Parse( const char *buf, size_t size, size_t *consumed )
		{
		InputRange input( buf, buf + size );
		type::value_ptr_t ret = ParseImpl( input );
		if( consumed != NULL )
			*consumed = input.GetConsumedSize();

		return ret;
		}
	}
