/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_DIVIDEBYZERO_H_
#define _EXCEPTION_DIVIDEBYZERO_H_
#include <Exception.h>

namespace Meow {
	namespace Exceptions {
		class DivideByZero : public Meow::Exception
		{
		public:
			DivideByZero();
			~DivideByZero();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
