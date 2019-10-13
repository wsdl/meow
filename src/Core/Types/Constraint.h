// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_CONSTRAINT_H_
#define _CORE_CONSTRAINT_H_
#include <Core/Types/ValueType.h>

// Define our builtin expressions
#define CONSTRAINT_STRING       		"^([®©~a-zA-Z0-9!@#$%^&*()-_=+;:'\\\"|~`<>?/{} ])*$"
#define CONSTRAINT_FILENAME       		"^([a-zA-Z0-9.-_])*$"
#define CONSTRAINT_TEXT					"^([®©a-zA-Z0-9!@#$%^&*()-_=+;:\n'\\\"|~`<>?/{} ])*$" 
#define CONSTRAINT_INTEGER      		"^(\\+|-)?\\d+$"
#define CONSTRAINT_FLOAT        		"^[-+]?\\d*\\.?\\d*$"
#define CONSTRAINT_DATE         		"^([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2})$"
#define CONSTRAINT_DATETIME     		"^([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2}) (?:(?:([01]?\\d|2[0-3]):)?([0-5]?\\d):)?([0-5]?\\d)$"
#define CONSTRAINT_TIME         		"^([0-1][0-9]|[2][0-3]):([0-5][0-9])$" "^([0-1][0-9]|[2][0-3]):([0-5][0-9])$"
#define CONSTRAINT_BOOLEAN      		"^(true|false|TRUE|FALSE|True|False|T|F|0|1)$"
#define CONSTRAINT_EMAIL        		"^([a-zA-Z0-9_\\-\\.]+)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([a-zA-Z0-9\\-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)$"
#define CONSTRAINT_URL          		"^(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?"
#define CONSTRAINT_PHONE        		"^(\\+[1-9][0-9]*(\\([0-9]*\\)|-[0-9]*-))?[0]?[1-9][0-9\\- ]*$"
#define CONSTRAINT_POSTCODE     		"^[a-zA-Z]{1,2}[0-9][0-9A-Za-z]{0,1} {0,1}[0-9][A-Za-z]{2}$"
#define CONSTRAINT_OCODEURL     		"^[^/]:[^/]+$"
#define CONSTRAINT_OCODEKEY     		"^[^/]+$"
#define CONSTRAINT_NULLABLESTRING       "^([®©a-zA-Z0-9!@#$%^&*()-_=+;:'\\\"|~`<>?/{} ])*$"
#define CONSTRAINT_NULLABLEINTEGER      "^(\\+|-)?\\d*$"
#define CONSTRAINT_NULLABLEFLOAT        "^[-+]?\\d*\\.?\\d*$"
#define CONSTRAINT_NULLABLEBOOLEAN      "^(true|false|TRUE|FALSE|True|False|T|F|0|1)*$"
#define CONSTRAINT_NULLABLEEMAIL        "^([a-zA-Z0-9_\\-\\.]+)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([a-zA-Z0-9\\-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)*$"
#define CONSTRAINT_PASSWORD				"^([a-zA-Z0-9!@#$%^&*()-_=+;:'\\\"|~`<>?/{} ]{8,})*$"
#define CONSTRAINT_NULLABLEURL			"(^$|^(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#]))"
#define CONSTRAINT_NULLABLEDATE         "(^$|^([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2})$)"
#define CONSTRAINT_NULLABLEDATETIME  	"(^$|^([0]?[1-9]|[1|2][0-9]|[3][0|1])[./-]([0]?[1-9]|[1][0-2])[./-]([0-9]{4}|[0-9]{2}) (?:(?:([01]?\\d|2[0-3]):)?([0-5]?\\d):)?([0-5]?\\d)$)"

namespace Meow {
	namespace Core {
		namespace Types {
			class Constraint;
			typedef Reference<Constraint> Constraint_ptr;

			class Constraint : public Object
			{
			public:
				Constraint( std::string regexp );
				Constraint();
				virtual ~Constraint();

				bool Test( Meow::Core::Types::ValueType_ptr other );
				std::string GetExpression();
				void SetExpression( std::string regexp );
			private:
				std::string iOriginal;
				void *      iExpression;
			};
		}
	}
}

#endif
