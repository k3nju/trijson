#include <stdio.h>
#include <stdint.h>
#include <assert.h>

uint16_t Conv2Short( char *srcVal )
	{
	uint16_t num = 0;
	
	for( int i = 0; i < 4; ++i )
		{
		num <<= 4;
		// '0' - '9'
		if( 0x30 <= srcVal[i] && srcVal[i] <= 0x39 )
			num |= srcVal[i] - 0x30;
		// 'A' - 'F'
		else if( 0x41 <= srcVal[i] && srcVal[i] <= 0x46 )
			num |= srcVal[i] - 0x41 + 0x0a;
		// 'a' - 'f'
		else if( 0x61 <= srcVal[i] && srcVal[i] <= 0x66 )
			num |= srcVal[i] - 0x61 + 0xa;
		else
			puts( "Error" );
		}

	return num;
	}

void Conv2ShortTest()
	{
	assert( Conv2Short( "0990" ) == 0x0990 );
	assert( Conv2Short( "affa" ) == 0xaffa );
	assert( Conv2Short( "AFFA" ) == 0xaffa );
	assert( Conv2Short( "0aAF" ) == 0x0aaf );
	}

size_t EncodeUtf8( uint16_t src, char *dst, int dstSize )
	{
	
	}

int main()
	{
	Conv2ShortTest();
	
	return 0;
	}
