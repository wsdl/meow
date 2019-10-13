// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Time/Years.h>
#include <Core/Types/String.h>
#include <stdio.h>

Meow::Core::Time::Years::Years( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Years");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Years::Years( std::string date_as_string )
{
    SetMeowClassName("Years");
    Meow::Core::Types::String_ptr date=new Meow::Core::Types::String(date_as_string);
    ts=date->GetValueAsInteger();

}

Meow::Core::Time::Years::Years( long date_as_long )
{
    SetMeowClassName("Years");
    ts=date_as_long;

}
Meow::Core::Time::Years::Years()
{
    SetMeowClassName("Years");
    ts=0;
}

Meow::Core::Time::Years::~Years()
{

}

bool Meow::Core::Time::Years::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Years::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Years::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Years::GetValueAsInteger()
{
    return ts*31536000;
}
double Meow::Core::Time::Years::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Years::GetValueAsString()
{
    char temp[255];
    sprintf(temp,"%d",(int)ts);

    std::string reval=temp;
    return reval;
}


void Meow::Core::Time::Years::Increment( )
{
    ts++;
}
void Meow::Core::Time::Years::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Years::Let( Meow::Core::Types::ValueType_ptr other )
{
    ts=other->GetValueAsInteger();
}
void Meow::Core::Time::Years::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
}
void Meow::Core::Time::Years::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
}
void Meow::Core::Time::Years::Divide( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts/other->GetValueAsInteger();
}
void Meow::Core::Time::Years::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
}
bool Meow::Core::Time::Years::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Years::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Years::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Years::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}

