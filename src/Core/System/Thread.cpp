// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/System/Thread.h>
#ifdef WIN32
	#include <windows.h>
#else
	#define __USE_PTHREADS 1
	#include <pthread.h>

#endif
#include <iostream>

struct thread_args
{
    void * function;
};
#ifdef WIN32
	DWORD WINAPI ExecuteThread( void * data )
#else
	void * ExecuteThread( void * data )
#endif
{
    struct thread_args * data_casted=(struct thread_args*)data;
    Meow::Core::Threadable_ptr object=(Meow::Core::Threadable*)data_casted->function;
    object->Run();
#ifdef __USE_PTHREADS
    pthread_exit(NULL);
#endif
    free(data_casted);
}

Meow::Core::System::Thread::Thread( ) : handle(NULL)
{
    SetMeowClassName("Thread");
}

Meow::Core::System::Thread::~Thread( )
{
#ifdef __USE_PTHREADS
    pthread_join((pthread_t)handle,NULL);
    sharers--;
    if ( sharers==0 )
    {
        pthread_key_delete(*((pthread_key_t*)thread_key));
        free(thread_key);
    }
#endif
#ifdef WIN32
    sharers--;
    if ( sharers==0 )
        TlsFree((DWORD)thread_key);
#endif
}

void Meow::Core::System::Thread::SetContext( Object_ptr ctx )
{
    if ( thread_key==NULL )
    {
#ifdef __USE_PTHREADS
        thread_key=malloc(sizeof(pthread_key_t));
        pthread_key_t *thread_key_converted=(pthread_key_t*)thread_key;
        pthread_key_create(thread_key_converted,NULL);
        sharers++;
#endif
#ifdef WIN32
        thread_key=(void*)TlsAlloc();
        sharers++;
#endif
    }
#ifdef __USE_PTHREADS
    ptr ptr_thr=pthread_getspecific(*(pthread_key_t*)thread_key);
    if ( ptr_thr!=NULL )
    {
        Object * ptr_as_obj=(Object*)ptr_thr;
        ptr_as_obj->Decref();
        if ( ptr_as_obj->GetRefCount()<1 )
            delete ptr_as_obj;
        ptr_thr=NULL;
    }
    if ( !ctx.IsNull() )
    {
        Object * ptr_as_obj=ctx.getPointer();

        ptr_as_obj->Incref();
        pthread_setspecific(*((pthread_key_t*)thread_key),(void*)ptr_as_obj);
    }
#endif
#ifdef WIN32
    void * dptr;
    dptr=TlsGetValue((DWORD)thread_key);
    if ( dptr!=NULL )
    {
        Object * ptr_as_obj=(Object*)dptr;
        ptr_as_obj->Decref();
        if ( ptr_as_obj->GetRefCount()<1 )
            delete ptr_as_obj;
        dptr=NULL;
    }
    if ( !ctx.IsNull() )
    {
        Object * ptr_as_obj=ctx.getPointer();
        ptr_as_obj->Incref();
        TlsSetValue((DWORD)thread_key,dptr);
    }
#endif
}

Meow::Core::Object_ptr Meow::Core::System::Thread::GetContext( )
{
    Meow::Core::Object_ptr retval;

    if ( thread_key==NULL )
    {
#ifdef __USE_PTHREADS
        thread_key=malloc(sizeof(pthread_key_t));
        pthread_key_t *thread_key_converted=(pthread_key_t*)thread_key;
        pthread_key_create(thread_key_converted,NULL);
        sharers++;
#endif
#ifdef WIN32
        thread_key=(void*)TlsAlloc();
        sharers++;
#endif
    }

#ifdef __USE_PTHREADS
    ptr ptr_thr=pthread_getspecific(*(pthread_key_t*)thread_key);
    retval=ptr_thr;
#endif
#ifdef WIN32
    void * dptr;
    dptr=TlsGetValue((DWORD)thread_key);
#endif
    return retval;
}

void Meow::Core::System::Thread::SetMemory( ptr object )
{
    Memory=object;
}

void Meow::Core::System::Thread::SetFunction( Threadable_ptr object )
{
    Function=object;
}

void Meow::Core::System::Thread::Detach( )
{
    if ( ! Function.IsNull() )
    {
#ifdef __USE_PTHREADS
        pthread_t tid;
#endif
#ifdef WIN32
        HANDLE    thread;
        DWORD     tid;
#endif
        int rc;
        if ( ! Memory.IsNull() )
            Function->SetMemory(Memory);

        struct thread_args *data=(struct thread_args*)malloc(sizeof(struct thread_args));
        data->function=(void*)Function.getPointerForceIncref();

#ifdef __USE_PTHREADS
        rc=pthread_create(&tid,NULL,ExecuteThread,(void*)data);
        if ( rc!=0 )
        {
            SystemException exception;
            exception.SetInitiator(this);
            exception.SetLineNumber(48);
            exception.SetMessage("Could not detach a thread, OS problem.");
            throw exception;
        }
#endif
#ifdef WIN32
        thread=CreateThread(NULL,65535,ExecuteThread,(LPVOID)data,0,&tid);
        if(!thread)
        {
            SystemException exception;
            exception.SetInitiator(this);
            exception.SetLineNumber(48);
            exception.SetMessage("Could not detach a thread, OS problem.");
            throw exception;
        }
#endif
        handle=(void*)tid;
    }
    else
    {
        SystemException exception;
        exception.SetInitiator(this);
        exception.SetLineNumber(46);
        exception.SetMessage("Could not detach thread as no function set.");
        throw exception;
    }
}

void * Meow::Core::System::Thread::thread_key=NULL;
unsigned long Meow::Core::System::Thread::sharers=0;
