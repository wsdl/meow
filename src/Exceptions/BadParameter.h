/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _EXCEPTION_BADPARAMETER_H_
#define _EXCEPTION_BADPARAMETER_H_
#include <Exception.h>

namespace Meow {
	namespace Exceptions {
		class BadParameter : public Meow::Exception
		{
		public:
			BadParameter();
			~BadParameter();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
