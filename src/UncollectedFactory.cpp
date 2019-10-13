#include <UncollectedFactory.h>
Meow::UncollectedFactory::UncollectedFactory()
{
    SetMeowClassName("UncollectedFactory");
}

Meow::UncollectedFactory::~UncollectedFactory()
{
}
Meow::LifecycleObject_ptr Meow::UncollectedFactory::FindByPrimaryKey( Core::Types::Array_ptr key )
{
    return GetInstance("");
}

Meow::LifecycleObject_ptr
Meow::UncollectedFactory::CreateByPrimaryKey( Core::Types::Array_ptr key,
        std::string service )
{
    return GetInstance(service);
}

void
Meow::UncollectedFactory::DestroyByPrimaryKey( Core::Types::Array_ptr key )
{
}
