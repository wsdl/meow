// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/Array.h>
#include <Core/Types/Integer.h>
#include <Core/Tools/XMLToArray.h>
#include <Object.h>
#include <LifecycleObject.h>
#include <stdio.h>

#include <stack>
template <class T> 
Meow::Core::Types::Array<T>::Array() 
{
    SetMeowClassName("Array");
}
template <class T>
void Meow::Core::Types::Array<T>::Import( std::string array_as_string ) 
{
    SetMeowClassName("Array");
	Meow::Core::Tools::XMLToArray_ptr parser=new Meow::Core::Tools::XMLToArray();
	Core::Types::Array_ptr arrayPtr=this;
    parser->SetArray(arrayPtr);
    parser->ParseString(array_as_string);
}
template <class T>
Meow::Core::Types::Array<T>::~Array()
{
    // This should clear itself up?
}
template <class T>
void Meow::Core::Types::Array<T>::Initialise( std::vector<T> elements )
{
    int counter=0;
    for ( T element: elements)
    {
        elements[Core::Types::ValueType::IntegerToString(counter)]=element;
        counter++;
    }
}
template <class T>
void Meow::Core::Types::Array<T>::Set( std::string index, T item )
{
    elements[index]=item; 
}
template <class T>
void Meow::Core::Types::Array<T>::Set( std::string index, unsigned long id,  T item )
{
    elements[index]=item;
}

void Meow::Core::Types::Array::Set( unsigned long index, ptr item )
{
    elements[Core::Types::ValueType::IntegerToString(index)]=item;
}

Meow::Core::ptr Meow::Core::Types::Array::Get( std::string index )
{
    Core::ptr retval;
    if ( elements.find(index)!=elements.end )
        retval=elements[index];
    else
        retval=NULL; 
    return retval;
}


Meow::Core::ptr Meow::Core::Types::Array::Get( unsigned long index )
{
    Core::ptr retval;
    int offset=0;
    for ( auto key=elements.begin(); key != elements.end(); ++key  )
    {
        retval=elements[Core::Types::ValueType::IntegerToString(index)];
        if ( offset>=index )
            break;
        offset++;
    }
    return retval;
}

bool Meow::Core::Types::Array::IsSet( std::string index )
{
    return elements[index]!=elements.end;
}

bool Meow::Core::Types::Array::IsSet( unsigned long index )
{
    return elements[Core::Types::ValueType::IntegerToString(index)]!=elements.end;

}

unsigned long Meow::Core::Types::Array::Count( )
{
    return elements.size();
}

void Meow::Core::Types::Array::Unset( std::string key )
{
    elements.erase(key);
}

void Meow::Core::Types::Array::Unset( unsigned long key )
{
    elements.erase(Core::Types::ValueType::IntegerToString(key));
}

Meow::Core::Types::Stack_ptr Meow::Core::Types::Array::GetKeys( )
{
    Stack_ptr mystack;
    mystack=new Core::Types::Stack();
    for(std::unordered_map<std::string,Core::ptr>::iterator it = elements.begin(); it != elements.end(); ++it) 
        mystack->Push(new Core::Types::String(it->first));
   return mystack;
}
bool Meow::Core::Types::Array::FindIn( Meow::Core::Types::ValueType_ptr value )
{
    bool found=false;
    for (auto& x: elements) 
    {
        ptr Item=x.second;
        if ( Item->IsInstanceOf("ValueType") )
        {
            ValueType_ptr value=(ValueType_ptr)Item;
            if ( value->Equals(value) )
            {
                found=true;
                break;
            }
        }
    }

    return found;
}

Meow::Core::Types::Stack_ptr Meow::Core::Types::Array::GetValues( )
{

    Stack_ptr stack=new Stack();
    for (auto& x: elements) 
    {
        ptr Item=x.second;
        stack->Push(Item);
    }
    return stack;
}

unsigned long Meow::Core::Types::Array::IndexOf( std::string index )
{
    unsigned long offset=0;
    for ( auto key=elements.begin(); key != elements.end(); ++key  )
    {
        if ( index==key->first )
            break;
        offset++;
    }
    
    return offset;
}

