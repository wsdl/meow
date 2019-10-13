// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Time/Minutes.h>
#include <Core/Types/String.h>
#include <stdio.h>

Meow::Core::Time::Minutes::Minutes( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Minutes");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Minutes::Minutes( std::string date_as_string )
{
    SetMeowClassName("Minutes");
    Core::Types::String_ptr date=new Core::Types::String(date_as_string);
    ts=date->GetValueAsInteger();

}

Meow::Core::Time::Minutes::Minutes( long date_as_long )
{
    SetMeowClassName("Minutes");
    ts=date_as_long;

}
Meow::Core::Time::Minutes::Minutes()
{
    SetMeowClassName("Minutes");
    ts=0;
}

Meow::Core::Time::Minutes::~Minutes()
{

}

bool Meow::Core::Time::Minutes::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Minutes::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Minutes::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Minutes::GetValueAsInteger()
{
    return ts*60;
}
double Meow::Core::Time::Minutes::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Minutes::GetValueAsString()
{
    char temp[255];
    sprintf(temp,"%d",(int)ts);

    std::string reval=temp;
    return reval;
}


void Meow::Core::Time::Minutes::Increment( )
{
    ts++;
}
void Meow::Core::Time::Minutes::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Minutes::Let( Meow::Core::Types::ValueType_ptr other )
{
    ts=other->GetValueAsInteger();
}
void Meow::Core::Time::Minutes::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
}
void Meow::Core::Time::Minutes::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
}
void Meow::Core::Time::Minutes::Divide( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts/other->GetValueAsInteger();
}
void Meow::Core::Time::Minutes::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
}
bool Meow::Core::Time::Minutes::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Minutes::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Minutes::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Minutes::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}

