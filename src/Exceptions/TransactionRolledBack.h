/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_TRANSACTIONROLLEDBACK_H_
#define _EXCEPTION_TRANSACTIONROLLEDBACK_H_
#include <Exception.h>

namespace Meow
{
	namespace Exceptions
	{
		class TransactionRolledBack : public Meow::Exception
		{
		public:
			TransactionRolledBack();
			~TransactionRolledBack();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow
#endif
