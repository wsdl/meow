// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetMeowClassName and GetMeowClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#include <Core/Types/Queue.h>
#include <Core/Types/String.h>

Meow::Core::Types::QueueElement::QueueElement()
{
}

Meow::Core::Types::QueueElement::~QueueElement()
{
}

Meow::Core::Types::Queue::Queue() :
    itemcount(0)
{
    SetMeowClassName("Queue");
    TopObject=NULL;
    BottomObject=NULL;
}

Meow::Core::Types::Queue::~Queue()
{
    if ( itemcount>0 )
        Clear();
}


void Meow::Core::Types::Queue::Push( string data )
{
    String_ptr item=new String();
    item->SetValue(data);
    Push((ptr)item);
}

void Meow::Core::Types::Queue::Push( ptr item )
{
    if ( itemcount==0 )
    {
        TopObject=new QueueElement();
        TopObject->Item=item;
        TopObject->Next=NULL;
        BottomObject=TopObject;
    }
    else
    {
        BottomObject->Next=new QueueElement();
        BottomObject->Next->Next=TopObject;
        BottomObject->Next->Item=item;
        BottomObject=BottomObject->Next;
    }
    itemcount++;
}

Meow::Core::ptr Meow::Core::Types::Queue::Pull( )
{
    Meow::Core::ptr retval;
    if ( itemcount==0 )
    {
        return retval;
    }
    retval=TopObject->Item;
    TopObject=TopObject->Next;
    itemcount--;
    if ( itemcount==0 )
        BottomObject=NULL;
    return retval;
}

Meow::Core::ptr Meow::Core::Types::Queue::First( )
{
    return TopObject->Item;
}

void Meow::Core::Types::Queue::Clear( )
{
    ptr Item;
    while ( !IsEmpty() )
        Item=Pull();
}

bool Meow::Core::Types::Queue::IsEmpty( )
{
    return itemcount==0;
}

Meow::Core::Types::QueueIterater::QueueIterater():
    elementposition(0)
{

}

Meow::Core::Types::QueueIterater::~QueueIterater()
{
}

void Meow::Core::Types::QueueIterater::SetQueue( Meow::Core::Types::Queue_ptr Queueptr )
{
    Queue=Queueptr;
    elementposition=0;
}

Meow::Core::ptr Meow::Core::Types::QueueIterater::First( )
{
    Meow::Core::ptr retval;

    if ( Queue.IsNull() )
        return retval;
    elementposition=0;
    return Queue->First();
}

Meow::Core::ptr Meow::Core::Types::QueueIterater::GetNextElement( )
{
    int counter;
    Meow::Core::ptr retval;
    QueueElement_ptr Current;

    if ( ! HasMoreElements( ) )
        return retval;

    counter=elementposition;
    Current=Queue->TopObject;
    while( counter>0 )
    {
        Current=Current->Next;
        counter--;
    }

    elementposition++;
    retval=Current->Item;
    return retval;
}

bool Meow::Core::Types::QueueIterater::HasMoreElements( )
{
    if ( Queue.IsNull() )
        return false;
    if ( elementposition>=Queue->itemcount )
        return false;
    return true;
}
unsigned long Meow::Core::Types::Queue::Count( )
{
    return itemcount;
}

