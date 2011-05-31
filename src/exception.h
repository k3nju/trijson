#pragma once
#include <stdexcept>

namespace speedyjson
	{
	//-----------------------------------------------------------------------------------------//
	// Throws when invalid cast happend.
	struct CastException :public std::runtime_error
		{
			CastException( const char *msg,
						   value_type_t from,
						   value_type_t to )
				:std::runtime_error( msg ),
				 typeFrom( from ),
				 typeTo( to ){};
			virtual ~CastException() throw(){};
			
			const value_type_t typeFrom;
			const value_type_t typeTo;
		};

	//-----------------------------------------------------------------------------------------//
	// Throws when key is not found on an object value.
	struct KeyException :public std::runtime_error
		{
			KeyException( const char *key )
				:std::runtime_error( key ){};
		};
	
	//-----------------------------------------------------------------------------------------//
	// Throws when parse error happend.
	struct ParseException :public std::runtime_error
		{
			ParseException( const char *msg )
				:std::runtime_error( msg ){};
		};
	}
