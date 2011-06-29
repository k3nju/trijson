#pragma once
#include <stdexcept>

namespace trijson
	{
	//-----------------------------------------------------------------------------------------//
	// Thrown when invalid cast happend.
	struct CastException :public std::runtime_error
		{
			CastException( const char *msg,
						   type::value_type_t from,
						   type::value_type_t to )
				:std::runtime_error( msg ),
				 typeFrom( from ),
				 typeTo( to ){};
			virtual ~CastException() throw(){};
			
			const type::value_type_t typeFrom;
			const type::value_type_t typeTo;
		};

	//-----------------------------------------------------------------------------------------//
	// Thrown when key is not found on an object value.
	struct KeyException :public std::runtime_error
		{
			explicit KeyException( const char *key )
				:std::runtime_error( key ){};
		};
	
	//-----------------------------------------------------------------------------------------//
	// Thrown when parse error happend.
	struct ParseException :public std::runtime_error
		{
			inline ParseException( const char *msg )
				:std::runtime_error( msg ), line( 0 ){};
			inline ParseException( const char *msg, unsigned int l )
				:std::runtime_error( msg ), line( l ){};
			unsigned int line;
		};
	}
