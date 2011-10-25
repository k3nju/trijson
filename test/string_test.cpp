#include <stdio.h>
#include <gtest/gtest.h>
#include "testtools.h"

TEST( StringTests, AsciiStringTest )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/string1.enc", "./build/testdata/string1.raw" ) );
	}

TEST( StringTests, EscapedCtrlCharTest )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/string2.enc", "./build/testdata/string2.raw" ) );
	}

TEST( StringTests, FourHexDigitsTest )
	{
	ASSERT_TRUE( CompareFile( "./build/testdata/string3.enc", "./build/testdata/string3.raw" ) );
	}

int main( int argc, char **argv )
	{
	::testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS();

	return 0;
	}
