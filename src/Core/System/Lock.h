// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_LOCK_H_
#define _CORE_LOCK_H_
#include <Core/Object.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace System {
			class Lock : public Object
			{
			public:
				Lock();
				~Lock();

				bool Try( );
				void Gain( );
				void Release( );
				void Respite( );
			private:
				void LazyInit( );
				void * __handle;
			};
			typedef Reference<Lock> Lock_ptr;
		}
	}
} // End of namespace

#endif
