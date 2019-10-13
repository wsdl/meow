/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_PAGEPARSER_H_
#define _XML_PAGEPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>
#include <Core/Types/Stack.h>
#include <Object.h>

namespace Meow {
	namespace XML {
		class PageParser;
		typedef Reference<PageParser> PageParser_ptr;
		class PageParameter;
		typedef Reference<PageParameter> PageParameter_ptr;
		class PageEvent;
		typedef Reference<PageEvent> PageEvent_ptr;
		class PageDiv;
		typedef Reference<PageDiv> PageDiv_ptr;
		class PageContent;
		typedef Reference<PageContent> PageContent_ptr;
		
		class PageContent : public Object
		{
		public:
			PageContent();
			~PageContent();
			std::string iContent;
			Core::Types::Array_ptr iSecurity;
			
		};

		class PageParameter : public Object
		{
		public:
			PageParameter();
			~PageParameter();
			std::string iType;
			std::string iRegularExpression;
			std::string iName;
			std::string iDefaultValue;
		};

		class PageEvent : public Object
		{
		public:
			PageEvent();
			~PageEvent();

			std::string eventName;
			Core::Types::Array_ptr iParameters;
		};

		class PageDiv : public Object
		{
		public:
			PageDiv();
			~PageDiv();
			std::string iComponent;
			std::string iKey;
			std::string iMethod;
			std::string iParameters;
			Core::Types::Array_ptr 		iSecurity;
			Core::Types::ValueType_ptr iRefresh;
			bool iShowBusy;
			std::string iHandler;
            std::string iLeadTime;
		};

		class PageParser : public Core::Tools::XMLParser
		{
		public:
			PageParser();
			virtual ~PageParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			Core::Types::Array_ptr GetResources();
			Core::Types::Array_ptr GetStyles();
			Core::Types::Array_ptr GetPrintStyles();
			Core::Types::Array_ptr GetHeaderLines();
			Core::Types::Array_ptr GetGroups();
			Core::Types::Array_ptr GetParameters();
			std::string			   GetTitle();
			Core::Types::Array_ptr GetOnLoadEvents();
			Core::Types::Array_ptr GetLoadedEvents();
			bool                   GetHTTPS();
			bool                   GetHTTP();
//			std::string            GetContent();
			Core::Types::Array_ptr GetContent();
			Core::Types::Array_ptr GetDivs();
			Core::Types::Array_ptr GetMetas();

		private:
			Core::Types::Array_ptr 		iResources;
			Core::Types::Array_ptr 		iStyles;
			Core::Types::Array_ptr		iPrintStyles;
			Core::Types::Array_ptr 		iHeaderLines;
			Core::Types::Array_ptr		iGroups;
			Core::Types::Array_ptr		iParameters;
			Core::Types::Array_ptr		iOnLoadEvents;
			Core::Types::Array_ptr		iLoadedEvents;
			Core::Types::Array_ptr		iMetas;
			bool				iHTTPS;
			bool				iHTTP;
//			std::string 			iContent;
			Core::Types::Array_ptr 		iContent;
			Core::Types::Array_ptr 		iDivs;
			std::string 			iTitle;
			std::string			iCurrentElement;
			std::string			iCurrentCharacterData;
			Core::Types::Stack_ptr 		iElementStack;
		};
	}
}

#endif
