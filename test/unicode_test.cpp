#include <stdio.h>
#include <stdint.h>

bool Str2Uint16( const char *src, uint16_t *dst )
	{
	uint16_t num = 0;
	char *p = (char*)&num;
	for( int i = 0; i < 4; ++i, ++p )
		{
		// '0' - '9'
		if( 0x30 <= *src && *src <= 0x39 )
			*p = *src - 0x30;
		// 'A' - 'F'
		else if( 0x41 <= *src && *src <= 0x46 )
			*p = *src - 0x37;
		// 'a' - 'f'
		else if( 0x61 <= *src && *src <= 0x66 )
			*p = *src - 0x57;
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

		uint16_t second = 0;
		if( Str2Uint16( 
		}
	
	
	}

int main()
	{
	Test();
	
	return 0;
	}
