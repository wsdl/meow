/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_DOMAINPARSER_H_
#define _XML_DOMAINPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>

namespace Meow {
	namespace XML {
		class DomainParser;
		typedef Reference<DomainParser> DomainParser_ptr;

		class DomainParser : public Core::Tools::XMLParser
		{
		public:
			DomainParser();
			virtual ~DomainParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			Core::Types::Array_ptr GetBanned();
			Core::Types::Array_ptr GetPermitted();
			std::string			   GetConfigurationFile();
			bool				   GetAvailable();

		private:
			Core::Types::Array_ptr iBanned;
			Core::Types::Array_ptr iPermitted;
			std::string		       iConfigurationFile;
			bool                   iAvailable;
			std::string			   iCurrentElement;
			std::string			   iCurrentCharacterData;
		};
	}
}

#endif
