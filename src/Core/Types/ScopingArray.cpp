// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetMeowClassName and GetMeowClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#include <Core/Types/ScopingArray.h>
#include <Core/Types/Integer.h>
#include <Core/Tools/XMLToArray.h>
#include <Object.h>
#include <LifecycleObject.h>
#include <stdio.h>


Meow::Core::Types::Scope::Scope() : isOpaque(false)
{
    SetMeowClassName("Scope");
    properties=new Array();
}
Meow::Core::Types::Scope::~Scope()
{
}

void Meow::Core::Types::Scope::DeepSet( string index, ptr item )
{
    if ( parent.IsNull() )
        properties->Set(index,item);
    else
        parent->DeepSet(index,item);
}
void Meow::Core::Types::Scope::DeepSet( string index, unsigned long id, ptr item )
{
    if ( parent.IsNull() )
        properties->Set(index,id,item);
    else
        parent->DeepSet(index,id,item);
}
void Meow::Core::Types::Scope::DeepSet( unsigned long index, ptr item )
{
    if ( parent.IsNull() )
        properties->Set(index,item);
    else
        parent->DeepSet(index,item);
}

void Meow::Core::Types::Scope::Set( string index, ptr item )
{
    if ( isOpaque || parent.IsNull() )
    {
        properties->Set(index,item);
    }
    else if ( !parent.IsNull() )
    {
        if ( parent->IsSet(index) )
        {
            parent->Set(index,item);
        }
        else
        {
            properties->Set(index,item);
        }
    }
    else
    {
        properties->Set(index,item);
    }
}
void Meow::Core::Types::Scope::Set( string index, unsigned long id, ptr item)
{
    if ( isOpaque || parent.IsNull() )
    {
        properties->Set(index,id,item);
    }
    else if ( !parent.IsNull() )
    {
        if ( parent->IsSet(index) )
        {
            parent->Set(index,id,item);
        }
        else
        {
            properties->Set(index,id,item);
        }
    }
    else
    {
        properties->Set(index,id,item);
    }
}
void Meow::Core::Types::Scope::Set( unsigned long index, ptr item )
{
    if ( isOpaque || parent.IsNull() )
    {
        properties->Set(index,item);
    }
    else if ( !parent.IsNull() )
    {
        if ( parent->IsSet(index) )
        {
            parent->Set(index,item);
        }
        else
        {
            properties->Set(index,item);
        }
    }
    else
    {
        properties->Set(index,item);
    }
}
Meow::Core::ptr  Meow::Core::Types::Scope::Get( string index )
{
    Meow::Core::ptr retval;
    if ( properties->IsSet(index) || isOpaque || parent.IsNull() )
        retval=properties->Get(index);
    else
    {
        // we know its not set here, so hunt in children
        retval=parent->Get(index);
    }
    return retval;
}
Meow::Core::ptr  Meow::Core::Types::Scope::Get( unsigned long index )
{
    Meow::Core::ptr retval;
    if ( properties->IsSet(index) || isOpaque || parent.IsNull() )
        retval=properties->Get(index);
    else
    {
        // we know its not set here, so hunt in children
        retval=parent->Get(index);
    }
    return retval;
}
Meow::Core::ptr  Meow::Core::Types::Scope::ParentGet( string index )
{
    Meow::Core::ptr retval;
    if ( !parent.IsNull() )
        retval=parent->Get(index);
    return retval;
}
Meow::Core::ptr  Meow::Core::Types::Scope::ParentGet( unsigned long index )
{
    Meow::Core::ptr retval;
    if ( !parent.IsNull() )
        retval=parent->Get(index);
    return retval;
}
bool Meow::Core::Types::Scope::IsSet( std::string key )
{
    bool retval;
    if ( properties->IsSet(key) )
        retval=true;
    else if ( isOpaque || parent.IsNull() )
        retval=false;
    else
        retval=parent->IsSet(key);
    return retval;
}
bool Meow::Core::Types::Scope::IsSet( unsigned long key )
{

    bool retval;
    if ( properties->IsSet(key) )
        retval=true;
    else if ( isOpaque || parent.IsNull() )
        retval=false;
    else
        retval=parent->IsSet(key);
    return retval;
}
unsigned long Meow::Core::Types::Scope::Count()
{
    unsigned long count=properties->Count();
    if ( !isOpaque && !parent.IsNull() )
        count=count+parent->Count();
    return count;
}
void Meow::Core::Types::Scope::Unset( std::string key )
{
    if ( properties->IsSet(key) )
        properties->Unset(key);
    else if ( !isOpaque && !parent.IsNull() )
        parent->Unset(key);
}
void Meow::Core::Types::Scope::Unset( unsigned long key )
{
    if ( properties->IsSet(key) )
        properties->Unset(key);
    else if ( !isOpaque && !parent.IsNull() )
        parent->Unset(key);
}
Meow::Core::Types::Stack_ptr Meow::Core::Types::Scope::GetKeys( )
{
    Core::Types::Array_ptr set=new Core::Types::Array();
    ptr nullvalue;
    if ( !isOpaque && !parent.IsNull() )
    {
        Core::Types::Stack_ptr childKeys=parent->GetKeys();
        while ( !childKeys->IsEmpty() )
        {
            String_ptr key=(String_ptr)childKeys->Pop();
            set->Set(key->GetValue(),nullvalue);
        }
    }
    Core::Types::Stack_ptr myKeys=properties->GetKeys();
    while ( !myKeys->IsEmpty() )
    {
        String_ptr key=(String_ptr)myKeys->Pop();
        set->Set(key->GetValue(),nullvalue);
    }
    return set->GetKeys();
}
Meow::Core::Types::Stack_ptr Meow::Core::Types::Scope::GetValues( )
{
    Core::Types::Stack_ptr keys=GetKeys();
    Core::Types::Array_ptr set=new Core::Types::Array();

    if ( !isOpaque && !parent.IsNull() )
    {
        Core::Types::Stack_ptr childKeys=parent->GetKeys();
        while ( !childKeys->IsEmpty() )
        {
            String_ptr key=(String_ptr)childKeys->Pop();
            set->Set(key->GetValue(),Get(key->GetValue()));
        }
    }
    Core::Types::Stack_ptr myKeys=properties->GetKeys();
    while ( !myKeys->IsEmpty() )
    {
        String_ptr key=(String_ptr)myKeys->Pop();
        set->Set(key->GetValue(),properties->Get(key->GetValue()));
    }
    return set->GetValues();
}

