/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_DUPLICATE_H_
#define _EXCEPTION_DUPLICATE_H_
#include <Exception.h>

namespace Meow {
	namespace Exceptions {
		class Duplicate : public Meow::Exception
		{
		public:
			Duplicate();
			~Duplicate();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
