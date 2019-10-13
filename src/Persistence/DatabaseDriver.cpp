#include <Persistence/DatabaseDriver.h>
#include <Persistence/Database.h>
#include <Core/Types/Array.h>

Meow::Persistence::DatabaseDriver::DatabaseDriver() :
    isunitemporal(false)
{
    SetMeowClassName("Persistence/DatabaseDriver");
}
Meow::Persistence::DatabaseDriver::~DatabaseDriver()
{
}
void Meow::Persistence::DatabaseDriver::SetCaller( Meow::Persistence::Database_ptr caller )
{
    iCaller=caller;
}

Meow::Core::Types::Array_ptr     Meow::Persistence::DatabaseDriver::FetchRowFromResult( void * descriptor, int guideRow )
{
	Meow::Core::Types::Array_ptr retval;
	return retval;
}
void Meow::Persistence::DatabaseDriver::FreeResult( void * descriptor )
{
	if ( descriptor != NULL )
			free(descriptor);
}

void Meow::Persistence::DatabaseDriver::Action(  std::string classname, SchemaOperation_ptr operation )
				throw(Meow::Persistence::Exceptions::SQLError,
					  Meow::Persistence::Exceptions::IPCError,
					  Meow::Exceptions::Rollback,
					  Meow::Exceptions::Terminate)
{
}

void Meow::Persistence::DatabaseDriver::Create(  std::string classname, Core::Types::Array_ptr operationList ) 
					throw(Meow::Persistence::Exceptions::SQLError,
						  Meow::Persistence::Exceptions::IPCError,
						  Meow::Exceptions::Rollback,
						  Meow::Exceptions::Terminate)
{
}
