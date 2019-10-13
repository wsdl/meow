/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _XML_OBJECTPARSER_H_
#define _XML_OBJECTPARSER_H_
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Array.h>
#include <Object.h>

namespace Meow {
	namespace XML {
		class ObjectParser;
		typedef Reference<ObjectParser> ObjectParser_ptr;

		class ObjectAttribute;
		typedef Reference<ObjectAttribute> ObjectAttribute_ptr;

		class ObjectRelationship;
		typedef Reference<ObjectRelationship> ObjectRelationship_ptr;

		class ObjectBond;
		typedef Reference<ObjectBond> ObjectBond_ptr;

		class ObjectAttribute : public Object
		{
		public:
			ObjectAttribute();
			~ObjectAttribute();

			std::string iType;
			bool		iIsKey;
			std::string iName;
			std::string iDefaultValue;
		};

		class ObjectRelationship : public Object
		{
		public:
			ObjectRelationship();
			~ObjectRelationship();
			std::string iOtherClass;
			Core::Types::Array_ptr iBindings;
		};


		class ObjectParser : public Core::Tools::XMLParser
		{
		public:
			ObjectParser();
			virtual ~ObjectParser();
			virtual void StartElement( string element, Core::Types::Array_ptr attributes );
			virtual void EndElement( string element );
			virtual void CharacterData( string data );

			std::string  GetDefinitionClassName();
			Core::Types::Array_ptr GetAttributes();
			Core::Types::Array_ptr GetRelationships();

		private:
			Core::Types::Array_ptr 	   iAttributes;
			Core::Types::Array_ptr	   iRelationships;
			std::string				   iDefinitionClassName;
		};

		class ObjectBond : public Object
		{
		public:
			ObjectBond();
			~ObjectBond();

			typedef enum { attributebind=0, theirvaluebind=1, myvaluebind=2 } bindingType;

			bindingType iBindingType;
			std::string iMyAttribute;
			std::string iTheirAttribute;
			std::string iOperation;
		};

	}
}

#endif
