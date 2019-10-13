// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_ARRAY_H_
#define _CORE_ARRAY_H_
#include <Core/Object.h>
#include <Core/Types/Stack.h>
#include <Core/Types/String.h>
#include <Core/Types/ValueType.h>
#include <unordered_map>
namespace Meow {
	namespace Core {
		namespace Types {
			class ArrayElement;
			typedef Reference<ArrayElement> ArrayElement_ptr;

			template <class T> class Array : public Core::Types::ValueType
			{
			public:
				Array();
				virtual ~Array();
				void Initialise( unsigned long items, char * array[] );
				virtual void Set( string index, T item );
				virtual void Set( string index, unsigned long id, T item );
				virtual void Set( unsigned long index, T item );
				virtual T  Get( string index );
				virtual T  Get( unsigned long key );
				virtual bool IsSet( std::string key );
				virtual bool IsSet( unsigned long key );
				virtual unsigned long Count();
				virtual void Unset( std::string key );
				virtual void Unset( unsigned long key );
				virtual Core::Types::Stack_ptr<Core::Types::String_ptr> GetKeys( );
				virtual Core::Types::Stack_ptr<T> GetValues( );
				virtual unsigned long IndexOf( std::string key );
				virtual bool FindIn( Core::Types::ValueType_ptr value );
			
				// Inherited from valuetype
				/* Enquiries */
				virtual bool        IsNumeric();
				virtual bool        CouldBeFloat();
				virtual bool        CouldBeInteger();
				virtual std::string GetValueAsString();

				/* Operations */
				virtual void        Append( Core::Types::ValueType_ptr other );
				virtual void        Let( Core::Types::ValueType_ptr other );
				virtual bool        Equals( Core::Types::ValueType_ptr other );
				virtual bool        NotEquals( Core::Types::ValueType_ptr other );
				virtual Core::Types::ValueType_ptr       Left( );
				virtual Core::Types::ValueType_ptr       Right( );

				/* Additional queries */
				virtual std::string GetStringKey( unsigned long item );

				unsigned long Hash( std::string data );
				void Import(std::string data);
			private:
				unordered_map <std::string,T> elements;
			};
			typedef Reference<Array> Array_ptr;
		}
	}
} // End of namespace

#endif
