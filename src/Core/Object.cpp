// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Removed strdup of classname and the free.
//		Changed classname to Core/Object from CoreObject.
#include <Core/Object.h>
#ifdef WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <iphlpapi.h>
#else
	#include <unistd.h>
	#include <sys/utsname.h>
#endif
#include <string.h>
#include <stdio.h>
#include <iostream>


Meow::Core::Object::Object( ) :
    refcount(0),
    ___names(NULL),
    darea(NULL)
{
	
}

Meow::Core::Object::~Object( )
{
    if ( ___names!=NULL )
    {
        struct NameTracker * pointer=___names;
        struct NameTracker * chaser=NULL;
        while ( pointer!=NULL )
        {
            chaser=pointer;
            pointer=pointer->next;
            free(chaser);
        }
    }
}
std::string Meow::Core::Object::GetMeowClassName( )
{
    std::string name="Core/Object";
    struct NameTracker * pointer;
    pointer=___names;
    while ( pointer!=NULL )
    {
        name=pointer->classname;
        pointer=pointer->next;
    }
    return name;
}

void Meow::Core::Object::SetMeowClassName( std::string name )
{
    const char * val=name.c_str();

	if ( ___names==NULL )
	{
		___names=(struct NameTracker*)malloc(sizeof(struct NameTracker));
		___names->classname=(char*)"Core/Object";
		___names->next=NULL;
	}
    struct NameTracker * pointer=___names;

    while ( pointer->next!=NULL )
        pointer=pointer->next;
    pointer->next=(struct NameTracker*)malloc(sizeof(struct NameTracker));
    pointer->next->next=NULL;
    pointer->next->classname=strdup(name.c_str());
}

bool Meow::Core::Object::IsInstanceOf( std::string classname )
{
    bool retval=false;
    struct NameTracker * pointer;
    pointer=___names;
    while ( pointer!=NULL )
    {
        if ( !strcmp(classname.c_str(),pointer->classname) )
        {

            retval=true;
            pointer=NULL;
        }
        else
            pointer=pointer->next;
    }
    return retval;
}
void Meow::Core::Object::Incref()
{
    refcount++;

}
void Meow::Core::Object::Decref()
{
    refcount--;

}
unsigned long Meow::Core::Object::GetRefCount()
{
    unsigned long retval;
    retval=refcount;
    return retval;
}

void Meow::Core::Object::Pause( long microseconds )
{
#ifdef WIN32
	Sleep(microseconds);
#else
	usleep(microseconds);
#endif
}
void Meow::Core::Object::Clone( Meow::Core::Object * other )
{

}

unsigned long Meow::Core::Object::Count()
{
    return sizeof(this);
}


unsigned long Meow::Core::Object::GetProcessID()
{
#ifdef WIN32
    return (unsigned long)GetCurrentProcessId();
#else
    return (unsigned long)getpid();
#endif
}

std::string Meow::Core::GetHardwareKey()
{
	// TODO: Implement a better hardware key mechanism
	return "";
}
Meow::Core::ptr Meow::Core::Object::Null()
{
	Meow::Core::ptr retval;
	return retval;
}

