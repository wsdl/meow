#include <Persistence/Drivers/MySQL/MYSQLDriver.h>
#include <Persistence/Drivers/Postgresql/PGSQLDriver.h>
#include <Persistence/Drivers/SQLite/SQLITEDriver.h>
#include <Persistence/SQLResult.h>
#include <Persistence/Database.h>
#include <Persistence/Update.h>
#include <Persistence/Select.h>
#include <Persistence/Where.h>
#include <Core/System/LinkLibrary.h>
#include <Core/Types/Stack.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Queue.h>
#include <Core/Log.h>
#include <Core/Time/Time.h>
#include <Core/Types/Void.h>
#include <Core/Types/String.h>
#include <inc/configuration.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <Registry.h>
#include <Security/User.h>
#include <unistd.h>
#include <Exceptions/Retry.h>
#include <Exceptions/Rollback.h>
#include <Exceptions/Terminate.h>

Meow::Persistence::DatabaseFactory::DatabaseFactory()
{
    SetMeowClassName("Persistence/DatabaseFactory");
    iLibraries=new Core::Types::Array();
    iPreinstalledDrivers=new Core::Types::Array();
}
Meow::Persistence::DatabaseFactory::~DatabaseFactory()
{
}
Meow::LifecycleObject_ptr Meow::Persistence::DatabaseFactory::GetInstance(std::string service) throw(Meow::Exceptions::NotFound)
{
    LifecycleObject_ptr retval=new Meow::Persistence::Database();
    return retval;
}
void Meow::Persistence::DatabaseFactory::__Destroy()
{
}
void Meow::Persistence::DatabaseFactory::Purge()
{
}
void Meow::Persistence::DatabaseFactory::RegisterEvents( )
{
    Core::Types::Array_ptr params=new Core::Types::Array();
    GetRegistry()->Subscribe("OwnerKeyChange",this,params);
}
void Meow::Persistence::DatabaseFactory::OnEvent( std::string event, Meow::Core::Types::Array_ptr parameters )
{

}
Meow::Persistence::DatabaseDriver_ptr Meow::Persistence::DatabaseFactory::LoadDriver( std::string driver ) throw (Meow::Exceptions::NotFound)
{
    Meow::Persistence::DatabaseDriver_ptr driverInstance;
    if ( !iPreinstalledDrivers->IsSet(driver) )
    {
        try
        {
            Core::System::LinkLibrary_ptr DLL;
            Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","ss","Attempting to load driver for:",driver.c_str());
            if ( iLibraries->IsSet(driver) )
            {
            	Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","s","Driver preloaded.");
                DLL=(Core::System::LinkLibrary_ptr)iLibraries->Get(driver);
            }
            else
            {
            	Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","s","Driver needs to be installed.");
                DLL=new Core::System::LinkLibrary();
                if ( Configuration::GetConfiguration()->IsSet(driver + ".library") )
                {

                	std::string driverFromConfiguration=Configuration::GetConfiguration()->Get(driver + ".library");
                	Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","ss","Library is:",driverFromConfiguration.c_str());
                	DLL->Install(driverFromConfiguration);
                	Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","s","Installed.");

                    iLibraries->Set(driver,(Core::ptr)DLL);
                }
                else
                {
                    std::string message="Could not find library for driver in configuration file for:[" + driver + "], use (driver).library=(library)";
                    Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","s",message.c_str());
                    Meow::Exceptions::NotFound nf;
                    throw nf;
                }
            }
            Meow::Persistence::DatabaseDriver_ptr (*init)(std::string);
	    // Extern C isn't going to work on its own, so get the ordinal name from config.
	    std::string ordinalName=Configuration::GetConfiguration()->Get("database.loadordinal");
            // We want to change this back to GetInstance when we figure out the extern "C" problem!
            Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","ss","Looking for ",ordinalName.c_str());
            init=(Meow::Persistence::DatabaseDriver_ptr (*)(std::string))DLL->GetFunction(ordinalName);
            Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","s","Found, now calling init.");
            driverInstance=init(driver);
        } catch ( SystemException &e )
        {
            std::string message="Could not load/resolve driver from library:" + driver;
            Core::Log::GetLog()->Sequel("Meow::Persistence::DatabaseFactory::LoadDriver","s",message.c_str());
            Meow::Exceptions::NotFound nf;
            throw nf;
        }
    }
    else
    {
        driverInstance=(DatabaseDriver_ptr)iPreinstalledDrivers->Get(driver);
    }
    return driverInstance;
}
void Meow::Persistence::DatabaseFactory::PreinstallDriver( std::string driverName, DatabaseDriver_ptr driver )
{
    iPreinstalledDrivers->Set(driverName,(Core::ptr)driver);

}

