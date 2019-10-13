/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_VERSIONPARSER_H_
#define _XML_VERSIONPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>

/* <versions>
	<component name="abcdef/ghi" currentversion="" installed="" />
	<component name="abcdef/ghi" currentversion="" installed="" />
	<component name="abcdef/ghi" currentversion="" installed="" />
	<component name="abcdef/ghi" currentversion="" installed="" />
</versions> */

namespace Meow {
	namespace XML {
		class VersionParser;
		typedef Reference<VersionParser> VersionParser_ptr;

		class Version;
		class Version : public Core::Object
		{
		public:	
			Version();
			~Version();

			int currentVersion;
			int installed;
		};
		typedef Reference<Version> Version_ptr;

		class VersionParser : public Core::Tools::XMLParser
		{
		public:
			VersionParser();
			virtual ~VersionParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			void UpdateComponent( std::string key, int version );
			int  GetVersion( std::string key );
			Core::Types::Array_ptr GetComponents();
			std::string ToString();
		private:
			Core::Types::Array_ptr iComponents;
		};
	}
}

#endif
