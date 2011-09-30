#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <gtest/gtest.h>
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

bool Test( char *srcFile, char *cmpFile )
	{
	char srcBuf[1024];
	char cmpBuf[1024];

	int srcSize = Read( srcFile, srcBuf, sizeof( srcBuf ) - 1 );
	if( srcSize == -1 )
		{
		fprintf( stderr, "src file read error" );
		return false;
		}

	int cmpSize = Read( outFile, cmpBuf, sizeof( cmpBuf ) - 1 );
	if( cmpSize == -1 )
		{
		fprintf( stderr, "out file read error" );
		return false;
		}

	size_t sizeRead = 0;
	size_t consumed = 0;
	type::value_ptr_t val;
	
	try
		{
		val = Parse( srcBuf, srcSize, &consumed );
		}
	catch( ParseException &E )
		{
		fprintf( stderr, E.what() );
		return false;
		}

	string s;
	val->Get( s );

	EXPECT_EQ( consumed, srcSize );
	EXPECT_EQ( s.length(), cmpSize );
	EXPECT_EQ( memcmp( s.c_str(), cmpBuf, cmpSize ), 0 );
	}

TEST( StringTests, NormalTests )
	{
	Test( "./testdata/string1.enc", "./testdata/string1.raw" );
	}

int main( int argc, char **argv )
	{
	::testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS();

	return 0;
	}
