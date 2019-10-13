/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_SERVICEPARSER_H_
#define _XML_SERVICEPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>
#include <Object.h>
namespace Meow {
	namespace XML {

		class ServiceRecord;
		typedef Reference<ServiceRecord> ServiceRecord_ptr;

		class ServiceRecord : public Core::Object
		{
		public:
			ServiceRecord();
			~ServiceRecord();

			std::string iClassName;
			std::string iServiceName;
			Core::Types::Array_ptr iGroups;
			Core::Types::Array_ptr iMethods;
			bool isFactory;
		};


		class ServiceParser;
		typedef Reference<ServiceParser> ServiceParser_ptr;

		class ServiceParser : public Core::Tools::XMLParser
		{
		public:
			ServiceParser();
			virtual ~ServiceParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			Core::Types::Array_ptr GetServices();

		private:
			Core::Types::Array_ptr 	   iServices;
			ServiceRecord_ptr		   iCurrentServiceRecord;
			std::string				   iCurrentMethod;

			std::string			       iCurrentElement;
			std::string			       iCurrentCharacterData;
		};
	}
}

#endif
