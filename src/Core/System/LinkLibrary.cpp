// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/System/Compat.h>
#include <Core/System/LinkLibrary.h>
#include <Core/Log.h>


#ifdef WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

Meow::Core::System::LinkLibrary::LinkLibrary( ) :
    LoadCount(0),
    Library(NULL)
{
    SetMeowClassName("LinkLibrary");
}

Meow::Core::System::LinkLibrary::~LinkLibrary( )
{
#ifdef WIN32
    if ( LoadCount>0 )
        FreeLibrary((HINSTANCE)Library);
#else
    if ( LoadCount>0 )
        dlclose(Library);
#endif
}

void Meow::Core::System::LinkLibrary::Install( std::string libraryname )
{
    if ( LoadCount<1 )
    {
#ifndef WIN32
        // Clear old condition
    	dlerror();
        Library=dlopen(libraryname.c_str(),RTLD_NOW);
        if ( Library==NULL )
        {
            char * error=dlerror();
            if ( error!=NULL )
			{
				Core::Log::GetLog()->Error("Meow::Core::System::LinkLibrary::Install","ssss","Install of ",libraryname.c_str()," failed because ",error);
			}
            else
            	Core::Log::GetLog()->Error("Meow::Core::System::LinkLibrary::Install","ss",libraryname.c_str()," may not have loaded but no error returned from dlerror()");
            SystemException exception;
            exception.SetMessage(error);
            throw exception;
        }
        else
        		LoadCount++;
#endif
#ifdef WIN32
        Library=(void*)LoadLibrary(libraryname.c_str());
        if ( Library!=NULL )
        {
            LoadCount++;
        }
        else
        {
            char * errorMessage="";
            SystemException exception;
            exception.SetMessage(GetLastErrorAsString());
            Core::Log::GetLog()->Error("Meow::Core::System::LinkLibrary::Install","ssss","Install of ",libraryname.c_str()," failed because ",GetLastError());
            throw exception;
        }
#endif
    }
}

void * Meow::Core::System::LinkLibrary::GetFunction( std::string ordinalname )
{
    void * retval=NULL;
    if ( LoadCount>0 )
    {
#ifndef WIN32
        dlerror();
        retval=dlsym(Library,ordinalname.c_str());
        char * errorMessage=dlerror();
        if ( errorMessage!=NULL )
        {
        	Core::Log::GetLog()->Error("Meow::Core::System::LinkLibrary::Install","ssss","Get of ",ordinalname.c_str()," failed because ",errorMessage);
            retval=NULL;
        }
#endif
#ifdef WIN32
        retval=(void*)GetProcAddress((HINSTANCE)Library,ordinalname.c_str());
#endif
        if ( retval==NULL )
        {
            SystemException exception;
               Core::Log::GetLog()->Error("Meow::Core::System::LinkLibrary::Install","ss","Install of library failed because I could not locate ordinal ",ordinalname.c_str());

#ifndef WIN32
            if ( errorMessage!=NULL )
            {
            	Core::Log::GetLog()->Error("Meow::Core::System::LinkLibrary::Install","ssss","Ordinal ",ordinalname.c_str()," failed due to error ",errorMessage);
                exception.SetMessage(errorMessage);
            }
#endif
#ifdef WIN32
            exception.SetMessage(GetLastErrorAsString());
#endif

            throw exception;
        }
    }
    else
    {
        SystemException exception;
        exception.SetMessage("No library loaded.");
        throw exception;
    }
    return retval;
}
