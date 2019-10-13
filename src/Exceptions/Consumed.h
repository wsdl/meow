/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _EXCEPTION_CONSUMED_H_
#define _EXCEPTION_CONSUMED_H_
#include <Exception.h>

namespace Meow {
	namespace Exceptions {
		class Consumed : public Meow::Exception
		{
		public:
			Consumed();
			~Consumed();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
