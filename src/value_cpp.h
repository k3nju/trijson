#pragma once
#include <sstream>
#include "value.h"
#include "exception.h"

namespace trijson
	{
	namespace type
		{
		//-----------------------------------------------------------------------------------------//
		// Value implementation types
		class NullValueImpl;
		class TrueValueImpl;
		class FalseValueImpl;
		class NumberValueImpl;
		class StringValueImpl;
		class ArrayValueImpl;
		class ObjectValueImpl;

		//-----------------------------------------------------------------------------------------//
		// Implementation holder types
		template < class > class ImplHolder;
		typedef ImplHolder< NullValueImpl   > NullImplHolder;
		typedef ImplHolder< TrueValueImpl   > TrueImplHolder;
		typedef ImplHolder< FalseValueImpl  > FalseImplHolder;
		typedef ImplHolder< NumberValueImpl > NumberImplHolder;
		typedef ImplHolder< StringValueImpl > StringImplHolder;
		typedef ImplHolder< ArrayValueImpl  > ArrayImplHolder;
		typedef ImplHolder< ObjectValueImpl > ObjectImplHolder;
		
		//-----------------------------------------------------------------------------------------//
		// Value types
		class NullValue;
		class TrueValue;
		class FalseValue;
		class NumberValue;
		class StringValue;
		class ArrayValue;
		class ObjectValue;

		//-----------------------------------------------------------------------------------------//
		// Pointer types
		typedef boost::shared_ptr< NullValue > null_value_ptr_t;
		typedef boost::shared_ptr< TrueValue > true_value_ptr_t;
		typedef boost::shared_ptr< FalseValue > false_value_ptr_t;
		typedef boost::shared_ptr< NumberValue > number_value_ptr_t;
		typedef boost::shared_ptr< StringValue > string_value_ptr_t;
		typedef boost::shared_ptr< ArrayValue > array_value_ptr_t;
		typedef boost::shared_ptr< ObjectValue > object_value_ptr_t;
		
		//-----------------------------------------------------------------------------------------//
		template < class T >
		struct ImplHolder
			{
				inline ImplHolder():impl(){};
				template < class A0 >
				inline ImplHolder( A0 &a0 ):impl( a0 ){};
				template < class A0, class A1 >
				inline ImplHolder( A0 &a0, A1 &a1 ):impl( a0, a1 ){};
				template < class A0, class A1, class A2 >
				inline ImplHolder( A0 &a0, A1 &a1, A2 &a2 ):impl( a0, a1, a2 ){};
				
				T impl;
			};
		
		//-----------------------------------------------------------------------------------------//
		// Null
		class NullValueImpl :public IValueImpl
			{
			public:
				inline NullValueImpl():IValueImpl(), null_( NULL ){};
				virtual const void* GetValue() const { return &null_; };

			private:
				const char *null_;
			};
		
		class NullValue :private NullImplHolder,
						 public Value
			{
			public:
				inline NullValue()
					:NullImplHolder(),
					 Value( null_type, &(NullImplHolder::impl) ){};

				virtual std::string Dump() const { return "null"; };
			};
		
		//-----------------------------------------------------------------------------------------//
		// True
		class TrueValueImpl :public IValueImpl
			{
			public:
				inline TrueValueImpl():IValueImpl(), true_( true ){};
				virtual const void* GetValue() const { return &true_; };

			private:
				bool true_;
			};
		
		class TrueValue :private TrueImplHolder,
						 public Value
			{
			public:
				inline TrueValue()
					:TrueImplHolder(),
					 Value( bool_type, &(TrueImplHolder::impl) ){};

				virtual std::string Dump() const { return "true"; };
			};

		//-----------------------------------------------------------------------------------------//
		// False
		class FalseValueImpl :public IValueImpl
			{
			public:
				inline FalseValueImpl():IValueImpl(), false_( false ){};
				virtual const void* GetValue() const { return &false_; };

			private:
				bool false_;
			};
		
		class FalseValue :private FalseImplHolder,
						  public Value
			{
			public:
				inline FalseValue()
					:FalseImplHolder(),
					 Value( bool_type, &(FalseImplHolder::impl) ){}

				virtual std::string Dump() const { return "false"; };
			};

		//-----------------------------------------------------------------------------------------//
		// Number
		class NumberValueImpl :public IValueImpl
			{
			public:
				inline explicit NumberValueImpl( number_t &num ):IValueImpl(), number_( num ){};
				virtual const void* GetValue() const { return &number_; };

			private:
				number_t number_;
			};
		
		class NumberValue :private NumberImplHolder,
						   public Value
			{
			public:
				inline explicit NumberValue( number_t num )
					:NumberImplHolder( num ),
					 Value( number_type, &(NumberImplHolder::impl) ){}
				virtual std::string Dump() const
					{
					std::stringstream ss;
					ss << *(number_t*)(NumberImplHolder::impl.GetValue());
					return ss.str();
					}
			};
		
		//-----------------------------------------------------------------------------------------//
		// String
		class StringValueImpl :public IValueImpl
			{
			public:
				inline StringValueImpl( const char *str, size_t size ):IValueImpl(), str_( str, size ){};
				inline explicit  StringValueImpl( const string_t &str ):IValueImpl(), str_( str ){};
				virtual const void* GetValue() const { return &str_; };
				
			private:
				string_t str_;
			};
		
		class StringValue :private StringImplHolder,
						   public Value
			{
			public:
				inline StringValue( const char *str, size_t size )
					:StringImplHolder( str, size ),
					 Value( string_type, &(StringImplHolder::impl) ){};
				inline explicit StringValue( const string_t &str )
					:StringImplHolder( str ),
					 Value( string_type, &(StringImplHolder::impl) ){};
				virtual std::string Dump() const { return *(string_t*)(StringImplHolder::impl.GetValue()); };
			};

		//-----------------------------------------------------------------------------------------//
		// Array
		class ArrayValueImpl :public IValueImpl
			{
			public:
				inline ArrayValueImpl():IValueImpl(), array_(){};
				inline ArrayValueImpl( const array_t &array ):IValueImpl(), array_( array ){};
				virtual const void* GetValue() const { return &array_; };
				
			private:
				array_t array_;
			};

		class ArrayValue :private ArrayImplHolder,
						  public Value
			{
			public:
				inline ArrayValue()
					:ArrayImplHolder(),
					 Value( array_type, &(ArrayImplHolder::impl) ){};
				inline ArrayValue( const array_t &array )
					:ArrayImplHolder( array ),
					 Value( array_type, &(ArrayImplHolder::impl) ){};
				inline void Append( value_ptr_t value ){ GetArray()->push_back( value ); };
				virtual std::string Dump() const
					{
					std::stringstream ss;
					ss << "[";
					array_t::const_iterator begin = GetArray()->begin();
					array_t::const_iterator end   = GetArray()->end();
					for(; begin != end; ++begin )
						{
						ss << (*begin)->Dump();
						if( begin + 1 != end )
							ss << ",";
						}
					ss << "]";

					return ss.str();
					}

			private:
				inline const array_t* GetArray() const { return ArrayImplHolder::impl.Cast< array_t >(); };
				inline array_t* GetArray(){ return const_cast< array_t* >( static_cast< const ArrayValue* >( this )->GetArray() ); };
			};

		//-----------------------------------------------------------------------------------------//
		// Object
		class ObjectValueImpl :public IValueImpl
			{
			public:
				inline ObjectValueImpl():IValueImpl(){};
				inline ObjectValueImpl( const object_t &object ):IValueImpl(), object_( object ){};
				virtual const void* GetValue() const { return &object_; };
				
			private:
				object_t object_;
			};

		class ObjectValue :private ObjectImplHolder,
						   public Value
			{
			public:
				inline ObjectValue()
					:ObjectImplHolder(),
					 Value( object_type, &(ObjectImplHolder::impl) ){};
				inline ObjectValue( const object_t &object )
					:ObjectImplHolder( object ),
					 Value( object_type, &(ObjectImplHolder::impl) ){};
				void Insert( const value_ptr_t key, value_ptr_t value );
				value_ptr_t operator[] ( const std::string &key );
				virtual std::string Dump() const
					{
					std::stringstream ss;
					ss << "{";
					object_t::const_iterator begin = GetObject()->begin();
					object_t::const_iterator end   = GetObject()->end();

					if( begin++ != end )
						ss << "\"" << begin->first << "\":" << begin->second->Dump();
					for(; begin != end; ++begin )
						ss << "," << "\"" << begin->first << "\":" << begin->second->Dump();
					
					ss << "}";
					return ss.str();
					}
				
			private:
				inline const object_t* GetObject() const { return ObjectImplHolder::impl.Cast< object_t >(); };
				inline object_t* GetObject(){ return const_cast< object_t* >( static_cast< const ObjectValue* >(this)->GetObject() ); };
			};
		
		//-----------------------------------------------------------------------------------------//
		void ObjectValue::Insert( const value_ptr_t key, value_ptr_t value )
			{
			string_t k;
			if( key->Get( k ) == false )
				throw CastException( "Type mismatch", key->GetType(), string_type );
			
			GetObject()->insert( std::make_pair( k, value ) );
			}

		//-----------------------------------------------------------------------------------------//
		value_ptr_t ObjectValue::operator[] ( const std::string &key )
			{
			object_t::iterator iter = GetObject()->find( key );
			if( iter == GetObject()->end() )
				throw KeyException( key.c_str() );
			
			return iter->second;
			}
		}
	}
