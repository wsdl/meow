#include <Persistence/Drivers/Postgresql/PGSQLDriver.h>
#include <Persistence/Drivers/Postgresql/postgreslib.h>
#include <Core/Log.h>

Meow::Persistence::DatabaseDriver_ptr GetInstance( std::string drivername )  throw (Meow::Exceptions::NotFound)
{
    Meow::Persistence::DatabaseDriver_ptr driver;
    if ( !drivername.compare("postgres") )
        driver=new Meow::Persistence::Drivers::PGSQLDriver();
    else
    {
        std::string message=drivername + " driver not found in postgreslib";
        Meow::Core::Log::GetLog()->Sequel("postgreslib::GetInstance","s",message.c_str());
        Meow::Exceptions::NotFound nf;
        throw nf;
    }
    return driver;
}

