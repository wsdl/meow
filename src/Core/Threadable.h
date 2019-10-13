// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_THREADABLE_H_
#define _CORE_THREADABLE_H_
#include <Core/Object.h>
#include <string>

namespace Meow {
	namespace Core {
		class Threadable : public Object
		{
		public:
			Threadable();
			virtual ~Threadable();

			void SetMemory( ptr object );
			ptr  GetMemory( );

			virtual void Run( );

			void Respite( );
			void Signal( );
			void WaitForSignal(  );

		private:
			ptr Memory;
			void * iSignal;
			void * iMutex;
		};
		typedef Reference<Threadable> Threadable_ptr;
	}
} // End of namespace

#endif
