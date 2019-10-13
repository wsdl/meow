#include <Security/User.h>
#include <Registry.h>
#include <inc/configuration.h>

Meow::Security::UserFactory::UserFactory()
{
	SetMeowClassName("Security/UserFactory");

}
Meow::Security::UserFactory::~UserFactory()
{
	
}
Meow::LifecycleObject_ptr Meow::Security::UserFactory::FindByPrimaryKey( Core::Types::Array_ptr key )

{
	Factory_ptr factory=GetDelegateFactory();
	if ( !factory.IsNull() )
		return factory->FindByPrimaryKey(key);
	return Factory::FindByPrimaryKey(key);
}
Meow::LifecycleObject_ptr Meow::Security::UserFactory::FindByPrimaryKeyOrRestore( Core::Types::Array_ptr key, Core::Types::Array_ptr attributes )

{
	Factory_ptr factory=GetDelegateFactory();
	if ( !factory.IsNull() )
		return factory->FindByPrimaryKeyOrRestore(key,attributes);
	return Factory::FindByPrimaryKeyOrRestore(key,attributes);
}
Meow::LifecycleObject_ptr Meow::Security::UserFactory::CreateByPrimaryKey( Core::Types::Array_ptr key, std::string service )

{
	Factory_ptr factory=GetDelegateFactory();
	if ( !factory.IsNull() )
		return factory->CreateByPrimaryKey(key,service);
	return Factory::CreateByPrimaryKey(key,service);

}
void Meow::Security::UserFactory::DestroyByPrimaryKey( Core::Types::Array_ptr key )
{	
	Factory_ptr factory=GetDelegateFactory();
	if ( !factory.IsNull() )
		factory->DestroyByPrimaryKey(key);
	else
		Factory::DestroyByPrimaryKey(key);
}
void Meow::Security::UserFactory::MarkChangedExternally( Core::Types::Array_ptr key )
{
	Factory_ptr factory=GetDelegateFactory();
	if ( !factory.IsNull() )
		factory->MarkChangedExternally(key);
	else
		Factory::MarkChangedExternally(key);

}
Meow::LifecycleObject_ptr Meow::Security::UserFactory::GetInstance( std::string service )
{
	return new Meow::Security::User();
}
Meow::Security::User_ptr Meow::Security::UserFactory::GetAnonymousUser()
{
	Meow::Security::User_ptr retval;
	try
	{
		retval=(Meow::Security::User_ptr)Factory::GetRegistry()->FindMe("Security/User",1,"Anonymous");
	} catch ( Exception &e )
	{
		Core::Log::GetLog()->Security("Meow::Security::UserFactory::GetAnonymousUser","s","Cannot get an actual user for Anonymous User, this will be NULL instead.");
	}
	return retval;
}
void Meow::Security::UserFactory::CanRead()
{
}
Meow::Factory_ptr Meow::Security::UserFactory::GetDelegateFactory( )
{
	if ( delegateFactory.IsNull() )
	{
		std::string delegateFactoryStr=Configuration::GetConfiguration()->Get("security.delegatefactory");
		if ( delegateFactoryStr.length() > 0 )
		{
			try
			{
				delegateFactory=Registry::GetRegistry()->FindFactory(delegateFactoryStr);	
			} catch ( Exceptions::NotFound &nf )
			{
				Core::Log::GetLog()->Warning("Meow::Security::UserFactory::GetDelegateFactory","ss","Could not side load delegate factory, perhaps its plugin has not been loaded yet:",delegateFactoryStr.c_str());
			}
		}
	}		
	return delegateFactory;
}

Meow::Security::User::User()
{
	SetMeowClassName("Security/User");
}
Meow::Security::User::~User()
{
}
std::string Meow::Security::User::GetPublicAlias()
{
	return GetUsername();
}
unsigned long Meow::Security::User::GetUid( )
{
	Core::Types::ValueType_ptr uid=(Core::Types::ValueType_ptr)GetUniqueKey()->Get(0);
	return uid->GetValueAsInteger();
}
void Meow::Security::User::SetUsername( std::string uname )
{
	LifecycleObject::SetVariable("Username",new Core::Types::String(uname));
}
void Meow::Security::User::SetPassword( std::string password )
{
	LifecycleObject::SetVariable("Password",new Core::Types::String(password));
}
void Meow::Security::User::SetEmailAddress( std::string email )
{
	LifecycleObject::SetVariable("EmailAddress",new Core::Types::String(email));
}
void Meow::Security::User::SetContactNumber( std::string phone )
{
	LifecycleObject::SetVariable("ContactNumber",new Core::Types::String(phone));
}
std::string Meow::Security::User::GetPassword( )
{
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)LifecycleObject::GetVariable("Password");
	return retval->GetValueAsString();
}
std::string Meow::Security::User::GetUsername( )
{
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)LifecycleObject::GetVariable("Username");
	return retval->GetValueAsString();
}
std::string Meow::Security::User::GetEmailAddress()
{
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)LifecycleObject::GetVariable("EmailAddress");
	return retval->GetValueAsString();
}
std::string Meow::Security::User::GetContactNumber()
{
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)LifecycleObject::GetVariable("ContactNumber");
	return retval->GetValueAsString();
}
void Meow::Security::User::SetRelatedObjectReference( std::string ref )
{
	iRelatedObjectReference=ref;
}
std::string Meow::Security::User::GetRelatedObjectReference()
{
	return iRelatedObjectReference;
}



