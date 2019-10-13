#include <Security/GroupMember.h>
#include <Exceptions/Security.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Boolean.h>
#include <Security/User.h>
#include <Registry.h>

Meow::Security::GroupMemberFactory::GroupMemberFactory()
{
    SetMeowClassName("Security/GroupMemberFactory");
}

Meow::Security::GroupMemberFactory::~GroupMemberFactory()
{
}

Meow::LifecycleObject_ptr Meow::Security::GroupMemberFactory::GetInstance( std::string service ) 
{
    Meow::LifecycleObject_ptr retval=new GroupMember();
    return retval;
}

Meow::Security::GroupMember::GroupMember()
{
    SetMeowClassName("Security/GroupMember");

}
Meow::Security::GroupMember::~GroupMember()
{
}
std::string Meow::Security::GroupMember::GetName()
{
    Core::Types::ValueType_ptr value=(Core::Types::ValueType_ptr)GetUniqueKey()->Get(0);
    return value->GetValueAsString();
}
Meow::Security::User_ptr Meow::Security::GroupMember::GetUser()
{
	Meow::Security::User_ptr value=(Meow::Security::User_ptr)GetUniqueKey()->Get(1);
	return value;
}
