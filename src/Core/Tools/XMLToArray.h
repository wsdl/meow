// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_XMLTOARRAY_H_
#define _CORE_XMLTOARRAY_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>
#include <Core/Types/Stack.h>

namespace Meow {
	namespace Core {
		namespace Tools {
			class XMLToArrayElement;
			typedef Reference<XMLToArrayElement> XMLToArrayElement_ptr;
			class XMLToArray;
			typedef Reference<XMLToArray> XMLToArray_ptr;

			class XMLToArrayElement : public Object
			{
			public:
				XMLToArrayElement();
				virtual ~XMLToArrayElement();

				Core::Types::Array_ptr attributes;
				std::string element;
				std::string characterdata;
				ptr storage;
			};

			class XMLToArray : public XMLParser
			{
			public:
				XMLToArray();
				virtual ~XMLToArray();

				void SetArray( Meow::Core::Types::Array_ptr array_ptr );
				virtual void StartElement( string element, Meow::Core::Types::Array_ptr attributes );
				virtual void EndElement( string element );
				virtual void CharacterData( string data );
			private:
				Core::Types::Stack_ptr iArrayStack;
				Core::Types::Stack_ptr iElementStack;
			};
		}
	}
}
#endif

