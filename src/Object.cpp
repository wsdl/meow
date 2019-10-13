#include <Object.h>
#include <iostream>
#include <string.h>
#include <Registry.h>

Meow::Object::Object() : iHash(0), __Ready(false)
{
    SetMeowClassName("Object");
    PrivateKey=new Core::Types::Array();
    __Data=new Core::Types::Array();
}

Meow::Object::~Object()
{

}


Meow::Core::Types::Array_ptr Meow::Object::GetUniqueKey( )
{
    return PrivateKey;
}

void Meow::Object::SetUniqueKey( Meow::Core::Types::Array_ptr key )
{
    PrivateKey=key;
}

unsigned long Meow::Object::GetHashCode(  )
{
    if ( iHash!=0 )
        return iHash;

    std::string retval="";
    for ( int i=0; i<PrivateKey->Count(); i++ )
    {
        Core::Types::String_ptr member=(Core::Types::String_ptr)PrivateKey->Get(i);
        if ( ! member.IsNull() )
            retval=retval + "/" + member->GetValue();
        else
            retval=retval + "/";
    }
    const char * str=retval.c_str();
    unsigned long hash=0;
    int c;
    while ( c= *str++ )
        hash = c + (hash<<6)+(hash<<16)-hash;
    iHash=hash;
    return hash;
}

std::string Meow::Object::ToString()
{
	std::string retval=GetMeowClassName();
    retval.append("@");
    return retval + Registry::CompileKey(GetUniqueKey());
}

void Meow::Object::__Destroy( )
{
}

bool Meow::Object::Equals( Object_ptr other )
{
    return other.getPointer()==this;
}
void Meow::Object::SetVariable( std::string varname, Core::ptr data ) throw (Exceptions::Security)
{
	Core::Log::GetLog()->Information("Meow::Object::SetVariable","ss","Calling set:",varname.c_str());
	__Data->Set(varname,data);
}
Meow::Core::ptr Meow::Object::GetVariable( std::string varname )  throw (Exceptions::Security)
{
	return __Data->Get(varname);
}
bool Meow::Object::ExistsVariable( std::string varname )  throw (Exceptions::Security)
{
	return __Data->IsSet(varname);
}
Meow::Core::Types::Stack_ptr Meow::Object::GetVariableKeys()  throw (Exceptions::Security)
{
	return __Data->GetKeys();
}

Meow::Core::Log_ptr Meow::Object::GetLog()
{
    return Meow::Core::Log::GetLog();
}
Meow::Core::ptr Meow::Object::ExecuteMethod( std::string method, Core::Types::Array_ptr parameters)  throw (Exceptions::NotFound,Exceptions::Rollback,Exceptions::Terminate)
{
	Core::ptr retval;
	return retval;
}
bool Meow::Object::GetReady()
{
	return __Ready;
}
void Meow::Object::IsReady()
{
	__Ready=true;
}
