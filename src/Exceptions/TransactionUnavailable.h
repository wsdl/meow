/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _EXCEPTION_TRANSACTIONUNAVAILABLE_H_
#define _EXCEPTION_TRANSACTIONUNAVAILABLE_H_
#include <Exception.h>
namespace Meow
{
	namespace Exceptions
	{
		class TransactionUnavailable : public Meow::Exception
		{
		public:
			TransactionUnavailable();
			~TransactionUnavailable();
		};
	} // End of NameSpace Exception
} // End of NameSpace Meow

#endif
