#include <Persistence/Drivers/MySQL/mysqlib.h>
#include <Persistence/Drivers/MySQL/MYSQLDriver.h>
#include <Core/Log.h>

Meow::Persistence::DatabaseDriver_ptr GetInstance( std::string drivername )  throw (Meow::Exceptions::NotFound)
{
    Meow::Persistence::DatabaseDriver_ptr driver;
    if ( !drivername.compare("mysql") )
        driver=new Meow::Persistence::Drivers::MYSQLDriver();
    else
    {
        Meow::Core::Log::GetLog()->Sequel("mysqllib::GetInstance","ss",drivername.c_str()," not found in mysqllib");
        Meow::Exceptions::NotFound nf;
        throw nf;
    }
    return driver;
}

