/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
// Version: 3.1
// Change History:
//	14/09/2016 @DavidSpencer
//		Brought in to support a customer function.
// @requires: libjsoncpp
#ifndef _CORE_JSONPARSER_H_
#define _CORE_JSONPARSER_H_
#include <Core/Object.h>
#include <Core/Types/Array.h>
#include <jsoncpp/json/value.h>

namespace Meow {
	namespace Core {
		namespace Tools {
			class JSONParser;
			typedef Reference<JSONParser> JSONParser_ptr;

			class JSONParser : public Meow::Core::Object
			{
			public:
				JSONParser();
				virtual ~JSONParser();
				bool ParseString( string document );
				virtual void HandleData( Core::Types::ValueType_ptr rootElement );
				std::string GetErrorMessage();
				
			private:
				Core::Types::ValueType_ptr iArray;
				std::string iError;
				// Void * so we can avoid issues where changing the underlying
				// implementation requires a rebuild of everything.
				Core::Types::ValueType_ptr ParseValue( Json::Value & root);
			};
		}
	}
} // End of namespace

#endif
