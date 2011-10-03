#include <stdio.h>
#include <string>
#include <gtest/gtest.h>
#include "value_cpp.h"

using namespace std;
using namespace trijson::type;

TEST( TypeTests, NullTest )
	{
	NullValue null;
	Value &v = null;
	void *p = (void*)1;
	v.Get( p );
	
	EXPECT_EQ( p, NULL );
	EXPECT_EQ( v.Dump(), string( "null" ) );
	}

TEST( TypeTests, TrueTest )
	{
	TrueValue t;
	Value &v = t;
	bool b = false;
	v.Get( b );
	EXPECT_EQ( b, true );
	EXPECT_EQ( v.Dump(), string( "true" ) );
	}

TEST( TypeTests, FalseTest )
	{
	FalseValue f;
	Value &v = f;
	bool b = true;
	v.Get( b );
	EXPECT_EQ( b, false );
	EXPECT_EQ( v.Dump(), string( "false" ) );
	}

TEST( TypeTests, NumberTest )
	{
	NumberValue n( 0.1 );
	Value &v = n;
	number_t num = 0xff;
	v.Get( num );
	EXPECT_EQ( num, 0.1 );
	EXPECT_EQ( v.Dump(), "0.1" );
	}

TEST( TypeTests, StringTest )
	{
	StringValue s( "ABC", 3 );
	Value &v = s;
	string_t str = "DDD";
	v.Get( str );
	EXPECT_EQ( str, "ABC" );
	EXPECT_EQ( v.Dump(), "\"ABC\"" );
	
	StringValue s2( string_t("ABC") );
	Value &v2 = s2;
	string_t str2 = "DDD";
	v2.Get( str2 );
	EXPECT_EQ( str2, "ABC" );
	EXPECT_EQ( v2.Dump(), "\"ABC\"" );
	}

TEST( TypeTests, ArrayTest )
	{
	value_ptr_t str( new StringValue( "ABC", 3 ) );
	value_ptr_t num( new NumberValue( 1.234 ) );
	value_ptr_t null( new NullValue() );
	value_ptr_t t( new TrueValue() );
	value_ptr_t f( new FalseValue() );
	ArrayValue a;
	a.Append( str );
	a.Append( num );
	a.Append( null );
	a.Append( t );
	a.Append( f );
	array_t arr;
	a.Get( arr );

	string_t str2;
	arr[0]->Get( str2 );
	EXPECT_EQ( str2, "ABC" );

	number_t num2 = 0;
	arr[1]->Get( num2 );
	EXPECT_EQ( num2, 1.234 );

	null_t null2 = NULL;
	arr[2]->Get( null2 );
	EXPECT_EQ( null2, NULL );

	bool_t t2 = false;
	arr[3]->Get( t2 );
	EXPECT_EQ( t2, true );
	
	bool_t f2 = true;
	arr[4]->Get( f2 );
	EXPECT_EQ( f2, false );

	EXPECT_EQ( a.Dump(), "[\"ABC\",1.234,null,true,false]" );
	}

TEST( TypeTest, ObjectTest )
	{
	value_ptr_t str( new StringValue( "ABC", 3 ) );
	value_ptr_t num( new NumberValue( 1.234 ) );
	value_ptr_t null( new NullValue() );
	value_ptr_t t( new TrueValue() );
	value_ptr_t f( new FalseValue() );
	array_value_ptr_t a( new ArrayValue() );
	a->Append( str );
	a->Append( num );
	a->Append( null );
	a->Append( t );
	a->Append( f );
	ObjectValue object;
	object.Insert( value_ptr_t( new StringValue( "str", 3 ) ), str );
	object.Insert( value_ptr_t( new StringValue( "num", 3 ) ), num );
	object.Insert( value_ptr_t( new StringValue( "null", 4 ) ), null );
	object.Insert( value_ptr_t( new StringValue( "true", 4 ) ), t );
	object.Insert( value_ptr_t( new StringValue( "false", 5 ) ), f );
	object.Insert( value_ptr_t( new StringValue( "arr", 3 ) ), a );

	{
	string_t s;
	object["str"]->Get( s );
	EXPECT_EQ( s, "ABC" );
	}
	
	{
	double d = 0.0;
	object["num"]->Get( d );
	EXPECT_EQ( d, 1.234 );
	}

	{
	void *n = NULL;
	object["null"]->Get( n );
	EXPECT_EQ( n, NULL );
	}

	{
	bool t = false;
	object["true"]->Get( t );
	EXPECT_EQ( t, true );
	}

	{
	bool f = true;
	object["false"]->Get( f );
	EXPECT_EQ( f, false );
	}

	{
	array_t a;
	object["arr"]->Get(a);
	
	{
	string_t s;
	a[0]->Get( s );
	EXPECT_EQ( s, "ABC" );
	}

	{
	double d = 0.0;
	a[1]->Get( d );
	EXPECT_EQ( d, 1.234 );
	}

	{
	void *n = NULL;
	a[2]->Get( n );
	EXPECT_EQ( n, NULL );
	}

	{
	bool t = false;
	a[3]->Get( t );
	EXPECT_EQ( t, true );
	}

	{
	bool f = true;
	a[4]->Get( f );
	EXPECT_EQ( f, false );
	}
	}
	}

int main( int argc, char **argv )
	{
	::testing::InitGoogleTest( &argc, argv );
	RUN_ALL_TESTS();
	
	return 0;
	}

