// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/IO/ConnectionManager.h>
#include <errno.h>
#ifdef WIN32
	#include <windows.h>
	#include <winsock.h>
#else
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <unistd.h>
	#include <string.h>
#endif
#include <iostream>
#include <stdio.h>
#include <Core/System/Thread.h>

Meow::Core::IO::ServiceThread::ServiceThread() :
    status(unconfigured),
    port(0),
    sock(-1),
    queuelength(5)
{
    threads=new Core::System::ThreadPool();
    runner=new Core::System::Thread();
    runner->SetFunction((Threadable*)threads);
    runner->Detach();
    SetMeowClassName("ServiceThread");
}

Meow::Core::IO::ServiceThread::~ServiceThread()
{

}

void Meow::Core::IO::ServiceThread::SetMaxListeners( short queuelength_ )
{
    queuelength=queuelength_;
}

void Meow::Core::IO::ServiceThread::SetServicePort( long portno )
{
    port=portno;
}

long Meow::Core::IO::ServiceThread::GetPort( )
{
    return port;
}

void Meow::Core::IO::ServiceThread::Stop( )
{
    if ( status==ready ||
            status==listening ||
            status==running )
    {
        // This will remove the bind, no further connections will be accepted
        // and really we should notify all threads in the thread pool they should
        // self close as soon as possible.
#ifdef WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        status=stopped;
        threads->Stop();
    }
}

void Meow::Core::IO::ServiceThread::Initialise( )
{
    if ( status==unconfigured )
    {
        if ( port>0 )
        {
            struct sockaddr_in address;
            struct in_addr     inaddr;
            int                addresslength;

            addresslength=sizeof(struct sockaddr_in);
            if ( ( sock=socket(AF_INET,SOCK_STREAM,0)) < 0 )
            {
                SystemException exception;
                exception.SetLineNumber(45);
                exception.SetInitiator(this);
                exception.SetMessage(strerror(errno));
                throw exception;
            }

            address.sin_family=AF_INET;
            address.sin_addr.s_addr=htonl(INADDR_ANY);
            address.sin_port=htons(port);
            memset(&address.sin_zero,'\0',sizeof(address.sin_zero));

            if ( bind(sock,(struct sockaddr*)&address,sizeof(address)) )
            {

                SystemException exception;
                exception.SetLineNumber(45);
                exception.SetInitiator(this);
                exception.SetMessage(strerror(errno));
                throw exception;
            }

            if ( listen(sock,queuelength) )
            {

                SystemException exception;
                exception.SetLineNumber(45);
                exception.SetInitiator(this);
                exception.SetMessage(strerror(errno));
                throw exception;
            }

            threads->SetFunction(program);
            status=ready;
        }
    }
}

void Meow::Core::IO::ServiceThread::Run( )
{
    if ( status==unconfigured )
        Initialise();
    while(status==ready)
    {
        status=listening;
        int                workersocket;
        struct sockaddr_in address;
        int                addresslength;

        addresslength=sizeof(address);
#ifdef WIN32
        if ( (workersocket=accept(sock,(struct sockaddr*)&address,&addresslength))< 0 )
#else
        if ( (workersocket=accept(sock,(struct sockaddr*)&address,(socklen_t*)&addresslength))< 0 )
#endif
        {
            status=stopped;
        }
        else
        {
            status=running;
            Client_ptr client=new Client();
            client->sock=workersocket;
            client->status=open;

            threads->QueueThis((ptr)client);
        }
        status=ready;
    }
    // cout << "Gone" << endl;
    status=stopped;
}

void Meow::Core::IO::ServiceThread::SetCallback( Meow::Core::Threadable_ptr thread )
{
    program=thread;
}

Meow::Core::IO::ConnectionManager::ConnectionManager() :
    ConnectionPoolMax(10)
{
    Services=new Core::Types::Array();
    ClientPool=new Core::Types::Array();
}

Meow::Core::IO::ConnectionManager::~ConnectionManager()
{

}

void Meow::Core::IO::ConnectionManager::SetConnectionPoolMax( unsigned long max )
{
    ConnectionPoolMax=max;
}

