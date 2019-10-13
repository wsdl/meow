// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/Integer.h>
#include <sstream>
#include <stdlib.h>
#include <Core/Log.h>
#ifdef WIN32
	#include <windows.h>
	#include <time.h>
#else
	#include <time.h>
#endif


Meow::Core::Types::Integer_ptr Meow::Core::Types::Integer::GetRandomNumber()
{
    srand( time(NULL) );
    Integer_ptr retval=new Integer(rand());
    return retval;
}
Meow::Core::Types::Integer::Integer( ) :
    Content(0)
{
    SetMeowClassName("Integer");
}
Meow::Core::Types::Integer::Integer( std::string value )
{
    SetMeowClassName("Integer");
    Content=atol(value.c_str());
}
Meow::Core::Types::Integer::Integer( int value )
{
    SetMeowClassName("Integer");
    Content=value;
}
Meow::Core::Types::Integer::~Integer( )
{
}
void Meow::Core::Types::Integer::SetValue( long item )
{
    Content=item;
}
long Meow::Core::Types::Integer::GetValue( )
{
    return Content;
}
std::string Meow::Core::Types::Integer::ToString()
{
    stringstream stringsink;

    stringsink << Content;
    return stringsink.str();
}

void Meow::Core::Types::Integer::Increment()
{
    Content++;
}

void Meow::Core::Types::Integer::Decrement()
{
    Content--;
}

bool Meow::Core::Types::Integer::IsNumeric()
{
    return true;
}
bool Meow::Core::Types::Integer::CouldBeFloat()
{
    return true;
}
bool Meow::Core::Types::Integer::CouldBeInteger()
{
    return true;
}
long Meow::Core::Types::Integer::GetValueAsInteger()
{
    return Content;
}
double Meow::Core::Types::Integer::GetValueAsFloat()
{
    return (double)Content;
}

bool  Meow::Core::Types::Integer::GetValueAsBoolean()
{
    return (bool)Content;
}
void      * Meow::Core::Types::Integer::GetValueAsVoid()
{
    return &Content;
}
long  Meow::Core::Types::Integer::GetVoidLength()
{
    return sizeof(long);
}

std::string Meow::Core::Types::Integer::GetValueAsString()
{
    return ToString();
}

void 		Meow::Core::Types::Integer::Append( Meow::Core::Types::ValueType_ptr other )
{

}


void		Meow::Core::Types::Integer::Let( Meow::Core::Types::ValueType_ptr other )
{
    Content=other->GetValueAsInteger();
}
void		Meow::Core::Types::Integer::Repeat( int times )
{
    std::string val=GetValueAsString();
    std::string outcome="";
    for ( int i=0; i<times; i++ )
        outcome.append(val);
    Content=atol(outcome.c_str());

}
void 		Meow::Core::Types::Integer::Add( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content+other->GetValueAsInteger();
}
void		Meow::Core::Types::Integer::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content-other->GetValueAsInteger();
}
void		Meow::Core::Types::Integer::Divide( Meow::Core::Types::ValueType_ptr other )
{
	if ( other->GetValueAsInteger()==0 )
	{
		Exceptions::DivideByZero e;
		throw e;
	}
    Content=Content/other->GetValueAsInteger();
}
void		Meow::Core::Types::Integer::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content*other->GetValueAsInteger();
}
void		Meow::Core::Types::Integer::Negate( )
{
    Content=0-Content;
}
void		Meow::Core::Types::Integer::Modulus( Meow::Core::Types::ValueType_ptr other )
{
    Content=Content%other->GetValueAsInteger();
}
bool		Meow::Core::Types::Integer::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return Content>other->GetValueAsInteger();
}
bool		Meow::Core::Types::Integer::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return Content<other->GetValueAsInteger();
}
bool		Meow::Core::Types::Integer::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return Content==other->GetValueAsInteger();
}
bool		Meow::Core::Types::Integer::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return Content!=other->GetValueAsInteger();
}
unsigned long Meow::Core::Types::Integer::Count( )
{
    return GetValueAsString().length();
}