bool Meow::Persistence::DatabaseFactory::SecondPhasePurge()
{
    return true;
}

void Meow::Persistence::DatabaseFactory::CanRead() throw (Meow::Exceptions::Security)
{
}

Meow::Persistence::Database::Database() :
    alias_writes(0),
    isintransaction(false),
    isconnected(false),
    isunitemporal(false),
    traceresults(false)
{

    SetMeowClassName("Persistence/Database");
    traceresults=true;
}
Meow::Persistence::Database::~Database()
{
    // We don't need to do this, but we might as well.

    // __Destroy();
}
void Meow::Persistence::Database::SetAlias( std::string value )
{
    if ( alias_writes<1 )
    {
        Meow::Core::Types::Array_ptr key=GetUniqueKey();
        Meow::Core::Types::String_ptr alias=(Meow::Core::Types::String_ptr)key->Get(0);
        alias->SetValue(value);
    }
    alias_writes++;
}
std::string Meow::Persistence::Database::GetAlias( )
{
    Meow::Core::Types::Array_ptr key=GetUniqueKey();
    Meow::Core::Types::String_ptr alias=(Meow::Core::Types::String_ptr)key->Get(0);
    return	alias->GetValue();
}
bool Meow::Persistence::Database::GetIsInTransaction( )
{
    return isintransaction;
}
void Meow::Persistence::Database::SetIsInTransaction( bool flag )
{
    isintransaction=flag;
}
void Meow::Persistence::Database::SetIsConnected( bool flag )
{
    isconnected=flag;
}
bool Meow::Persistence::Database::GetIsConnected( )
{
    return isconnected;
}
void Meow::Persistence::Database::Initialise()

{
    if ( iDriver.IsNull( ) )
    {
#ifdef MEOW_PERFORMANCE_ON
	    MEOW_PERFORMANCE_ENTRY(handlePersistenceMethodTimer,"Initialise",PERFORMANCE_SUBSYS_DATA,GetUniqueKey());
#endif
        std::string drivername=GetAlias();
        drivername=Configuration::GetConfiguration()->Get(drivername + ".driver");
        if ( Configuration::GetConfiguration()->IsSet(drivername + ".traceresults") )
            traceresults=true;
        try
        {

            iDriver=((DatabaseFactory_ptr)GetFactory())->LoadDriver( drivername );
            Database_ptr databasePtr=this;
            iDriver->SetCaller(databasePtr);
            iDriver->Initialise();
	    Registry::GetRegistry()->RegisterForTransactionLifecycle( (LifecycleObject_ptr)databasePtr );
        }
        catch ( SystemException &e )
        {
            Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Initialise()","sss",std::string("Unknown Driver [" + drivername + "]").c_str());
        }
#ifdef MEOW_PERFORMANCE_ON
        MEOW_PERFORMANCE_EXIT(handlePersistenceMethodTimer);
#endif
    }
    if ( !iDriver.IsNull() )
    {
        if ( !GetIsConnected() )
            iDriver->Initialise();
    }
    else
    	Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Initialise()","s","No driver");
}
void Meow::Persistence::Database::BeginTransaction()
{
    Initialise();
    if ( !iDriver.IsNull() )
        iDriver->BeginTransaction();
}
void Meow::Persistence::Database::CommitTransaction()
{
    Initialise();
    if ( !iDriver.IsNull() )
        iDriver->CommitTransaction();
}
void Meow::Persistence::Database::RollbackTransaction()
{
    Initialise();
    if ( !iDriver.IsNull() )
        iDriver->RollbackTransaction();
}
std::string Meow::Persistence::Database::Sanitize( std::string data, bool quoteit )
{
    Initialise();
    if ( !iDriver.IsNull() )
        return iDriver->Sanitize(data,quoteit);
    return data;
}

std::string     Meow::Persistence::Database::SanitizeColumnName( std::string data, bool notas )
{
    Initialise();
    if ( !iDriver.IsNull() )
        return iDriver->SanitizeColumnName(data,notas);
    return data;
}
std::string Meow::Persistence::Database::SanitizeTableName( std::string data )
{
    Initialise();
    if ( !iDriver.IsNull() )
        return iDriver->SanitizeTableName(data);
    return data;
}
std::string	Meow::Persistence::Database::CastToString( std::string data )
{
	Initialise();
	if ( !iDriver.IsNull() )
		return iDriver->CastToString(data);
	return data;
}
std::string	Meow::Persistence::Database::CastToInteger( std::string data )
{
	Initialise();
	if ( !iDriver.IsNull() )
		return iDriver->CastToInteger(data);
	return data;
}


