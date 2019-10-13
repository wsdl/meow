// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.

#include <Core/NameValueStore.h>
#include <Core/Types/Array.h>

Meow::Core::NameValueStore::NameValueStore() : iDriver(NULL)
{
    SetMeowClassName("NameValueStore");
    iDriver=new Meow::Core::Types::Array();

}

Meow::Core::NameValueStore::~NameValueStore()
{
    delete (Meow::Core::Types::Array*)iDriver;
}

void Meow::Core::NameValueStore::SetValue(
    std::string domain,
    std::string name,
    Meow::Core::Types::ValueType_ptr value )
{
	Meow::Core::Types::Array * driver=(Meow::Core::Types::Array*)iDriver;
    Meow::Core::Types::Array_ptr domainMap=new Meow::Core::Types::Array();

    if ( driver->IsSet(domain) )
        domainMap=(Meow::Core::Types::Array_ptr)driver->Get(domain);
    else
        driver->Set(domain,(ptr)domainMap);

    domainMap->Set(name,(ptr)value);
}

Meow::Core::Types::ValueType_ptr Meow::Core::NameValueStore::GetValue(
    std::string domain,
    std::string name )
{
    Meow::Core::Types::ValueType_ptr value;

    Meow::Core::Types::Array * driver=(Meow::Core::Types::Array*)iDriver;
    if ( driver->IsSet(domain) )
    {
    	Meow::Core::Types::Array_ptr domainMap=(Meow::Core::Types::Array_ptr)driver->Get(domain);
        value=(Meow::Core::Types::ValueType_ptr)domainMap->Get(name);
    }
    return value;
}

void Meow::Core::NameValueStore::UnsetValue(
    std::string domain,
    std::string name )
{
	Meow::Core::Types::Array * driver=(Meow::Core::Types::Array*)iDriver;
    if ( driver->IsSet(domain) )
    {
    	Meow::Core::Types::Array_ptr domainMap=(Meow::Core::Types::Array_ptr)driver->Get(domain);
        domainMap->Unset(name);
        if ( domainMap->Count()==0 )
            driver->Unset(domain);
    }
}

bool Meow::Core::NameValueStore::ExistsValue(
    std::string domain,
    std::string name )
{
    bool retval=false;
    Meow::Core::Types::Array * driver=(Meow::Core::Types::Array*)iDriver;
    if ( driver->IsSet(domain) )
    {
    	Meow::Core::Types::Array_ptr domainMap=(Meow::Core::Types::Array_ptr)driver->Get(domain);
        retval=domainMap->IsSet(name);
    }
    return retval;
}
Meow::Core::Types::ValueType_ptr Meow::Core::NameValueStore::OverrideValue( std::string domain, std::string name, Core::Types::ValueType_ptr currentValue )
{
	Core::Types::ValueType_ptr retval=currentValue;
	if ( ExistsValue(domain,name) )
	{
		retval=GetValue(domain,name);
	}
	return retval;
}