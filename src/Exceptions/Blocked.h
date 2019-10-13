/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _EXCEPTION_BLOCKED_H_
#define _EXCEPTION_BLOCKED_H_
#include <Exception.h>

namespace Meow {
	namespace Exceptions {
		class Blocked : public Meow::Exception
		{
		public:
			Blocked();
			~Blocked();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow
#endif