Meow::Persistence::SQLResult_ptr
Meow::Persistence::Database::ObjectQuery( Query_ptr sql)
throw(Meow::Persistence::Exceptions::SQLError,
      Meow::Persistence::Exceptions::IPCError,
      Meow::Exceptions::Rollback,
      Meow::Exceptions::Terminate)
{
    
    Initialise();
#ifdef MEOW_PERFORMANCE_ON
    MEOW_PERFORMANCE_ENTRY(handlePersistenceMethodTimer,"ObjectQuery",PERFORMANCE_SUBSYS_DATA,GetUniqueKey());
#endif
    Meow::Persistence::SQLResult_ptr retval=new SQLResult();
    if ( !iDriver.IsNull() )
    {
        if ( Configuration::GetConfiguration()->IsSet("retry.policy.repeats") )
        {
            bool success=false;
            int  tries=0;
            int  limit=10;
            int  sleepfor=100;
            Core::Types::ValueType_ptr retry_policy_repeats=new Core::Types::String(Configuration::GetConfiguration()->Get("retry.policy.repeats"));
            limit=retry_policy_repeats->GetValueAsInteger();
            if ( Configuration::GetConfiguration()->IsSet("retry.policy.sleepms") )
            {
            	Core::Types:: ValueType_ptr retry_policy_sleepms=new Core::Types::String(Configuration::GetConfiguration()->Get("retry.policy.sleepms"));
                sleepfor=retry_policy_sleepms->GetValueAsInteger();
            }
            while(!success)
            {
                try
                {
                    if ( tries >= limit )
                    {
                    	Meow::Exceptions::Rollback rb;
                    	rb.SetMessage("Retry limit reached");
                    	throw rb;
                    }
                    retval=iDriver->ObjectQuery(sql);
                    success=true;
                } catch ( Meow::Exceptions::Retry &retry )
                {
                    tries++;
                    Pause(sleepfor);
                }
            }
        }
        else
        {
            try
            {
                retval=iDriver->ObjectQuery(sql);
            } catch ( Meow::Exceptions::Retry &retry )
            {
                Meow::Exceptions::Rollback rollback;
                rollback.SetMessage(retry.Diagnostic());
                throw rollback;
            }
        }
    }
#ifdef MEOW_PERFORMANCE_ON
    MEOW_PERFORMANCE_EXIT(handlePersistenceMethodTimer);
#endif
    return retval;
}

std::string Meow::Persistence::Database::ConvertTypeForComparison( std::string typedefof )
{
    Initialise();
    if ( !iDriver.IsNull() )
        iDriver->ConvertTypeForComparison(typedefof);
    return typedefof;
}
std::string Meow::Persistence::Database::ConvertClass( std::string classname )
{
	std::string retval=classname;
    Initialise();
	if ( !iDriver.IsNull() )
		retval=iDriver->ConvertClass(classname);
	return retval;
	
}
std::string Meow::Persistence::Database::ConvertAttribute( std::string attribute )
{
	std::string retval=attribute;
    Initialise();
	if ( !iDriver.IsNull() )
		retval=iDriver->ConvertAttribute(attribute);
	return retval;  
}
void Meow::Persistence::Database::Create(  std::string classname, Core::Types::Array_ptr operationList ) 
				throw(Meow::Persistence::Exceptions::SQLError,
					  Meow::Persistence::Exceptions::IPCError,
					  Meow::Exceptions::Rollback,
					  Meow::Exceptions::Terminate)
{
	Initialise();
	if ( !iDriver.IsNull() )
		iDriver->Create(classname,operationList);
	else
	{
		Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Create","s","Could not get a driver descriptor to fulfil an Creation request.");
	}
}
void Meow::Persistence::Database::Action(  std::string classname,  SchemaOperation_ptr operation ) 
				throw(Meow::Persistence::Exceptions::SQLError,
					  Meow::Persistence::Exceptions::IPCError,
					  Meow::Exceptions::Rollback,
					  Meow::Exceptions::Terminate)
{
    Initialise();
	if ( !iDriver.IsNull() )
		iDriver->Action(classname,operation);
	else
	{
		Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Action","s","Could not get a driver descriptor to fulfil an Action request.");
	}
}
