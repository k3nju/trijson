#include <gtest/gtest.h>
#include "testtools.h"

TEST( utf8test, Test1Byte )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/utf8_1.enc", "./build/testdata/utf8_1.raw" ) );
	}

TEST( utf8test, Test2Byte )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/utf8_2.enc", "./build/testdata/utf8_2.raw" ) );
	}

TEST( utf8test, Test2Byte2 )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/utf8_3.enc", "./build/testdata/utf8_3.raw" ) );
	}

int main( int argc, char **argv )
	{
	::testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS();
	}
