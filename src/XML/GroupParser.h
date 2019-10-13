/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_GROUPPARSER_H_
#define _XML_GROUPPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>

namespace Meow {
	namespace XML {
		class GroupParser;
		typedef Reference<GroupParser> GroupParser_ptr;

		class GroupParser : public Core::Tools::XMLParser
		{
		public:
			GroupParser();
			virtual ~GroupParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			Core::Types::Array_ptr GetGroups();

		private:
			Core::Types::Array_ptr 	   iGroups;
			std::string			       iCurrentElement;
			std::string			       iCurrentCharacterData;
		};
	}
}

#endif
