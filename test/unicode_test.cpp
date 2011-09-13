#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <algorithm>
using namespace std;



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
