/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_WIPING_H_
#define _EXCEPTION_WIPING_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class Wiping : public Meow::Exception
		{
		public:
			Wiping();
			~Wiping();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