Meow::Core::Types::ScopingArray::ScopingArray()
{
    SetMeowClassName("ScopingArray");
    iScopes=new Core::Types::Stack();
    Scope_ptr default_scope=new Scope();
    iScopes->Push((ptr)default_scope);
}
Meow::Core::Types::ScopingArray::~ScopingArray()
{
    while ( ! iScopes->IsEmpty() )
        ptr result=iScopes->Pop();
}

void Meow::Core::Types::ScopingArray::DeepSet( std::string index, ptr item )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->DeepSet(index,item);
}

void Meow::Core::Types::ScopingArray::DeepSet( std::string index, unsigned long id,  ptr item )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->Set(index,id,item);
}

void Meow::Core::Types::ScopingArray::DeepSet( unsigned long index, ptr item )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->DeepSet(index,item);
}

void Meow::Core::Types::ScopingArray::Set( std::string index, ptr item )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->Set(index,item);
}

void Meow::Core::Types::ScopingArray::Set( std::string index, unsigned long id,  ptr item )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->Set(index,id,item);
}

void Meow::Core::Types::ScopingArray::Set( unsigned long index, ptr item )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->Set(index,item);
}

Meow::Core::ptr Meow::Core::Types::ScopingArray::Get( std::string index )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->Get(index);
}

Meow::Core::ptr Meow::Core::Types::ScopingArray::Get( unsigned long index )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->Get(index);
}


Meow::Core::ptr Meow::Core::Types::ScopingArray::ParentGet( std::string index )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->Get(index);
}

Meow::Core::ptr Meow::Core::Types::ScopingArray::ParentGet( unsigned long index )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->Get(index);
}

bool Meow::Core::Types::ScopingArray::IsSet( std::string index )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->IsSet(index);
}

bool Meow::Core::Types::ScopingArray::IsSet( unsigned long index )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->IsSet(index);

}

unsigned long Meow::Core::Types::ScopingArray::Count( )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->Count();
}

void Meow::Core::Types::ScopingArray::Unset( std::string key )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->Unset(key);
}

void Meow::Core::Types::ScopingArray::Unset( unsigned long key )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    currentscope->Unset(key);
}

Meow::Core::Types::Stack_ptr Meow::Core::Types::ScopingArray::GetKeys( )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->GetKeys();
}

Meow::Core::Types::Stack_ptr Meow::Core::Types::ScopingArray::GetValues( )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    return currentscope->GetValues();
}
void Meow::Core::Types::ScopingArray::PushScope()
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    Scope_ptr newScope=new Scope();
    iScopes->Push((ptr)newScope);
    newScope->parent=currentscope;
}
void Meow::Core::Types::ScopingArray::PushScopeOpaque()
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    Scope_ptr newScope=new Scope();
    iScopes->Push((ptr)newScope);
    newScope->parent=currentscope;
    newScope->isOpaque=true;
}
void Meow::Core::Types::ScopingArray::PushScopeOpaqueWithCopy( Meow::Core::Types::Array_ptr survivors )
{
    Scope_ptr currentscope=(Scope_ptr)iScopes->Top();
    Scope_ptr newScope=new Scope();
    iScopes->Push((ptr)newScope);
    newScope->parent=currentscope;
    newScope->isOpaque=true;
    Stack_ptr keys=currentscope->GetKeys();
    if ( !survivors.IsNull() )
    {
        // Filter copy
        while ( !keys->IsEmpty() )
        {
            String_ptr key=(String_ptr)keys->Pop();
            if ( survivors->IsSet(key->GetValueAsString()))
                newScope->properties->Set(key->GetValueAsString(),currentscope->Get(key->GetValueAsString()));
        }
    }
    else
    {
        while ( !keys->IsEmpty() )
        {
            String_ptr key=(String_ptr)keys->Pop();
            newScope->properties->Set(key->GetValueAsString(),currentscope->Get(key->GetValueAsString()));
        }
    }
}
void Meow::Core::Types::ScopingArray::PopScope()
{
    iScopes->Pop();
}


