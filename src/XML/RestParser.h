/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_RESTPARSER_H_
#define _XML_RESTPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>

namespace Meow {
	namespace XML {
		class RestParser;
		typedef Reference<RestParser> RestParser_ptr;

		class RestParser : public Core::Tools::XMLParser
		{
		public:
			RestParser();
			virtual ~RestParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			bool GetEnabled();

		private:
			std::string			       iCurrentElement;
			std::string			       iCurrentCharacterData;
			bool 					   iEnabled;
		};
	}
}

#endif
