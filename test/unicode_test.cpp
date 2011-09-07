#include <stdio.h>
#include <stdint.h>
#include <assert.h>

<<<<<<< HEAD
bool Str2Uint16( const char *src, uint16_t *dst )
	{
	uint16_t num = 0;
	char *p = (char*)&num;
	for( int i = 0; i < 4; ++i, ++p )
		{
	
	for( int i = 0; i < 4; ++i )
		{
		num <<= 4;
>>>>>>> 97f5718957b7b4bdfd59b52d2970977fa6e4aab7
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
			return false;
		}
	*dst = num;
	
	return true;
	}

bool Parse( const char *src, size_t srcSize, char *dst, size_t dstSize )
	{
	if( srcSize < 4 )
		return false;

	uint16_t num = 0;
	if( Str2Uint16( src, &num ) == false )
		return false;
	srcSize -= 4;
	
	// surrogate pair	
	if( 0xd800 <= num && num <= 0xdbff )
		{
		if( srcSize < 4 )
			return false;

<<<<<<< HEAD
		uint16_t second = 0;
		if( Str2Uint16( 
		}
	
=======
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
>>>>>>> 97f5718957b7b4bdfd59b52d2970977fa6e4aab7
	
	}

int main()
	{
	Conv2ShortTest();
	
	return 0;
	}
