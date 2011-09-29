#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include "trijson.h"

using namespace std;
using namespace trijson;

TEST( StringTests, EscapeStrings )
	{
	int fd = open( "test/escape_string.dat", O_RDONLY );
	if( fd == -1 )
		{
		perror( "open() failed" );
		return;
		}

	// "\"\\\/\b\f\n\r\t"
	char buf[1024];
	int sizeRead = read( fd, buf, sizeof( buf ) - 1 );
	if( sizeRead == -1 )
		{
		perror( "read() failed" );
		return;
		}

	close( fd );
	
	buf[sizeRead] = '\0';
	size_t consumed = 0;
	type::value_ptr_t val;
	
	try
		{
		val = Parse( buf, sizeRead, &consumed );
		}
	catch( ParseException &E )
		{
		puts( E.what() );
		}
	
	string s;
	val->Get( s );

	EXPECT_EQ( consumed, sizeRead );
	EXPECT_EQ( s, "\"\\/\b\f\n\r\t" );
	}

int main( int argc, char **argv )
	{
	::testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS();

	return 0;
	}
