// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Time/Hours.h>
#include <Core/Types/String.h>
#include <stdio.h>
Meow::Core::Time::Hours::Hours( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Hours");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Hours::Hours( std::string date_as_string )
{
    SetMeowClassName("Hours");
    Meow::Core::Types::String_ptr date=new Meow::Core::Types::String(date_as_string);
    ts=date->GetValueAsInteger();

}

Meow::Core::Time::Hours::Hours( long date_as_long )
{
    SetMeowClassName("Hours");
    ts=date_as_long;

}
Meow::Core::Time::Hours::Hours()
{
    SetMeowClassName("Hours");
    ts=0;
}

Meow::Core::Time::Hours::~Hours()
{

}

bool Meow::Core::Time::Hours::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Hours::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Hours::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Hours::GetValueAsInteger()
{
    return ts*3600;
}
double Meow::Core::Time::Hours::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Hours::GetValueAsString()
{
    char temp[255];
    sprintf(temp,"%d",(int)ts);

    std::string reval=temp;
    return reval;
}
void Meow::Core::Time::Hours::Increment( )
{
    ts++;
}
void Meow::Core::Time::Hours::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Hours::Let( Meow::Core::Types::ValueType_ptr other )
{
    ts=other->GetValueAsInteger();
}
void Meow::Core::Time::Hours::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
}
void Meow::Core::Time::Hours::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
}
void Meow::Core::Time::Hours::Divide( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts/other->GetValueAsInteger();
}
void Meow::Core::Time::Hours::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
}
bool Meow::Core::Time::Hours::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Hours::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Hours::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Hours::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}

