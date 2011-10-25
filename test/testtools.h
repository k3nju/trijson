#include <stdio.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string>
#include "trijson.h"

using namespace std;
using namespace trijson;

int ReadFile( const char *file, char *out, size_t outSize )
	{
	int fd = open( file, O_RDONLY );
	if( fd == -1 )
		{
		perror( "open() failed" );
		return -1;
		}
	
	int sizeRead = read( fd, out, outSize );
	if( sizeRead == -1 )
		{
		perror( "read() failed" );
		close( fd );
		
		return -1;
		}

	close( fd );
	
	return sizeRead;
	}

bool CompareFile( const char *encFile, const char *rawFile )
	{
	char encBuf[1024];
	char rawBuf[1024];

	int encSize = ReadFile( encFile, encBuf, sizeof( encBuf ) - 1 );
	if( encSize == -1 )
		{
		fprintf( stderr, "%s",  "enc file read error" );
		return false;
		}

	int rawSize = ReadFile( rawFile, rawBuf, sizeof( rawBuf ) - 1 );
	if( rawSize == -1 )
		{
		fprintf( stderr, "%s",  "out file read error" );
		return false;
		}

	size_t consumed = 0;
	type::value_ptr_t val;
	
	try
		{
		val = Parse( encBuf, encSize, &consumed );
		}
	catch( ParseException &E )
		{
		fprintf( stderr, "%s:%d\n", E.what(), E.line );
		return false;
		}

	string s;
	val->Get( s );

	EXPECT_EQ( consumed, encSize );
	EXPECT_EQ( s.length(), rawSize );
	EXPECT_EQ( memcmp( s.c_str(), rawBuf, rawSize ), 0 );

	return true;
	}
