// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_CONNECTIONMANAGER_H_
#define _CORE_CONNECTIONMANAGER_H_
#include <Core/Object.h>
#include <Core/IO/Client.h>
#include <Core/Types/Array.h>
#include <Core/Threadable.h>
#include <Core/System/Thread.h>
#include <Core/System/ThreadPool.h>
#include <SystemException.h>
#include <string>

namespace Meow
{
	namespace Core
	{
		namespace IO
		{
			typedef enum {unconfigured,ready,listening,running,stopped} ServiceStatus;

			class ServiceThread : public Threadable
			{
			public:
				ServiceThread();
				virtual ~ServiceThread();

				void SetMaxListeners( short queuelength );
				void SetServicePort( long portno );
				long GetPort( );
				virtual void Run();
				void SetCallback( Threadable_ptr serverprogram );
				void Stop( );
				void Initialise( ) ;
			private:
				ServiceStatus status;
				Threadable_ptr program;
				long port;
				long sock;
				short queuelength;
				Core::System::ThreadPool_ptr threads;
				Core::System::Thread_ptr runner;
			};

			typedef Reference<ServiceThread> ServiceThread_ptr;

			class ConnectionManager : public Core::Object
			{
			public:
				ConnectionManager();
				virtual ~ConnectionManager();

				void SetConnectionPoolMax( unsigned long max );
				unsigned long GetConnectionPoolMax( );

				Client_ptr AccessService( std::string hostname, long portno );
				void BindServiceAccess( long portno, Threadable_ptr serverprogram );
				void UnbindServiceAccess( long portno );
				void         WaitTillFinished( );
				void         Wait( );
			private:
				Meow::Core::Types::Array_ptr Services;
				Meow::Core::Types::Array_ptr ClientPool;
				unsigned long ConnectionPoolMax;
			};
			typedef Reference<ConnectionManager> ConnectionManager_ptr;

			class TrackingObject : public Core::Object
			{
			public:
				TrackingObject();
				~TrackingObject();
				unsigned long ConnectionsOpened;
				Meow::Core::Types::Stack_ptr ConnectionsWaiting;
				Meow::Core::System::Lock_ptr TheLock;
			};
			typedef Reference<TrackingObject> TrackingObject_ptr;
		}
	}
} // End of namespace

#endif
