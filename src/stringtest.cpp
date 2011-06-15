#include <stdio.h>
#include <assert.h>
#include <string>
#include <cstring>
//#include "value.h"
#include "trijson.h"
#include "value_cpp.h"

using namespace std;
using namespace trijson;
using namespace trijson::type;

void Test()
	{
	const char buf[] = "\"ABC\\\"CD\nEFG\"";
	size_t consumed = 0;
	value_ptr_t v = Parse( buf, strlen(buf), &consumed );
	assert( consumed == 13 );
	std::string s;
	v->Get(s);
	puts( s.c_str() );
	}


int main()
	{
	Test();
	return 0;
	}
