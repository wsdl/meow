/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_ROLLBACK_H_
#define _EXCEPTION_ROLLBACK_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class Rollback : public Meow::Exception
		{
		public:
			Rollback();
			~Rollback();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow
#endif
