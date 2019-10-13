// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/Boolean.h>
#include <sstream>
#include <stdlib.h>

Meow::Core::Types::Boolean::Boolean( ) :
    Content(true)
{
    SetMeowClassName("Boolean");
}

Meow::Core::Types::Boolean::Boolean( std::string value )
{
    SetMeowClassName("Boolean");
    Content=!value.compare("true") || !value.compare("1");
}

Meow::Core::Types::Boolean::Boolean( bool value )
{
    SetMeowClassName("Boolean");
    Content=value;
}

Meow::Core::Types::Boolean::~Boolean( )
{
}

void Meow::Core::Types::Boolean::SetValue( bool item )
{
    Content=item;
}

bool Meow::Core::Types::Boolean::GetValue( )
{
    return Content;
}

std::string Meow::Core::Types::Boolean::ToString()
{
    if ( Content )
        return "true";

    return "false";
}

void Meow::Core::Types::Boolean::Increment()
{
    Content=!Content;
}

void Meow::Core::Types::Boolean::Decrement()
{
    Content=!Content;
}

bool Meow::Core::Types::Boolean::IsNumeric()
{
    return false;
}
bool Meow::Core::Types::Boolean::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Types::Boolean::CouldBeInteger()
{
    return false;
}
long Meow::Core::Types::Boolean::GetValueAsInteger()
{
    return (long)Content;
}
double Meow::Core::Types::Boolean::GetValueAsFloat()
{
    return (double)Content;
}

bool  Meow::Core::Types::Boolean::GetValueAsBoolean()
{
    return Content;
}
void  * Meow::Core::Types::Boolean::GetValueAsVoid()
{
    return &Content;
}
long  Meow::Core::Types::Boolean::GetVoidLength()
{
    return sizeof(bool);
}

std::string Meow::Core::Types::Boolean::GetValueAsString()
{
    return ToString();
}

void Meow::Core::Types::Boolean::Let( Meow::Core::Types::ValueType_ptr other )
{
    Content=other->GetValueAsBoolean();
}

void Meow::Core::Types::Boolean::Add( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content && other->GetValueAsBoolean();
}
void Meow::Core::Types::Boolean::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content && other->GetValueAsBoolean();
}
void Meow::Core::Types::Boolean::Negate( )
{
    Content=!Content;
}
bool Meow::Core::Types::Boolean::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    if ( Content && ! other->GetValueAsBoolean() )
        return true;
    return false;
}
bool Meow::Core::Types::Boolean::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    if ( Content && ! other->GetValueAsBoolean() )
        return false;
    return true;
}
bool Meow::Core::Types::Boolean::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return Content==other->GetValueAsBoolean();
}
bool Meow::Core::Types::Boolean::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return Content!=other->GetValueAsBoolean();
}

void Meow::Core::Types::Boolean::And( ValueType_ptr other )
{
    Content=Content&&other->GetValueAsBoolean();
}
void Meow::Core::Types::Boolean::Or( ValueType_ptr other )
{
    Content=Content||other->GetValueAsBoolean();
}
void Meow::Core::Types::Boolean::Xor( ValueType_ptr other )
{

}
