// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Threadable.h>
#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#include <iostream>

Meow::Core::Threadable::Threadable( )
{
    SetMeowClassName("Threadable");
#ifdef __USE_PTHREADS
    iSignal=(void*)malloc(sizeof(pthread_cond_t));
    iMutex=(void*)malloc(sizeof(pthread_mutex_t));

    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_cond_init(__signal,NULL);

    pthread_mutex_t * __mutex=(pthread_mutex_t*)iMutex;
    pthread_mutex_init(__mutex,NULL);
#else

#endif
}

Meow::Core::Threadable::~Threadable( )
{
#ifdef __USE_PTHREADS
    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_mutex_t * __mutex=(pthread_mutex_t*)iMutex;

    pthread_mutex_unlock(__mutex);
    pthread_cond_destroy(__signal);
    free(__mutex);
    free(__signal);
#else
#endif
}

void Meow::Core::Threadable::SetMemory( Meow::Core::ptr object )
{
    Memory=object;
}

Meow::Core::ptr Meow::Core::Threadable::GetMemory( )
{
    return Memory;
}

void Meow::Core::Threadable::Run( )
{

}

void Meow::Core::Threadable::Respite( )
{
#ifdef __USE_PTHREADS
    usleep(100);
#endif
#ifdef WIN32
    Sleep(100);
#endif
}

void Meow::Core::Threadable::Signal( )
{
#ifdef __USE_PTHREADS
    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_cond_signal(__signal);
#endif
}

void Meow::Core::Threadable::WaitForSignal(  )
{
#ifdef __USE_PTHREADS
    pthread_cond_t * __signal=(pthread_cond_t*)iSignal;
    pthread_mutex_t * __mutex=(pthread_mutex_t*)iMutex;

    pthread_mutex_lock(__mutex);
    pthread_cond_wait(__signal,__mutex);
    pthread_mutex_unlock(__mutex);

#endif
}
