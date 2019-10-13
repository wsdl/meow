// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/System/ThreadPool.h>
#include <iostream>
#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#ifdef WIN32
DWORD WINAPI TPExecuteThread( void * data )
#else
void * TPExecuteThread( void * data )
#endif
{
    Meow::Core::System::QueueThreadEntry_ptr object=(Meow::Core::System::QueueThreadEntry*)data;
    while ( !object->GetReadyToStop() )
    {
        object->Process( );
        object->WaitForSignal( );
    }
#ifndef WIN32
    pthread_exit(NULL);
#endif
}

void Meow::Core::System::QueueThreadEntry::QueueThis( ptr object )
{
    WorkQueueLock->Gain();
    WorkQueue->Push(object);
    WorkQueueLock->Release();
    Signal( );
}

void Meow::Core::System::QueueThreadEntry::Process( )
{
    bool HasWorkToDo=false;
    WorkQueueLock->Gain();
    HasWorkToDo=!WorkQueue->IsEmpty();
    WorkQueueLock->Release();

    if ( HasWorkToDo )
    {
        while ( HasWorkToDo )
        {
            WorkQueueLock->Gain();
            ptr Memory=WorkQueue->Pull();
            WorkQueueLock->Release();
            Function->SetMemory(Memory);
            Function->Run();
            WorkQueueLock->Gain();
            HasWorkToDo=!WorkQueue->IsEmpty();
            WorkQueueLock->Release();
        }
        ptr self=this;
        Completion->Push(self);
    }
}

Meow::Core::System::QueueThreadEntry::QueueThreadEntry( ) :
    RequestStop(false)
{
    WorkQueue=new Meow::Core::Types::Queue();
    WorkQueueLock=new Lock();
#ifdef __USE_PTHREADS
    iSignal=(void*)new pthread_cond_t;
    iMutex=(void*)new pthread_mutex_t;

    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_cond_init(__signal,NULL);

    pthread_mutex_t * __mutex=(pthread_mutex_t*)iMutex;
    pthread_mutex_init(__mutex,NULL);
#else
#endif
}

Meow::Core::System::QueueThreadEntry::~QueueThreadEntry( )
{
#ifdef __USE_PTHREADS
    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_mutex_t * __mutex=(pthread_mutex_t*)iMutex;

    pthread_mutex_unlock(__mutex);
    pthread_cond_destroy(__signal);
    delete __mutex;
    delete __signal;
#else
#endif
}


void Meow::Core::System::QueueThreadEntry::Signal( )
{
#ifdef __USE_PTHREADS
    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_cond_signal(__signal);
#endif
}

void Meow::Core::System::QueueThreadEntry::WaitForSignal(  )
{
#ifdef __USE_PTHREADS
    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_mutex_t * __mutex=(pthread_mutex_t*)iMutex;
    pthread_mutex_lock(__mutex);
    pthread_cond_wait(__signal,__mutex);
    pthread_mutex_unlock(__mutex);
#endif
}

void Meow::Core::System::QueueThreadEntry::SetFunction( Meow::Core::Threadable_ptr object )
{
    Function=object;
}

Meow::Core::Threadable_ptr Meow::Core::System::QueueThreadEntry::GetFunction( )
{
    return Function;
}

void Meow::Core::System::QueueThreadEntry::SetCompletionStack( Meow::Core::Types::Stack_ptr stack )
{
    Completion=stack;
}

void Meow::Core::System::QueueThreadEntry::PleaseStop( )
{
    RequestStop=true;
}

bool Meow::Core::System::QueueThreadEntry::GetReadyToStop( )
{
    return RequestStop;
}

void Meow::Core::System::QueueThreadEntry::Respite( )
{
#ifndef WIN32
    usleep(100);
#else
    Sleep(100);
#endif
}



Meow::Core::System::ThreadPool::ThreadPool( ) :
    Maximum(10),
    Active(0),
    pleasestop(false)
{
    SetMeowClassName("ThreadPool");
    Queue=new Core::Types::Queue();
    ReadyThreads=new Core::Types::Stack();
    Sync=new Lock();
}

Meow::Core::System::ThreadPool::~ThreadPool( )
{
}

void Meow::Core::System::ThreadPool::QueueThis( ptr object )
{
    Sync->Gain();
    Queue->Push(object);
    Sync->Release();
    Signal();
}

void Meow::Core::System::ThreadPool::Stop( )
{
    Queue->Clear();
    Maximum=0;
}

void Meow::Core::System::ThreadPool::SetMaximum( unsigned long max )
{
    Maximum=max;
}

unsigned long Meow::Core::System::ThreadPool::GetMaximum( )
{
    return Maximum;
}

void Meow::Core::System::ThreadPool::Run( )
{
    while ( Maximum>0 )
    {
        Sync->Gain();
        while ( ! ReadyThreads->IsEmpty() &&
                Active>Maximum )
        {
            QueueThreadEntry_ptr thread=(QueueThreadEntry*)ReadyThreads->Pop().Cast();
            thread->PleaseStop();
            Active--;
        }
        // Are there any queue entries?
        while ( ! Queue->IsEmpty() &&
                ( Maximum-Active>0 || !ReadyThreads->IsEmpty() ) )
        {

            ptr Memory=Queue->Pull();
            if ( !ReadyThreads->IsEmpty() )
            {
                // Reuse an existing one
                QueueThreadEntry_ptr thread=(QueueThreadEntry*)ReadyThreads->Pop().Cast();
                thread->QueueThis(Memory);
            }
            else
            {
                QueueThreadEntry_ptr thread=new QueueThreadEntry();
                thread->QueueThis(Memory);
                thread->SetFunction(Function);
                thread->SetCompletionStack(ReadyThreads);
#ifndef WIN32
                pthread_t tid;
                int rc;
                rc=pthread_create(&tid,NULL,TPExecuteThread,(void*)thread.getPointerForceIncref());
                if ( rc==0 )
                {
                    pthread_detach(tid);
                    Active++;
                }
#else
                HANDLE    threadh;
                DWORD     tid;
                threadh=CreateThread(NULL,65535,TPExecuteThread,(LPVOID)thread.getPointerForceIncref(),0,&tid);
                if ( threadh )
                {
                    Active++;
                }
#endif
            }
        }
        Sync->Release();
        WaitForSignal( );
    }

}
void Meow::Core::System::ThreadPool::SetFunction( Meow::Core::Threadable_ptr fn )
{
    Function=fn;
}
