// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_THREADPOOL_H_
#define _CORE_THREADPOOL_H_
#include <Core/Object.h>
#include <SystemException.h>
#include <string>
#include <Core/Threadable.h>
#include <Core/System/Lock.h>
#include <Core/Types/Array.h>
#include <Core/Types/Queue.h>

namespace Meow {
	namespace Core {
		namespace System {
			class QueueThreadEntry : public Object
			{
			public:
				QueueThreadEntry();
				~QueueThreadEntry();
				void           SetFunction( Threadable_ptr object );
				Threadable_ptr GetFunction( );
				void           SetCompletionStack( Core::Types::Stack_ptr stack );
				void           Process( );
				void           PleaseStop( );
				void           Respite();
				bool           GetReadyToStop( );
				void            QueueThis( ptr object );
				void           Signal( );
				void           WaitForSignal(  );
			private:
				Meow::Core::Types::Queue_ptr WorkQueue;
				Lock_ptr WorkQueueLock;
				Threadable_ptr Function;
				Meow::Core::Types::Stack_ptr Completion;
				bool RequestStop;
				void * iSignal;
				void * iMutex;
			};
			typedef Reference<QueueThreadEntry> QueueThreadEntry_ptr;

			class ThreadPool : public Threadable
			{
			public:
				ThreadPool();
				~ThreadPool();
				void            SetFunction( Threadable_ptr object );
				void            QueueThis( ptr object );
				void            Stop( );
				void            SetMaximum( unsigned long max );
				unsigned long   GetMaximum( );
				virtual void    Run( );
			private:
				Meow::Core::Types::Queue_ptr Queue;
				Threadable_ptr Function;
				unsigned long Maximum;
				unsigned long Active;
				Meow::Core::Types::Stack_ptr ReadyThreads;
				Lock_ptr Sync;
				bool pleasestop;
			};
			typedef Reference<ThreadPool> ThreadPool_ptr;
		}
	}
} // End of namespace

#endif
