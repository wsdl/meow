/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_WIPED_H_
#define _EXCEPTION_WIPED_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class Wiped : public Meow::Exception
		{
		public:
			Wiped();
			~Wiped();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
