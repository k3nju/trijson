#pragma once
#include <stddef.h>
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

namespace trijson
	{
	namespace type
		{
		//-----------------------------------------------------------------------------------------//
		// Declaration of types
		class Value;
		typedef boost::shared_ptr< Value > value_ptr_t;
		typedef char invalid_type_t;
		typedef void* null_t;
		typedef bool bool_t;
		typedef double number_t;
		typedef std::string string_t;
		typedef std::vector<value_ptr_t> array_t;
		typedef std::map<string_t, value_ptr_t> object_t;
		
		//-----------------------------------------------------------------------------------------//
		// Enumeration of types
		typedef enum
			{
			invalid_type,
			null_type,
			bool_type,
			number_type,
			string_type,
			array_type,
			object_type
			} value_type_t;
		
		//-----------------------------------------------------------------------------------------//
		// Interface of data storage for each types
		class IValueImpl
			{
			public:
				virtual ~IValueImpl(){};
				
				// GetValue() return pointer to actual data.
				virtual void* GetValue() const = 0;
				// Sugar.
				template < class T >
				T* Cast() const { return (T*)GetValue(); };
			};

		//-----------------------------------------------------------------------------------------//
		// Common interface for convertion from JSON types to C/C++ types.
		class Value
			{
			public:
				inline Value( value_type_t type, IValueImpl *holder ):type_( type ), holder_( holder ){};
				inline value_type_t GetType(){ return type_; };
				
				// Primary member function template.
				template < class T > bool Get( T &out ) const;
				virtual std::string Dump() const = 0;
				
			protected:
				value_type_t type_;
				IValueImpl *holder_;
			};

		//-----------------------------------------------------------------------------------------//
		// Convertion functions for each types.
#define DEF_GET( T )												\
		template <> bool Value::Get<T##_t>( T##_t& out )			\
			{														\
			if( type_ != T##_type )									\
				return false;										\
			out = *(T##_t*)(holder_->GetValue());					\
			return true;											\
			}
		DEF_GET( null );
		DEF_GET( bool );
		DEF_GET( number );
		DEF_GET( string );
		DEF_GET( array );
		DEF_GET( object );
#undef DEF_GET
		}
	}
	
