#include <Core/Time/Time.h>
#include <Core/Types/String.h>
#include <stdio.h>

Meow::Core::Time::Time::Time( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Time");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Time::Time( std::string date_as_string )
{
    SetMeowClassName("Time");


    Meow::Core::Types::String_ptr time_value=new Meow::Core::Types::String(date_as_string);
    Meow::Core::Types::Array_ptr elements=time_value->Explode(":");
    ts=0;

    switch ( elements->Count() )
    {
    case 1:
    {
        // minutes
    	Meow::Core::Types::String_ptr minutes=(Meow::Core::Types::String_ptr)elements->Get(0);
        ts=minutes->GetValueAsInteger()*60;
        break;
    }
    case 2:
    {
    	Meow::Core::Types::String_ptr hours=(Meow::Core::Types::String_ptr)elements->Get(0);
    	Meow::Core::Types::String_ptr minutes=(Meow::Core::Types::String_ptr)elements->Get(1);
        ts=minutes->GetValueAsInteger()*60 +
           hours->GetValueAsInteger()*3600;
        break;
    }
    case 3:
    {
    	Meow::Core::Types::String_ptr hours=(Meow::Core::Types::String_ptr)elements->Get(0);
    	Meow::Core::Types::String_ptr minutes=(Meow::Core::Types::String_ptr)elements->Get(1);
    	Meow::Core::Types::String_ptr seconds=(Meow::Core::Types::String_ptr)elements->Get(2);
        ts=minutes->GetValueAsInteger()*60 +
           hours->GetValueAsInteger()*3600 +
           seconds->GetValueAsInteger();
    }
    break;
    }
}

Meow::Core::Time::Time::Time( long date_as_long )
{
    SetMeowClassName("Time");
    ts=date_as_long%86400;

}
Meow::Core::Time::Time::Time()
{
    SetMeowClassName("Time");
    ts=0;
}

Meow::Core::Time::Time::~Time()
{

}

bool Meow::Core::Time::Time::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Time::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Time::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Time::GetValueAsInteger()
{
    return ts;
}
double Meow::Core::Time::Time::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Time::GetValueAsString()
{
    char temp[255];

    if ( ts <60 )
    	sprintf(temp,"%d",(int)ts);
    else if ( ts <3600 )
    {
        unsigned long seconds;
        unsigned long minutes;
        minutes=ts/60;
        seconds=ts%60;

        sprintf(temp,"%d:%d",(int)minutes,(int)seconds);
    }
    else
    {
        unsigned long seconds;
        unsigned long minutes;
        unsigned long hours;
        hours=ts/3600;
        minutes=ts/60;
        seconds=ts%60;

        sprintf(temp,"%d:%d:%d",(int)hours,(int)minutes,(int)seconds);

    }
    std::string reval=temp;
    return reval;
}


void Meow::Core::Time::Time::Increment( )
{
    ts++;
}
void Meow::Core::Time::Time::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Time::Let( Meow::Core::Types::ValueType_ptr other )
{
    if ( other->IsInstanceOf("String") )
    {
    	Meow::Core::Types::String_ptr time_value=(Meow::Core::Types::String_ptr)other;
    	Meow::Core::Types::Array_ptr elements=time_value->Explode(":");
        ts=0;

        switch ( elements->Count() )
        {
        case 1:
        {
            // minutes
        	Meow::Core::Types::String_ptr minutes=(Meow::Core::Types::String_ptr)elements->Get(0);
            ts=minutes->GetValueAsInteger()*60;
            break;
        }
        case 2:
        {
        	Meow::Core::Types::String_ptr hours=(Meow::Core::Types::String_ptr)elements->Get(0);
        	Meow::Core::Types::String_ptr minutes=(Meow::Core::Types::String_ptr)elements->Get(1);
            ts=minutes->GetValueAsInteger()*60 +
               hours->GetValueAsInteger()*3600;
            break;
        }
        case 3:
        {
        	Meow::Core::Types::String_ptr hours=(Meow::Core::Types::String_ptr)elements->Get(0);
        	Meow::Core::Types::String_ptr minutes=(Meow::Core::Types::String_ptr)elements->Get(1);
        	Meow::Core::Types::String_ptr seconds=(Meow::Core::Types::String_ptr)elements->Get(2);
            ts=minutes->GetValueAsInteger()*60 +
               hours->GetValueAsInteger()*3600 +
               seconds->GetValueAsInteger();
            break;
        }
        }
    }
    else
        ts=other->GetValueAsInteger()%86400;
}
void Meow::Core::Time::Time::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
    ts=ts%86400;
}
void Meow::Core::Time::Time::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
    ts=ts%86400;
}
void Meow::Core::Time::Time::Divide( Meow::Core::Types::ValueType_ptr other ) 
{
    ts=ts/other->GetValueAsInteger();
    ts=ts%86400;
}
void Meow::Core::Time::Time::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
    ts=ts%86400;
}
bool Meow::Core::Time::Time::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Time::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Time::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Time::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}

