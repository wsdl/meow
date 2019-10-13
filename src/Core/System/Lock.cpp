// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/System/Lock.h>
#include <iostream>
#ifdef __USE_PTHREADS
	#include <pthread.h>
#endif
#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

Meow::Core::System::Lock::Lock( ) :
    __handle(NULL)
{
    SetMeowClassName("Lock");
}

Meow::Core::System::Lock::~Lock( )
{
    if ( __handle!=NULL )
    {
#ifdef __USE_PTHREADS
        pthread_mutex_destroy((pthread_mutex_t*)__handle);
        delete (pthread_mutex_t*)__handle;
#endif
#ifdef WIN32
        ReleaseMutex((HANDLE)__handle);
        CloseHandle((HANDLE)__handle);
#endif
    }
}

void Meow::Core::System::Lock::LazyInit( )
{
#ifdef __USE_PTHREADS
    __handle=(void*)new pthread_mutex_t;

    pthread_mutex_init((pthread_mutex_t*)__handle,NULL);
#endif
#ifdef WIN32
    __handle=(void*)CreateMutex(NULL,FALSE,NULL);
#endif
}

bool Meow::Core::System::Lock::Try( )
{
    if ( __handle==NULL )
        LazyInit();
#ifdef __USE_PTHREADS
    return (bool)pthread_mutex_trylock((pthread_mutex_t*)__handle);
#endif
#ifdef WIN32
    return (WAIT_OBJECT_0 == WaitForSingleObject((HANDLE)__handle, 0));
#endif
}

void Meow::Core::System::Lock::Gain( )
{
    if ( __handle==NULL )
        LazyInit();
#ifdef __USE_PTHREADS
    pthread_mutex_lock((pthread_mutex_t*)__handle);
#endif
#ifdef WIN32
    WaitForSingleObject((HANDLE)__handle, INFINITE);
#endif
}

void Meow::Core::System::Lock::Release( )
{
    if ( __handle==NULL )
        LazyInit();

#ifdef __USE_PTHREADS
    pthread_mutex_unlock((pthread_mutex_t*)__handle);
#endif
#ifdef WIN32
    ReleaseMutex((HANDLE)__handle);
#endif
}

void Meow::Core::System::Lock::Respite( )
{
#ifdef __USE_PTHREADS
    usleep(10);
#endif
#ifdef WIN32
    Sleep(10);
#endif
}
