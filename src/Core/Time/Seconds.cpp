// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetMeowClassName and GetMeowClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#include <Core/Time/Seconds.h>
#include <Core/Types/String.h>
#include <stdio.h>

Meow::Core::Time::Seconds::Seconds( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Seconds");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Seconds::Seconds( std::string date_as_string )
{
    SetMeowClassName("Seconds");
    Meow::Core::Types::String_ptr date=new  Meow::Core::Types::String(date_as_string);
    ts=date->GetValueAsInteger();

}

Meow::Core::Time::Seconds::Seconds( long date_as_long )
{
    SetMeowClassName("Seconds");
    ts=date_as_long;

}
Meow::Core::Time::Seconds::Seconds()
{
    SetMeowClassName("Seconds");
    ts=0;
}

Meow::Core::Time::Seconds::~Seconds()
{

}
bool Meow::Core::Time::Seconds::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Seconds::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Seconds::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Seconds::GetValueAsInteger()
{
    return ts;
}
double Meow::Core::Time::Seconds::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Seconds::GetValueAsString()
{
    char temp[255];
    sprintf(temp,"%d",(int)ts);

    std::string reval=temp;
    return reval;
}


void Meow::Core::Time::Seconds::Increment( )
{
    ts++;
}
void Meow::Core::Time::Seconds::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Seconds::Let( Meow::Core::Types::ValueType_ptr other )
{
    ts=other->GetValueAsInteger();
}
void Meow::Core::Time::Seconds::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
}
void Meow::Core::Time::Seconds::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
}
void Meow::Core::Time::Seconds::Divide( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts/other->GetValueAsInteger();
}
void Meow::Core::Time::Seconds::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
}
bool Meow::Core::Time::Seconds::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Seconds::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Seconds::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Seconds::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}

