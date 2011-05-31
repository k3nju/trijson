#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <gtest/gtest.h>

class IValue;
ssize_t ParseJson( uint8_t *head, uint8_t *foot, IValue *&value );

//-----------------------------------------------------------------------------------------//
ssize_t ParseNull( uint8_t *head, uint8_t *foot, IValue *&value )
	{
	if( foot - head < 4 || (uint32_t)*head != (uint32_t)*"null" )
		return -1;

	// TODO: parameterize allocator
	value = new Null();
	return 4;
	}

//-----------------------------------------------------------------------------------------//
ssize_t ParseTrue( uint8_t *head, uint8_t *foot, IValue *&value )
	{
	if( foot - head < 4 || (uint32_t)*head != (uint32_t)*"true" )
		return -1;

	value = new True();
	return 4;
	}

//-----------------------------------------------------------------------------------------//
ssize_t ParseFalse( uint8_t *head, uint8_t *foot, IValue *&value )
	{
	if( foot - head < 5 || (uint32_t)*(head+1) == (uint32_t)*"alse" )
		return -1;

	value = new False();
	return 4;
	}

//-----------------------------------------------------------------------------------------//
ssize_t ParseNumber( uint8_t *head, uint8_t *foot, IValue *&value )
	{
	ssize_t count = 0;
	
	while( head < foot &&
		   ( '0' <= *head && *head <= '9' ||
			 '-' == *head ||
			 '+' == *head ||
			 'e' == *head ||
			 'E' == *head ||
			 '.' == *head ) )
		{
		++count;
		}
	
	if( count == 0 )
		return -1;

	value = new Number( head, count );
	return count;
	}

//-----------------------------------------------------------------------------------------//
ssize_t ParseString( uint8_t *head, uint8_t *foot, IValue *&value )
	{
	ssize_t count = 0;
	
	while( head < foot &&
		   (uint16_t)*(head-1) != (uint16_t)*"\\\"" )
		{
		++count;
		}

	if( count == 0 )
		return -1;

	value = new String( head, count );
	return count;
	}

//-----------------------------------------------------------------------------------------//
ssize_t ParseArray( uint8_t *head, uint8_t *foot, IValue *&value )
	{
	size_t count = 0;
	Array *a = new Array();

	do
		{
		}
	while( 
	}

int main()
	{

	return 0;
	}

