// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetMeowClassName and GetMeowClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#include <Core/Types/Stack.h>
#include <iostream>

Meow::Core::Types::Stack::Stack()
{
    SetMeowClassName("Stack");
}

Meow::Core::Types::Stack::~Stack()
{
}

void Meow::Core::Types::Stack::Push( ptr item )
{
    elements.push_back(item);
}

Meow::Core::ptr Meow::Core::Types::Stack::Pop( )
{
    Meow::Core::ptr retval;
    if ( !elements.empty() )
    {
        retval=elements.back();
        elements.pop_back();
    }
    return retval;
}


Meow::Core::ptr Meow::Core::Types::Stack::Top( )
{
	Meow::Core::ptr toreturn=elements.back();
    return toreturn;
}

unsigned long Meow::Core::Types::Stack::Count()
{
    return elements.size();
}

void Meow::Core::Types::Stack::Clear( )
{
    ptr Item;
    while ( !IsEmpty() )
        Item=Pop();
}

bool Meow::Core::Types::Stack::IsEmpty( )
{
    return elements.empty();
}

Meow::Core::Types::Stack_ptr Meow::Core::Types::Stack::Flip( )
{
    Meow::Core::Types::Stack_ptr retval=new Meow::Core::Types::Stack();
    for ( auto it=elements.begin(); it!=elements.end(); ++it)
    {
            retval->Push(*it);
    }
    return retval;
}


Meow::Core::Types::StackIterater::StackIterater():
    elementposition(0)
{

}

Meow::Core::Types::StackIterater::~StackIterater()
{
}

void Meow::Core::Types::StackIterater::SetStack( Meow::Core::Types::Stack_ptr stackptr )
{
    stack=stackptr;
    elementposition=0;
}

Meow::Core::ptr Meow::Core::Types::StackIterater::First( )
{
	Meow::Core::ptr retval;

    if ( !stack.IsNull() )
        retval=stack->Top();
    elementposition=0;
}

Meow::Core::ptr Meow::Core::Types::StackIterater::GetNextElement( )
{
    int counter;
    Meow::Core::ptr retval;
    StackElement_ptr Current;

    if ( ! HasMoreElements( ) )
        return retval;

    auto it=stack->elements.begin();
    for ( int i=0; i<elementposition; i++ )
        ++it;
    ++it;
    retval=*it;
    return retval;
}


bool Meow::Core::Types::StackIterater::HasMoreElements( )
{
    if ( stack.IsNull() )
        return false;
    if ( elementposition>=stack->Count() )
        return false;
    return true;
}