bool Meow::Core::Types::Array::IsNumeric()
{
    return false;
}
bool Meow::Core::Types::Array::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Types::Array::CouldBeInteger()
{
    return false;
}
std::string	Meow::Core::Types::Array::GetValueAsString()
{
    std::string retval;
   
   
    retval="<Array>\n";
    unsigned long offset=0;
    for ( auto key=elements.begin(); key != elements.end(); ++key  )
    {
        
        std::string treatas="";
        std::string realvalue="";
        if ( key->second.IsNull() )
        {
            treatas="Null";
            realvalue="0x0";
        }
        else if ( key->second->IsInstanceOf("Browser/Input/HTTPFile") )
        {
            treatas="Null";
            realvalue="#PRIVATE#";
        }
        else if ( key->second->IsInstanceOf("Array") )
        {
            // Fix for array of arrays
            treatas="Array";
            Array_ptr arrayOfData=(Array_ptr)key->second;
            realvalue=arrayOfData->GetValueAsString();
        }
        else if ( key->second->IsInstanceOf("ValueType") )
        {
            treatas="ValueType";
            ValueType_ptr value=(ValueType_ptr)key->second;
            realvalue=value->GetValueAsString();
        }
        else if ( key->second->IsInstanceOf("Core/Object") )
        {
            treatas="Core/Object";
            ptr value=(ptr)key->second;
            realvalue=value->GetMeowClassName();
        }
        else
        {
            treatas="Null";
            realvalue="#PRIVATE#";
        }
        Integer_ptr intval=new Integer(offset);
        retval=retval + "<Element key=\"" + key->first +"\" treatas=\"" + treatas + "\" id=\"" + intval->GetValueAsString() + "\" class=\"";
        if ( !key->second.IsNull() )
        {
            retval=retval + key->second->GetMeowClassName() + "\">" + realvalue + "</Element>\n";
        }
        else
        {
            retval=retval + "invalid" + "\">" + realvalue + "</Element>\n";
        }
    }
    retval=retval + "</Array>\n";
    return retval;
}
void Meow::Core::Types::Array::Append( Meow::Core::Types::ValueType_ptr other )
{
    if ( other->IsInstanceOf("Array") )
    {
        Array_ptr other_as_array=(Array_ptr)other;
        Stack_ptr keys=other_as_array->GetKeys();
        while ( !keys->IsEmpty() )
        {
            String_ptr key=(String_ptr)keys->Pop();
            Set(key->GetValue(),other_as_array->Get(key->GetValue()));
        }
    }
    else
    {
        Set(Count(),(ptr)other);
    }

}

void Meow::Core::Types::Array::Let( Meow::Core::Types::ValueType_ptr other )
{
    if ( other->IsInstanceOf("Array") )
    {
        elements=((Core::Types::Array_ptr)other)->elements;
    }
}
bool Meow::Core::Types::Array::Equals( Core::Types::ValueType_ptr other )
{
    if ( !other->IsInstanceOf("Array") )
        return false;
    if ( other->Count()!=Count() )
        return false;
    Array_ptr other_as_array=(Array_ptr)other;

    Stack_ptr keys=other_as_array->GetKeys();
    while ( !keys->IsEmpty() )
    {
        String_ptr key=(String_ptr)keys->Pop();
        ptr left=Get(key->GetValueAsString());
        ptr right=other_as_array->Get(key->GetValueAsString());
        if ( left.IsNull() && !right.IsNull() )
            return false;
        if ( !left.IsNull() && right.IsNull() )
            return false;
        if ( left->IsInstanceOf("ValueType") && !right->IsInstanceOf("ValueType") )
            return false;
        {
            Core::Types::ValueType_ptr vLeft=(Core::Types::ValueType_ptr)left;
            Core::Types::ValueType_ptr vRight=(Core::Types::ValueType_ptr)right;
            if ( !vLeft->Equals(vRight) )
                return false;
        }
    }
    return true;
}
bool Meow::Core::Types::Array::NotEquals( Core::Types::ValueType_ptr other )
{
    return !Equals(other);
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::Array::Left( )
{
    unordered_map<std::string,ptr> replacement;
    Core::Types::ValueType_ptr retval;
    for ( auto key=elements.begin(); key != elements.end(); ++key  )
    {
        if ( retval.IsNull() )
            retval=(Core::Types::ValueType_ptr)key->second;
        else
            replacement[key->first]=key->second;
    }
    elements.clear();
    elements=replacement;
    
    return retval;
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::Array::Right( )
{
    unordered_map<std::string,ptr> replacement;
    Core::Types::ValueType_ptr retval;
    unsigned long max=elements.size();
    max--;
    unsigned long offset=0;
    for ( auto key=elements.begin(); key != elements.end(); ++key  )
    {
        retval=(Core::Types::ValueType_ptr)key->second;
        replacement[key->first]=key->second;
        if ( max <= offset )
            break;
    }
    elements.clear();
    elements=replacement;
    return retval;
}
unsigned long Meow::Core::Types::Array::Hash( std::string value )
{
    const char * str=value.c_str();
    unsigned long hash=0;
    int c;
    while ( c= *str++ )
        hash = c + (hash<<6)+(hash<<16)-hash;
    return hash;
}
