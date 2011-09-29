#pragma once
#include <algorithm>
#include <string>
#include <stdint.h>

namespace trijson
	{
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
	inline uint32_t StrToUint32( const char *in, size_t size, uint32_t *out )
		{
		uint32_t num = 0;
		size_t count = std::min( size, sizeof( uint32_t ) );
		size_t i = 0;
		
		for( i = 0; i < count; ++i )
			{
			num <<= 4;
			
			if( 0x30 <= in[i] && in[i] <= 0x39 )
				{
				num |= in[i] - 0x30;
				}
			else if( 0x41 <= in[i] && in[i] <= 0x46 )
				{
				num |= in[i] - 0x37;
				}
			else if( 0x61 <= in[i] && in[i] <= 0x66 )
				{
				num |= in[i] - 0x57;
				}
			else
				break;
			}
		
		if( i == 0 )
			return 0;
		
		*out = num;
		return i;
		}

	//-----------------------------------------------------------------------------------------//
	size_t EncodeFromUTF16ToUTF8( uint32_t in, uint8_t *out )
		{
#define AS_UINT8( n ) (uint8_t)( n & 0xff )
		size_t ret = 0;
		
		if( in < 0x80 )
			{
			out[0] = AS_UINT8( in );
			ret = 1;
			}
		else if( in < 0x800 )
			{
			out[0] = 0xc0 | AS_UINT8( in >> 0x06 );
			out[1] = 0x80 | AS_UINT8( in & 0x3f );
			ret = 2;
			}
		else if( in < 0x10000 )
			{
			out[0] = 0xe0 | AS_UINT8( in >> 0x0b );
			out[1] = 0x80 | AS_UINT8( ( in >> 0x06 ) & 0x3f );
			out[2] = 0x80 | AS_UINT8( in & 0x3f );
			ret = 3;
			}
		else if( in < 0x10ffff )
			{
			out[0] = 0xf0 | AS_UINT8( in >> 0x12 );
			out[1] = 0x80 | AS_UINT8( ( in >> 0x0b ) & 0x3f );
			out[2] = 0x80 | AS_UINT8( ( in >> 0x06 ) & 0x3f );
			out[3] = 0x80 | AS_UINT8( in & 0x3f );
			ret = 4;
			}
		else
			{
			ret = 0;
			}

		return ret;
#undef AS_UINT8
		}

	//-----------------------------------------------------------------------------------------//
	inline void CopyToVector( std::vector<char> &v, const char *src, size_t offset, size_t size )
		{
		if( size == 0 )
			return;

		size_t voffset = v.size();
		if( v.capacity() < v.size() + size )
			v.resize( v.size() + size );
		memmove( &v[voffset], src + offset, size );
		}
	}
