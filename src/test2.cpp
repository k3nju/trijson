#include "trijson.h"
#include "value.h"
#include "value_cpp.h"
#include <string.h>
#include <cassert>
#include <stdio.h>

int main()
	{
	using namespace trijson;
	using namespace trijson::type;

	{
	char buf[] = "null";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed == 4 );
	assert( v->GetType() == type::null_type );
	}

	{
	char buf[] = "true";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed == 4 );
	assert( v->GetType() == type::bool_type );
	}

	{
	char buf[] = "false";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed == 5 );
	assert( v->GetType() == type::bool_type );
	}

	{
	char buf[] = "1.1234";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed == 6 );
	assert( v->GetType() == type::number_type );
	}


	{
	char buf[] = "\"ABC\\\"\"";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed == 7 );
	assert( v->GetType() == type::string_type );
	string_t s;
	v->Get( s );
	assert( s == "ABC\\\"" );
	}

	{
	char buf[] = "[null,true , false, 1]";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed == strlen( buf ) );
	array_t arr;
	v->Get( arr );
	
	void *p;
	arr[0]->Get( p );
	assert( p == NULL );

	bool t;
	arr[1]->Get( t );
	assert( t == true );

	arr[2]->Get( t );
	assert( t == false );

	double d;
	arr[3]->Get( d );
	assert( d == 1 );
	}

	/*
	{
	char buf[] = "{\"str\" : 123 }";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen( buf ), &consumed );
	assert( consumed = strlen( buf ) );

	object_t o;
	v->Get( o );

	double num;
	o["str"]->Get( num );
	assert( num == 123 );
	}
	*/

	return 0;
	}
