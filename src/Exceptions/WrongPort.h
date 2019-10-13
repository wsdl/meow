/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_WRONGPORT_H_
#define _EXCEPTION_WRONGPORT_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class WrongPort : public Meow::Exception
		{
		public:
			WrongPort();
			~WrongPort();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
