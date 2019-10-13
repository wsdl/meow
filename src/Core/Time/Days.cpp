// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Time/Days.h>
#include <Core/Types/String.h>
#include <stdio.h>

Meow::Core::Time::Days::Days( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Days");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Days::Days( std::string date_as_string )
{
    SetMeowClassName("Days");
    Meow::Core::Types::String_ptr date=new Meow::Core::Types::String(date_as_string);
    ts=date->GetValueAsInteger();

}

Meow::Core::Time::Days::Days( long date_as_long )
{
    SetMeowClassName("Days");
    ts=date_as_long;

}
Meow::Core::Time::Days::Days()
{
    SetMeowClassName("Days");
    ts=0;
}

Meow::Core::Time::Days::~Days()
{
}

bool Meow::Core::Time::Days::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Days::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Days::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Days::GetValueAsInteger()
{
    return ts*86400;
}
double Meow::Core::Time::Days::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Days::GetValueAsString()
{
    char temp[255];
    sprintf(temp,"%d",(int)ts);

    std::string reval=temp;
    return reval;
}


void Meow::Core::Time::Days::Increment( )
{
    ts++;
}
void Meow::Core::Time::Days::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Days::Let( Meow::Core::Types::ValueType_ptr other )
{
    ts=other->GetValueAsInteger();
}
void Meow::Core::Time::Days::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
}
void Meow::Core::Time::Days::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
}
void Meow::Core::Time::Days::Divide( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts/other->GetValueAsInteger();
}
void Meow::Core::Time::Days::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
}
bool Meow::Core::Time::Days::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Days::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Days::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Days::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}

