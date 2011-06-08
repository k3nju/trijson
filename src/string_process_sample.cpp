#include <stdio.h>
#include <string>
#include <cstring>
#include <cassert>

using namespace std;

inline bool parseEscapeString( char in, char *out )
	{
	switch( in )
		{
		case 'b':
			*out = '\b';
			break;
		case 'f':
			*out = '\f';
			break;
		case 'n':
			*out = '\n';
			break;
		case 'r':
			*out = '\r';
			break;

		case 't':
			*out = '\t';
			break;
			
		case '"':
		case '/':
		case '\\':
			*out = in;
			break;

		default:
			return false;
		}

	return true;
	}

int main()
	{
	string s;
	char *raw = (char*)"\"ABC\n\\\"D\"";
	size_t rawLen = strlen( raw );
	char *head = raw;
	char *foot = head + rawLen;

	++head; // now head points to head of string value.
	
	char *copyStart = head;

	bool ok = false;

	while( head < foot )
		{
		if( *(head-1) != '\\' && *head == '"' )
			{
			if( head - copyStart > 0 )
				{
				s.append( copyStart, head - copyStart );
				}
			ok = true;
			break;
			}

		if( *head == '\\' )
			{
			++head; // now head points to the next of escape string.
			if( head >= foot )
				break;
			
			char c;
			if( parseEscapeString( *head, &c ) == false )
				break;
			
			if(  head - copyStart > 0 )
				s.append( copyStart, head - copyStart );
			
			copyStart = ++head;
			}
		++head;
		}

	assert( ok == true );
	puts( s.c_str() );
	
	return 0;
	}

