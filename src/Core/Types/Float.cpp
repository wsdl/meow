// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/Float.h>
#include <sstream>
#include <stdlib.h>
#include <Core/Log.h>
#include <cmath>

Meow::Core::Types::Float::Float( ) :
    Content(0.0)
{
    SetMeowClassName("Float");
}

Meow::Core::Types::Float::Float( float value )
{
    Content=value;
}

Meow::Core::Types::Float::Float( std::string value )
{
    Content=atof(value.c_str());
}


Meow::Core::Types::Float::~Float( )
{
}

void Meow::Core::Types::Float::SetValue( double item )
{
    Content=item;
}

double Meow::Core::Types::Float::GetValue( )
{
    return Content;
}

std::string Meow::Core::Types::Float::ToString( )
{
    stringstream stringsink;

    stringsink << Content;
    return stringsink.str();
}


bool Meow::Core::Types::Float::IsNumeric()
{
    return true;
}
bool Meow::Core::Types::Float::CouldBeFloat()
{
    return true;
}
bool Meow::Core::Types::Float::CouldBeInteger()
{
    return false;
}
long Meow::Core::Types::Float::GetValueAsInteger()
{
    return (long)Content;
}
double Meow::Core::Types::Float::GetValueAsFloat()
{
    return (double)Content;
}
std::string Meow::Core::Types::Float::GetValueAsString()
{
    return ToString();
}

bool Meow::Core::Types::Float::GetValueAsBoolean()
{
    return (bool)Content;
}
void * Meow::Core::Types::Float::GetValueAsVoid()
{
    return &Content;
}
long   Meow::Core::Types::Float::GetVoidLength()
{
    return sizeof(double);
}

void 	Meow::Core::Types::Float::Increment( )
{
    Content=Content+1.0;
}
void	Meow::Core::Types::Float::Decrement( )
{
    Content=Content-1.0;
}
void	Meow::Core::Types::Float::Let( Meow::Core::Types::ValueType_ptr other )
{
    Content=other->GetValueAsFloat();
}
void 	Meow::Core::Types::Float::Add( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content+other->GetValueAsFloat();
}
void	Meow::Core::Types::Float::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content-other->GetValueAsFloat();
}
void	Meow::Core::Types::Float::Divide( Meow::Core::Types::ValueType_ptr other ) 
{
	if ( other->GetValueAsFloat()==0.0 )
	{
		Exceptions::DivideByZero e;
		throw e;
	}
    Content=Content/other->GetValueAsFloat();
}
void	Meow::Core::Types::Float::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content*other->GetValueAsFloat();
}
void	Meow::Core::Types::Float::Negate( )
{
    Content=0.0-GetValueAsFloat();
}
bool	Meow::Core::Types::Float::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return Content>other->GetValueAsFloat();
}
bool	Meow::Core::Types::Float::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return Content<other->GetValueAsFloat();
}
bool	Meow::Core::Types::Float::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return Content==other->GetValueAsFloat();
}
bool	Meow::Core::Types::Float::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return Content!=other->GetValueAsFloat();
}
unsigned long Meow::Core::Types::Float::Count( )
{
    return 4;
}
void    Meow::Core::Types::Float::RoundUp( )
{
    Content=std::ceil(Content);
}
void    Meow::Core::Types::Float::RoundDown( )
{
    Content=std::floor(Content);
}
