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
	char *head = raw;
	char *foot = head + rawLen;

	while( head < foot )
		{
		head = SkipWhiteSpace( head, foot );
		if( head == NULL )
			break;
		
		Parse(
		}
	}
