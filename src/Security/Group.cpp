#include <Security/Group.h>
#include <Security/GroupMember.h>
#include <Exceptions/Security.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Boolean.h>
#include <Security/User.h>
#include <Registry.h>

Meow::Security::GroupFactory::GroupFactory()
{
    SetMeowClassName("Security/GroupFactory");
}

Meow::Security::GroupFactory::~GroupFactory()
{
}

Meow::LifecycleObject_ptr Meow::Security::GroupFactory::GetInstance( std::string service ) throw (Meow::Exceptions::NotFound)
{
    Meow::LifecycleObject_ptr retval=new Group();
    return retval;
}

Meow::Security::Group::Group()
{
    SetMeowClassName("Security/Group");

}
Meow::Security::Group::~Group()
{
}
std::string Meow::Security::Group::GetName()
{
    Core::Types::ValueType_ptr value=(Core::Types::ValueType_ptr)GetUniqueKey()->Get(0);
    return value->GetValueAsString();
}
std::string Meow::Security::Group::GetDescription()
{
	return iDescription;
}
void Meow::Security::Group::SetDescription( std::string desc )
{
	iDescription=desc;
}
bool Meow::Security::Group::IsAuthorised( Security::User_ptr user )
{
	bool retval=false;
	try
	{
		Core::Types::Array_ptr key=new Core::Types::Array();
		key->Set(0,new Core::Types::String(GetName()));
		key->Set(1,(Core::ptr)user);
		Security::GroupMember_ptr member=(Security::GroupMember_ptr)Registry::GetRegistry()->FindMe("Security/GroupMember",key);
		retval=true;
	} catch ( Exceptions::NotFound &nf )
	{
		// Not authorised, nothing to do lets not even log it.
	}
	return retval;
}
