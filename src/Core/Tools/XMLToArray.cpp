// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Tools/XMLToArray.h>
#include <Core/Types/String.h>
#include <Core/Types/Float.h>
#include <Core/Time/Date.h>
#include <Core/Types/Boolean.h>
#include <Core/Types/Void.h>
#include <Core/Types/Integer.h>
#include <iostream>
#include <Registry.h>

Meow::Core::Tools::XMLToArrayElement::XMLToArrayElement()
{
}

Meow::Core::Tools::XMLToArrayElement::~XMLToArrayElement()
{
}

Meow::Core::Tools::XMLToArray::XMLToArray()
{
    SetMeowClassName("XMLToArray");
    iArrayStack=new Core::Types::Stack();
    iElementStack=new Core::Types::Stack();
}
Meow::Core::Tools::XMLToArray::~XMLToArray()
{

}
void Meow::Core::Tools::XMLToArray::SetArray( Meow::Core::Types::Array_ptr arrayptr )
{
    iArrayStack->Push((Meow::Core::ptr)arrayptr);
}

void Meow::Core::Tools::XMLToArray::StartElement( string element, Meow::Core::Types::Array_ptr attributes )
{
	if ( !element.compare("Element") )
    {
        XMLToArrayElement_ptr current=new XMLToArrayElement();
        current->element=element;
        current->attributes=attributes;
        current->characterdata="";
        iElementStack->Push((ptr)current);

        Meow::Core::Types::String_ptr treatas=(Meow::Core::Types::String_ptr)attributes->Get("treatas");
        Meow::Core::Types::String_ptr id=(Meow::Core::Types::String_ptr)attributes->Get("id");
        Meow::Core::Types::String_ptr key=(Meow::Core::Types::String_ptr)attributes->Get("key");
        Meow::Core::Types::String_ptr classname=(Meow::Core::Types::String_ptr)attributes->Get("class");

        if ( !classname->GetValue().compare("Array") )
        {
            iArrayStack->Push(new Meow::Core::Types::Array());
        }
    }
}
void Meow::Core::Tools::XMLToArray::EndElement( string element )
{
	 if ( !element.compare("Element") )
    {
        XMLToArrayElement_ptr 	current=(XMLToArrayElement_ptr)iElementStack->Pop();
        Meow::Core::Types::String_ptr 				treatas=(Meow::Core::Types::String_ptr)current->attributes->Get("treatas");
        Meow::Core::Types::String_ptr				id=(Meow::Core::Types::String_ptr)current->attributes->Get("id");
        Meow::Core::Types::String_ptr 				key=(Meow::Core::Types::String_ptr)current->attributes->Get("key");
        Meow::Core::Types::String_ptr 				classname=(Meow::Core::Types::String_ptr)current->attributes->Get("class");

        ptr storage;

        if ( !treatas->GetValue().compare("ValueType") )
        {
            Meow::Core::Types::ValueType_ptr value=new Meow::Core::Types::String(current->characterdata);
           //  Meow::Core::Types::ValueType_ptr value=value_64->Base64Decode();
            if ( !classname->GetValue().compare("Boolean") )
            {
                storage=new Meow::Core::Types::Boolean(value->GetValueAsString());
            }
            else if ( !classname->GetValue().compare("String") )
            {
                storage=new Meow::Core::Types::String(value->GetValueAsString());
            }
            else if ( !classname->GetValue().compare("Integer") )
            {
                storage=new Meow::Core::Types::Integer(value->GetValueAsString());
            }
            else if ( !classname->GetValue().compare("Float") )
            {
                storage=new Meow::Core::Types::Float(value->GetValueAsString());
            }
            else if ( !classname->GetValue().compare("Void") )
            {
                storage=new Meow::Core::Types::Void(value->GetValueAsString());
            }
            else if ( !classname->GetValue().compare("Date") )
            {
                storage=new Meow::Core::Time::Date(value->GetValueAsString());
            }
            else if ( !classname->GetValue().compare("Array") )
            {
                storage=iArrayStack->Pop();
            }
        }
        else if ( !treatas->GetValue().compare("Object") )
        {
        	/* This should be implemented in a subclass

            try
            {
                Registry_ptr registry=Registry::GetRegistry();
                Array_ptr lkey=registry->InterpretKey(current->characterdata);
                storage=registry->GetMe(classname->GetValue(),lkey);
            } catch ( Exceptions::NotFound &e )
            {

            }
            */
        	// We can't really do anything here
        }
        else if ( !treatas->GetValue().compare("CoreObject") )
        {
            // We can't really do anything here

        }
        else
        {
        	 // We can't really do anything here
        }
        Meow::Core::Types::Integer_ptr id_as_int=new Meow::Core::Types::Integer(id->GetValueAsString());
        Meow::Core::Types::Array_ptr current_array=(Meow::Core::Types::Array_ptr)iArrayStack->Top();
        if ( !key.IsNull() )
            current_array->Set(key->GetValue(),id_as_int->GetValue(),(ptr)storage);
        else
            current_array->Set(id_as_int->GetValue(),(ptr)storage);

    }
}
void Meow::Core::Tools::XMLToArray::CharacterData( string data )
{
    if ( !iElementStack->IsEmpty() )
    {
        XMLToArrayElement_ptr current=(XMLToArrayElement_ptr)iElementStack->Top();
        current->characterdata=current->characterdata + data;
    }
}

