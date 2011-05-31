#include <stdio.h>
#include <assert.h>
//#include "value.h"
#include "value_cpp.h"

using namespace speedyjson::type;

void NullTest()
	{
	NullValue null;
	Value &v = null;
	void *p = (void*)1;
	v.Get( p );
	assert( p == NULL );
	}

void TrueTest()
	{
	TrueValue t;
	Value &v = t;
	bool b = false;
	v.Get( b );
	assert( b == true );
	}

void FalseTest()
	{
	FalseValue f;
	Value &v = f;
	bool b = true;
	v.Get( b );
	assert( b == false );
	}

void NumberTest()
	{
	NumberValue n( 0.1 );
	Value &v = n;
	number_t num = 0xff;
	v.Get( num );
	assert( num == 0.1 );
	}

void StringTest()
	{
	StringValue s( "ABC", 3 );
	Value &v = s;
	string_t str = "DDD";
	v.Get( str );
	assert( str == "ABC" );	
	}

void ArrayTest()
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
	assert( str2 == "ABC" );

	number_t num2;
	arr[1]->Get( num2 );
	assert( num2 == 1.234 );

	null_t null2;
	arr[2]->Get( null2 );
	assert( null2 == NULL );

	bool_t t2;
	arr[3]->Get( t2 );
	assert( t2 == true );
	
	bool_t f2;
	arr[4]->Get( f2 );
	assert( f2 == false );
	}

void ObjectTest()
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
	assert( s == "ABC" );
	}
	
	{
	double d;
	object["num"]->Get( d );
	assert( d == 1.234 );
	}

	{
	void *n;
	object["null"]->Get( n );
	assert( n == NULL );
	}

	{
	bool t;
	object["true"]->Get( t );
	assert( t == true );
	}

	{
	bool f;
	object["false"]->Get( f );
	assert( f == false );
	}

	{
	array_t a;
	object["arr"]->Get(a);

	{
	string_t s;
	a[0]->Get( s );
	assert( s == "ABC" );
	}

	{
	double d;
	a[1]->Get( d );
	assert( d == 1.234 );
	}

	{
	void *n;
	a[2]->Get( n );
	assert( n == NULL );
	}

	{
	bool t;
	a[3]->Get( t );
	assert( t == true );
	}

	{
	bool f;
	a[4]->Get( f );
	assert( f == false );
	}

	}

	}

int main()
	{
	NullTest();
	TrueTest();
	FalseTest();
	NumberTest();
	StringTest();
	ArrayTest();
	ObjectTest();

	string_value_ptr_t v( new StringValue( "ABC", 3 ) );
	
	return 0;
	}
