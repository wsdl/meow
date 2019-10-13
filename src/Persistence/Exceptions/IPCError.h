/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _MEOW_EXCEPTION_PERSISTENCE_IPCERROR_H_
#define _MEOW_EXCEPTION_PERSISTENCE_IPCERROR_H_
#include <Exception.h>
namespace Meow
{
	namespace Persistence
	{
		namespace Exceptions
		{
			class IPCError : public Meow::Exception
			{
			public:
				IPCError();
				~IPCError();
			};
		}
	}
}
#endif

