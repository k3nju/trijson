#include <stdio.h>
#include <stdint.h>

void Test()
	{
	char srcVal[] = "1234";
	char *src = srcVal;
	uint32_t num = 0;
	char *p = (char*)&num;
	for( int i = 0; i < 4; ++i, ++p )
		{
//		printf( "%x\n", src[i] );
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
			puts( "Error" );
		}

	printf( "%x\n", num );
	}

int main()
	{
	Test();
	
	return 0;
	}
