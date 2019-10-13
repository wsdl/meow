// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_XMLPARSER_H_
#define _CORE_XMLPARSER_H_
#include <Core/Object.h>
#include <Core/Types/Array.h>
#include <Core/Types/Stack.h>

namespace Meow {
	namespace Core {
		namespace Tools {
			class XMLParser;
			typedef Reference<XMLParser> XMLParser_ptr;

			class XMLElement;
			typedef Reference<XMLElement> XMLElement_ptr;

			class XMLElement : public Meow::Core::Object
			{
			public:
				XMLElement();
				virtual ~XMLElement();
				std::string elementName;
				Core::Types::Array_ptr elementAttributes;
				std::string characterData;
			};

			class XMLParser : public Meow::Core::Object
			{
			public:
				XMLParser();
				virtual ~XMLParser();
				bool ParseString( string document );
				bool ParsePartString( string document, bool finished=false );
				int  GetLineNumber( );
				string GetLastError( );
				virtual void StartElement( string element, Core::Types::Array_ptr attributes );
				virtual void EndElement( string element );
				virtual void CharacterData( string data );
				virtual Core::Types::Stack_ptr GetElementStack();

			private:
				bool parsing;
				void * parser;
				Core::Types::Stack_ptr iStack;
			};
		}
	}
} // End of namespace

#endif
