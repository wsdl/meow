/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_USERPARSER_H_
#define _XML_USERPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>

namespace Meow {
	namespace XML {
		class UserParser;
		typedef Reference<UserParser> UserParser_ptr;

		class UserParser : public Core::Tools::XMLParser
		{
		public:
			UserParser();
			virtual ~UserParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			Core::Types::Array_ptr GetUsers();

		private:
			Core::Types::Array_ptr 	   iUsers;
			std::string			   	   iCurrentUserId;
			std::string			   	   iCurrentUserName;
			std::string			   	   iCurrentPassword;
			std::string			   	   iCurrentEmail;
			std::string				   iCurrentObjectReference;
			std::string			       iCurrentElement;
			std::string			       iCurrentCharacterData;
			Core::Types::Array_ptr 	   iCurrentGroups;
		};
	}
}

#endif
