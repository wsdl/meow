// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/Void.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>

Meow::Core::Types::Void::Void( ) :
    Content(NULL),
    ContentLength(0)
{
    SetMeowClassName("Void");
}

Meow::Core::Types::Void::Void( std::string value )
{
    SetMeowClassName("Void");
    ContentLength=value.length()+1;
    Content=(char*)malloc(ContentLength);
    memcpy(Content,value.c_str(),ContentLength);
}

Meow::Core::Types::Void::Void( void * vptr, unsigned long length )
{
    SetMeowClassName("Void");
    ContentLength=length;
    Content=(char*)malloc(ContentLength);
    memcpy(Content,vptr,ContentLength);
}

Meow::Core::Types::Void::~Void( )
{
    if ( Content!=NULL )
        free(Content);
}

void Meow::Core::Types::Void::SetValue( void * vptr, unsigned long length )
{
    if ( Content!=NULL )
        free(Content);
    Content=(char*)malloc(length);
    memcpy(Content,vptr,length);
}

void * Meow::Core::Types::Void::GetValue( )
{
    return Content;
}

std::string Meow::Core::Types::Void::ToString()
{
    std::string retval=(char*)Content;
    return retval;
}
bool Meow::Core::Types::Void::IsNumeric()
{
    if ( ContentLength==4 || ContentLength==8 )
        return true;
    return false;
}
bool Meow::Core::Types::Void::CouldBeFloat()
{
    return IsNumeric();
}
bool Meow::Core::Types::Void::CouldBeInteger()
{
    return IsNumeric();
}
long Meow::Core::Types::Void::GetValueAsInteger()
{
    long retval=0;
    if ( ContentLength==4 )
        memcpy(&retval,Content,4);
    else if ( ContentLength==8 )
        memcpy(&retval,Content,8);
    return retval;
}
double Meow::Core::Types::Void::GetValueAsFloat()
{
    double retval=0.0;
    if ( ContentLength==4 )
        memcpy(&retval,Content,4);
    else if ( ContentLength==8 )
        memcpy(&retval,Content,8);
    return retval;
}
std::string Meow::Core::Types::Void::GetValueAsString()
{
    if ( Content[ContentLength]=='\0' )
	return (char*)Content;

    return ToString();
}
bool Meow::Core::Types::Void::GetValueAsBoolean()
{
    bool retval=false;
    if ( ContentLength==sizeof(bool) )
        memcpy(&retval,Content,sizeof(bool));

    return retval;
}
void      * Meow::Core::Types::Void::GetValueAsVoid()
{
    return Content;
}
long Meow::Core::Types::Void::GetVoidLength()
{
    return ContentLength;
}
void Meow::Core::Types::Void::Append( ValueType_ptr other )
{
    unsigned long TotalLength=other->GetVoidLength()+ContentLength;
    Content=(char*)realloc((void*)Content,(size_t)TotalLength);
    char * offset=&Content[ContentLength];
    memcpy(offset,other->GetValueAsVoid(),other->GetVoidLength());
    ContentLength=TotalLength;
}
void Meow::Core::Types::Void::Let( ValueType_ptr other )
{
    if ( Content!=NULL )
        free(Content);
    Content=(char*)malloc(other->GetVoidLength());
    memcpy(Content,other->GetValueAsVoid(),other->GetVoidLength());
}
void Meow::Core::Types::Void::Repeat( int times )
{
    if ( times==0 )
    {
        Content=NULL;
        ContentLength=0;
    }
    else
    {
        char * Buffer;
        Buffer=(char*)malloc(ContentLength*times);
        char * ptr;
        for ( int i=0; i<times; i++ )
        {
            unsigned long pos=i*ContentLength;
            ptr=&Buffer[pos];
            memcpy(ptr,Content,ContentLength);
        }
        free(Content);
        Content=Buffer;
        ContentLength=ContentLength*times;
    }
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::Void::Left( )
{
    Meow::Core::Types::ValueType_ptr retval;

    if ( ContentLength>1)
    {
        char * left_buffer=(char*)malloc(1);
        memcpy(left_buffer,Content,1);

        char * right_buffer=(char*)malloc(ContentLength-1);
        memcpy(right_buffer,&Content[1],ContentLength-1);
        free(Content);
        Content=right_buffer;
        ContentLength--;
        Void * temp=new Void(left_buffer,1);
        retval=(ValueType_ptr)temp;
    }
    else if ( ContentLength==1 )
    {
        Void_ptr temp=new Void();
        temp->Content=Content;
        temp->ContentLength=ContentLength;
        Content=NULL;
        ContentLength=0;
        retval=(ValueType_ptr)temp;
    }

    return retval;
//	Content << 1;
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::Void::Right( )
{
    Meow::Core::Types::ValueType_ptr retval;

    if ( ContentLength>1)
    {
        char * right_buffer=(char*)malloc(1);
        memcpy(right_buffer,&Content[ContentLength-1],1);

        char * left_buffer=(char*)malloc(ContentLength-1);
        memcpy(left_buffer,Content,ContentLength-1);
        free(Content);
        Content=left_buffer;
        ContentLength--;
        Void * temp=new Void(right_buffer,1);
        retval=(ValueType_ptr)temp;
    }
    else if ( ContentLength==1 )
    {
        Void_ptr temp=new Void();
        temp->Content=Content;
        temp->ContentLength=ContentLength;
        Content=NULL;
        ContentLength=0;
        retval=(ValueType_ptr)temp;
    }

    return retval;

}
unsigned long Meow::Core::Types::Void::Count( )
{
    return ContentLength;
}
