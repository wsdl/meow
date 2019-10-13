#include <Persistence/Drivers/SQLite/SQLITEDriver.h>
#include <Persistence/Drivers/SQLite/sqlitelib.h>
#include <Core/Log.h>

Meow::Persistence::DatabaseDriver_ptr GetInstance( std::string drivername )  throw (Meow::Exceptions::NotFound)
{
    Meow::Persistence::DatabaseDriver_ptr driver;
    if ( !drivername.compare("sqlite") )
        driver=new Meow::Persistence::Drivers::SQLITEDriver();
    else
    {
        std::string message=drivername + " driver not found in sqlite";
        Meow::Core::Log::GetLog()->Sequel("sqlitelib::GetInstance","s",message.c_str());
        Meow::Exceptions::NotFound nf;
        throw nf;
    }
    return driver;
}

