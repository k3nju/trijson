#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <algorithm>
using namespace std;

uint32_t StrToUint32( const char *src, size_t size )
	{
	uint32_t num = 0;
	int count = min( (int)size, 8 );
	for( int i = 0; i < count; ++i )
		{
		num <<= 4;

		if( 0x30 <= src[i] && src[i] <= 0x39 )
			{
			num |= src[i] - 0x30;
			}
		else if( 0x41 <= src[i] && src[i] <= 0x46 )
			{
			num |= src[i] - 0x37;
			}
		else if( 0x61 <= src[i] && src[i] <= 0x66 )
			{
			num |= src[i] - 0x57;
			}
		else
			break;
		}
	
	return num;
	}

bool Utf16ToUtf8( 
	{
	while( srcSize > 0 )
		{
		uint32_t codepoint = StrToUint32( src, srcSize );
		
		if( 0xd8000 <= codepoint && codepoint <= 0xdbff )
			{
			// surrogate pair
			uint32_t codepoint2 = StrToUint32( src,
			}
		}
	}


int main()
	{
	
	return 0;
	}
