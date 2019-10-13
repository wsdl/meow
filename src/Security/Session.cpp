#include <Security/Session.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Array.h>
#include <Core/Types/Void.h>
#include <Core/Time/Date.h>
#include <Registry.h>

Meow::Security::SessionFactory::SessionFactory()
{
    SetMeowClassName("Security/SessionFactory");

    AddCompoundKeyField("ID", 
		Factory::String, 
		true,
		"Counter",
		"Counter ID",
		true);
    AddAttributeField("created",
		Factory::Date,
		new Core::Time::Date(),
		0,
		"Created",
		"Guests",
		"Guests",
		"Created",
		true);
    AddAttributeField("lasttouched",
		Factory::Date,
		new Core::Time::Date(),
		0,
		"LastTouched",
		"Guests",
		"Guests",
		"Last Used",
		true);
    AddAttributeField("remoteaddress",
		Factory::String,
		new Core::Types::String(),
		0,
		"RemoteAddress",
		"Guests",
		"Guests",
		"Remote Address",
		true);
    AddAttributeField("sessiondata",
		Factory::Array,
		new Core::Types::Array(),
		0,
		"SessionData",
		"Guests",
		"Guests",
		"Session Data",
		true);
	
}
Meow::Security::SessionFactory::~SessionFactory()
{
}
Meow::LifecycleObject_ptr Meow::Security::SessionFactory::GetInstance( std::string service )  
{
    return new Session();
}
std::string Meow::Security::SessionFactory::GetFriendlyName()
{
	return "Default Session Handler";
}
bool Meow::Security::SessionFactory::IsPublicApplication()
{
	return false;
}
void Meow::Security::SessionFactory::CanRead()
{
}
void Meow::Security::SessionFactory::CanWrite()
{
}
void Meow::Security::SessionFactory::CanCreate()
{
}
void Meow::Security::SessionFactory::CanDelete()
{
}
Meow::Security::Session::Session()
{
    SetMeowClassName("Security/Session");
	iNotifiable=false;
}
Meow::Security::Session::~Session()
{
}

