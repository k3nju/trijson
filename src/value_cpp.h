#pragma once
#include "value.h"

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
		template < class > ImplHolder;
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
				virtual void* GetValue(){ return &null_; };

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
			};
		
		//-----------------------------------------------------------------------------------------//
		// True
		class TrueValueImpl :public IValueImpl
			{
			public:
				inline TrueValueImpl():IValueImpl(), true_( true ){};
				virtual void* GetValue(){ return &true_; };

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
			};

		//-----------------------------------------------------------------------------------------//
		// False
		class FalseValueImpl :public IValueImpl
			{
			public:
				inline FalseValueImpl():IValueImpl(), false_( false ){};
				virtual void* GetValue(){ return &false_; };

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
			};

		//-----------------------------------------------------------------------------------------//
		// Number
		class NumberValueImpl :public IValueImpl
			{
			public:
				inline NumberValueImpl( number_t &num ):IValueImpl(), number_( num ){};
				virtual void* GetValue(){ return &number_; };

			private:
				number_t number_;
			};
		
		class NumberValue :private NumberImplHolder,
						   public Value
			{
			public:
				inline NumberValue( number_t num )
					:NumberImplHolder( num ),
					 Value( number_type, &(NumberImplHolder::impl) ){}
			};
		
		//-----------------------------------------------------------------------------------------//
		// String
		class StringValueImpl :public IValueImpl
			{
			public:
				inline StringValueImpl( const char *str, size_t size ):IValueImpl(), str_( str, size ){};
				virtual void* GetValue(){ return &str_; };
				
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
			};

		//-----------------------------------------------------------------------------------------//
		// Array
		class ArrayValueImpl :public IValueImpl
			{
			public:
				inline ArrayValueImpl():IValueImpl(){};
				virtual void* GetValue(){ return &array_; };
				
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
				inline void Append( value_ptr_t value ){ GetArray()->push_back( value ); };

			private:
				inline array_t* GetArray(){ return ArrayImplHolder::impl.Cast< array_t >(); };
			};

		//-----------------------------------------------------------------------------------------//
		// Object
		class ObjectValueImpl :public IValueImpl
			{
			public:
				inline ObjectValueImpl():IValueImpl(){};
				virtual void* GetValue(){ return &object_; };
				
			private:
				object_t object_;
			};

		class ObjectValue :private ObjectImplHolder,
						   public Value
			{
			public:
				ObjectValue():ObjectImplHolder(),
							  Value( array_type, &(ObjectImplHolder::impl) ){};
				void Insert( const value_ptr_t key, value_ptr_t value );
				value_ptr_t operator[] ( const std::string &key );
				
			private:
				object_t* GetObject(){ return ObjectImplHolder::impl.Cast< object_t >(); };
			};
		
		//-----------------------------------------------------------------------------------------//
		void ObjectValue::void Insert( const value_ptr_t key, value_ptr_t value )
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
				throw KeyException();
			
			return iter->second;
			}
		}
	}
