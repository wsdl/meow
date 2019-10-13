/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_TERMINATE_H_
#define _EXCEPTION_TERMINATE_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class Terminate : public Meow::Exception
		{
		public:
			Terminate();
			~Terminate();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
