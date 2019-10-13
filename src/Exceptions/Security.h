/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_SECURITY_H_
#define _EXCEPTION_SECURITY_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class Security : public Meow::Exception
		{
		public:
			Security();
			~Security();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow
#endif
