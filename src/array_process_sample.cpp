#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>

#include "trijson.h"

using namespace std;
using namespace trijson;
using namespace trijson::type;


int main()
	{
	const char raw[] = "[1,2,3]";
	size_t rawLen = strlen( raw );
	const char *head = raw;
	const char *foot = head + rawLen;
	bool success = false;

	try
		{
		array_t arr;
		while( head < foot )
			{
			++head;
			head = SkipWhiteSpace( head, foot );
			if( head == NULL )
				break;
			
			size_t arrayConsumed = 0;
			value_ptr_t val = Parse( head, foot - head, &arrayConsumed );
			arr.push_back( val );

			printf( "arrayConsumed = %d\n", arrayConsumed );
			
			head += arrayConsumed;
			if( head >= foot )
				break;
			head = SkipWhiteSpace( head, foot );
			if( head == NULL )
				break;
			
			if( *head == ',' )
				{
				continue;
				}
			else if( *head == ']' )
				{
				success = true;
				break;
				}
			else
				{
				break;
				}
			}

		if(success)
			{
			for( array_t::iterator iter = arr.begin();
				 iter != arr.end();
				 ++iter )
				{
				double d = 0.0;
				printf( "%d ", (*iter)->Get( d ) );
				printf( "%f\n", d );
				}
			}
		else
			{
			puts( "failed" );
			}
		}
	catch( ParseException &E )
		{
		puts( E.what() );
		}
	}