unsigned long Meow::Core::IO::ConnectionManager::GetConnectionPoolMax( )
{
    return ConnectionPoolMax;
}


Meow::Core::IO::Client_ptr Meow::Core::IO::ConnectionManager::AccessService(
    std::string hostname,
    long portno )
{
    std::string url;
    Client_ptr client;
    char       pszBuf[256];

    url=hostname;
    url.append(":");
    sprintf(pszBuf,"%d",(int)portno);
    url.append(pszBuf);

    if ( ! ClientPool->IsSet(url) )
    {
        // OK this is our first connection
        TrackingObject_ptr Tracking=new TrackingObject();
        Tracking->ConnectionsOpened=1;
        Tracking->ConnectionsWaiting=new Core::Types::Stack();
        Tracking->TheLock=new Core::System::Lock();

        ClientPool->Set(url,(ptr)Tracking);
        client=new Client();
        client->SetOnComplete(Tracking->ConnectionsWaiting,Tracking->TheLock);
        client->SetHostName(hostname);
        client->SetPortNo(portno);
    }
    else
    {
        TrackingObject_ptr Tracking=(TrackingObject*)ClientPool->Get(url).Cast();
        Tracking->TheLock->Gain();
        if ( Tracking->ConnectionsWaiting->IsEmpty() )
        {
            if( Tracking->ConnectionsOpened<GetConnectionPoolMax() )
            {
                Tracking->TheLock->Release();
                Tracking->ConnectionsOpened++;
                client=new Client();
                client->SetOnComplete(Tracking->ConnectionsWaiting,Tracking->TheLock);
                client->SetHostName(hostname);
                client->SetPortNo(portno);
            }
            else
            {
                // Wait for a new client to become available
                bool GoAhead=false;
                Tracking->TheLock->Release();
                while ( !GoAhead )
                {
                    Wait();
                    Tracking->TheLock->Gain();
                    if ( ! Tracking->ConnectionsWaiting->IsEmpty() )
                    {
                        client=(Client*)Tracking->ConnectionsWaiting->Pop().Cast();
                        GoAhead=true;
                    }
                    else
                    {
                        Tracking->TheLock->Release();
                    }
                }
            }
        }
        else
        {
            client=(Client*)Tracking->ConnectionsWaiting->Pop().Cast();
            Tracking->TheLock->Release();
        }
    }
    return client;
}

void Meow::Core::IO::ConnectionManager::BindServiceAccess(
    long portno,
    Threadable_ptr serverprogram )
{
    if ( Services->IsSet(portno) )
    {
        throw new SystemException();
    }
    ServiceThread_ptr thread=new ServiceThread();
    thread->SetServicePort(portno);
    thread->SetCallback(serverprogram);
    thread->Initialise();

    Core::System::Thread_ptr runner=new Core::System::Thread();
    runner->SetFunction((Threadable*)thread.Cast());
    try
    {
        runner->Detach();
    } catch ( ... )
    {

    }

    // If there is a system exception thrown it will have been thrown by
    // initialise.
    Services->Set(portno,(ptr)thread);
}

void Meow::Core::IO::ConnectionManager::UnbindServiceAccess( long portno )
{
    if ( !Services->IsSet(portno) )
    {
        throw new SystemException();
    }
    ServiceThread_ptr thread=(ServiceThread*)Services->Get(portno).Cast();
    thread->Stop();
    Services->Unset(portno);
}

void Meow::Core::IO::ConnectionManager::Wait( )
{
#ifdef WIN32
    Sleep(1000);
#else
    usleep(1000);
#endif
}

void Meow::Core::IO::ConnectionManager::WaitTillFinished( )
{
    while ( true )
        Wait();
    
}

Meow::Core::IO::TrackingObject::TrackingObject()
{
	SetMeowClassName("Core/IO/TrackingObject");
	ConnectionsOpened=0;
	ConnectionsWaiting=new Core::Types::Stack();
	TheLock=new Core::System::Lock();
}
Meow::Core::IO::TrackingObject::~TrackingObject()
{
}

