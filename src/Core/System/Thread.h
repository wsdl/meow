// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_THREAD_H_
#define _CORE_THREAD_H_
#include <Core/Object.h>
#include <SystemException.h>
#include <string>
#include <Core/Threadable.h>

namespace Meow {
	namespace Core {
		namespace System {
			class Thread : public Object
			{
			public:
				Thread();
				~Thread();
				static void SetContext( Object_ptr object );
				static Object_ptr GetContext( );
				void SetMemory( ptr object );
				void SetFunction( Threadable_ptr object );
				void Detach( );
			private:
				ptr Memory;
				Threadable_ptr Function;
				void *          handle;
				static void *        thread_key;
				static unsigned long sharers;
			};
			typedef Reference<Thread> Thread_ptr;
		}
	}
} // End of namespace

#endif
