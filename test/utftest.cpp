#include <gtest/gtest.h>
#include "testtools.h"

TEST( utf8test, Test1Byte )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/testdata/utf8_1.enc", "./build/testdata/testdata/utf8_1.raw" ) );
	}

int main( int argc, char **argv )
	{
	::testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS();
	}
