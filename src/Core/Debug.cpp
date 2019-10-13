// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Debug.h>
#include <Core/Object.h>
#include <unistd.h>
#include <iostream>

DebugRecordMain * g_active;
DebugRecordMain * g_inactive;

using namespace std;

void GlobalInit( )
{
    g_active=(DebugRecordMain*)malloc(sizeof(DebugRecordMain));
    g_active->counter=0;
    g_active->top=NULL;
    g_active->permit=true;
    g_inactive=(DebugRecordMain*)malloc(sizeof(DebugRecordMain));
    g_inactive->counter=0;
    g_inactive->top=NULL;
    g_inactive->permit=true;
}

DebugRecordMain * GetGlobalActiveRecorder( )
{
    return g_active;
}

DebugRecordMain * GetGlobalInactiveRecord( )
{
    return g_inactive;
}

void AddGlobalActivePointer(
    Meow::Core::Object * pointer
)
{
      if ( g_active==NULL || g_inactive==NULL )
          GlobalInit();

      if ( g_active->top==NULL )
      {
        g_active->top=(DebugRecord*)malloc(sizeof(DebugRecord));
        g_active->top->pointer=pointer;
        g_active->top->next=NULL;
      }
      else
      {
        DebugRecord * ptr;
        ptr=g_active->top;
        while ( ptr->next!=NULL )
          ptr=ptr->next;
        ptr->next=(DebugRecord*)malloc(sizeof(DebugRecord));
        ptr->next->pointer=pointer;
        ptr->next->next=NULL;
      }
      g_active->counter++;
}

void RemoveFromGlobalActivePointer(
    Meow::Core::Object * pointer
)
{

    if ( ! g_active->permit )
      return;

    if ( g_active->counter==0 )
      return;
    if ( g_active->top->pointer==pointer )
    {
      DebugRecord * ptr;
      ptr=g_active->top;
      g_active->top=g_active->top->next;
      free(ptr);
    }
    else
    {
      DebugRecord * ptr;
      DebugRecord * chaser;

      ptr=g_active->top;
      chaser=NULL;
      while ( ptr->next!=NULL && ptr->pointer!=pointer )
      {
        chaser=ptr;
        ptr=ptr->next;
      }
      if ( ptr->pointer==pointer )
      {
        chaser->next=ptr->next;
        free(ptr);
      }
    }
    g_active->counter--;
}

void CleanupGlobalActivePointer( )
{
      g_active->permit=false;
      if ( g_active->counter>0 )
      {
        DebugRecord * ptr;
        DebugRecord * chaser;

        chaser=NULL;
        ptr=g_active->top;
        while ( ptr!=NULL )
        {
          chaser=ptr;
          delete chaser->pointer;
          free(chaser);
          ptr=ptr->next;
        }
      }
      else
      {
        cerr << "No items to dispose of." << endl;
      }
}


void AddGlobalInactivePointer(
    Meow::Core::Object * pointer
)
{
    if ( g_inactive->top==NULL )
    {
      g_inactive->top=(DebugRecord*)malloc(sizeof(DebugRecord));
      g_inactive->top->pointer=pointer;
      g_inactive->top->next=NULL;
    }
    else
    {
      DebugRecord * ptr;
      ptr=g_inactive->top;
      while ( ptr->next!=NULL )
        ptr=ptr->next;
      ptr->next=(DebugRecord*)malloc(sizeof(DebugRecord));
      ptr->next->pointer=pointer;
      ptr->next->next=NULL;
    }
    g_inactive->counter++;
}

void RemoveFromGlobalInactivePointer(
    Meow::Core::Object * pointer
)
{
      if ( ! g_inactive->permit )
        return;

      if ( g_inactive->counter==0 )
        return;

      if ( g_inactive->top->pointer==pointer )
      {
        DebugRecord * ptr;
        ptr=g_inactive->top;
        g_inactive->top=g_inactive->top->next;
        free(ptr);
      }
      else
      {
        DebugRecord * ptr;
        DebugRecord * chaser;

        ptr=g_inactive->top;
        chaser=NULL;
        while ( ptr->next!=NULL && ptr->pointer!=pointer )
        {
          chaser=ptr;
          ptr=ptr->next;
        }
        if ( ptr->pointer==pointer )
        {
          chaser->next=ptr->next;
          free(ptr);
        }
      }
      g_active->counter--;
}

void CleanupGlobalInactivePointer( )
{
      g_inactive->permit=false;
      if ( g_inactive->counter>0 )
      {
        DebugRecord * ptr;
        DebugRecord * chaser;

        chaser=NULL;
        ptr=g_inactive->top;
        while ( ptr!=NULL )
        {
          chaser=ptr;
          ptr=ptr->next;
          delete chaser->pointer;
          free(chaser);
        }
      }
      else
      {
        cerr << "No items to dispose of." << endl;
      }
}
