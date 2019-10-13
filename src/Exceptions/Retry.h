/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_RETRY_H_
#define _EXCEPTION_RETRY_H_
#include <Exception.h>
namespace Meow
{
	namespace Exceptions
	{
		class Retry : public Meow::Exception
		{
		public:
			Retry();
			~Retry();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow
#endif
