#include <Persistence/Drivers/SQLite/SQLITEDriver.h>
#include <Persistence/Query.h>
#include <Persistence/Where.h>
#include <Persistence/Update.h>
#include <Core/Types/Queue.h>
#include <Core/Types/Void.h>
#include <Registry.h>
#include <Security/User.h>
#include <Core/Log.h>
#include <Core/Types/Integer.h>
#include <inc/configuration.h>
#include <sqlite3.h>
#include <string.h>
#include <unistd.h>

Meow::Persistence::Drivers::SQLITEDriver::SQLITEDriver() :
    driver(NULL)
{
    SetMeowClassName("Meow/Persistence/SQLITEDriver");
}
Meow::Persistence::Drivers::SQLITEDriver::~SQLITEDriver()
{
}
void Meow::Persistence::Drivers::SQLITEDriver::Initialise()
{
    if ( !iCaller->GetIsConnected() )
    {
        database=iCaller->GetAlias();
        database.append("_file");
        database=Configuration::GetConfiguration()->Get(database);

        unitemporalt=iCaller->GetAlias();
        unitemporalt.append("_unitemporal");
        unitemporalt=Configuration::GetConfiguration()->Get(unitemporalt);
        isunitemporal=!unitemporalt.compare("true");


        std::string encryptionmode=iCaller->GetAlias();
        encryptionmode.append("_encryption");
        encryptionmode=Configuration::GetConfiguration()->Get(encryptionmode);
        int rc=sqlite3_open(database.c_str(),(sqlite3**)&driver);
        if ( rc )
        {
            char error[256];
            sprintf(error,"Can't open database: %s\n",sqlite3_errmsg((sqlite3*)driver));
            Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Initialise","ss","Can't open database:",sqlite3_errmsg((sqlite3*)driver));
            sqlite3_close((sqlite3*)driver);
            driver=NULL;
        }
        else
        {
            const void * pkey;
            int nkey_length;
            iCaller->SetIsConnected(true);
        }
    }
}
void Meow::Persistence::Drivers::SQLITEDriver::Close()
{
    if ( driver!=NULL )
    {
        sqlite3_close((sqlite3*)driver);
//			free(driver);
        driver=NULL;
    }
}
void Meow::Persistence::Drivers::SQLITEDriver::BeginTransaction()
{
    char * errorMessage=NULL;
    sqlite3_exec((sqlite3*)driver,"BEGIN TRANSACTION;",NULL,NULL,&errorMessage);
    if ( errorMessage!=NULL )
    {
        Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::SQLITEDriver::BeginTransaction","s",errorMessage);
        sqlite3_free(errorMessage);
    }
}
void Meow::Persistence::Drivers::SQLITEDriver::CommitTransaction()
{
    char * errorMessage=NULL;
    sqlite3_exec((sqlite3*)driver,"COMMIT TRANSACTION;",NULL,NULL,&errorMessage);
    if ( errorMessage!=NULL )
    {
    	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::SQLITEDriver::CommitTransaction","s",errorMessage);
        sqlite3_free(errorMessage);
    }
}
void Meow::Persistence::Drivers::SQLITEDriver::RollbackTransaction()
{
    char * errorMessage=NULL;
    sqlite3_exec((sqlite3*)driver,"ROLLBACK TRANSACTION;",NULL,NULL,&errorMessage);
    if ( errorMessage!=NULL )
    {
    	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::SQLITEDriver::RollbackTransaction","s",errorMessage);
        sqlite3_free(errorMessage);
    }
}
std::string Meow::Persistence::Drivers::SQLITEDriver::Sanitize( std::string data, bool quoteit )
{
    std::string retval;
    iCaller->Initialise();
    if ( iCaller->GetIsConnected() )
    {
        if (quoteit)
            retval.append("'");
        if ( data.length()>0)
        {
            char * query=sqlite3_mprintf("%q",data.c_str());
            retval.append(query);
            sqlite3_free(query);
        }
        if (quoteit)
            retval.append("'");
    }
    return retval;
}
std::string Meow::Persistence::Drivers::SQLITEDriver::SanitizeColumnName( std::string data, bool notas )
{
	if ( notas )
    {
        if ( data.find(".")!=std::string::npos )
        {
            Core::Types::String_ptr col=new Core::Types::String(data);
            Core::Types::Array_ptr elements=col->Explode(".");
            if ( !elements.IsNull() )
            {
                if ( elements->Count() > 1 )
                {
                	Core::Types::ValueType_ptr first=(Core::Types::ValueType_ptr)elements->Get(0);
                    Core::Types::ValueType_ptr second=(Core::Types::ValueType_ptr)elements->Get(1);
                    data="`" + first->GetValueAsString() + "`.`" + second->GetValueAsString() + "`";
                }
            }
        }
        else
        	data="`" + data + "`";
    }
    else
    	data="`" + data + "`";

    return data;
}
std::string Meow::Persistence::Drivers::SQLITEDriver::SanitizeTableName( std::string data )
{
    return data;
}

std::string		Meow::Persistence::Drivers::SQLITEDriver::CastToString( std::string data )
{
	return data;
}
 std::string		Meow::Persistence::Drivers::SQLITEDriver::CastToInteger( std::string data )
{
	return data;
}
void Meow::Persistence::Drivers::SQLITEDriver::HandleException( int rc ) throw(Meow::Persistence::Exceptions::SQLError,
        Meow::Persistence::Exceptions::IPCError,
        Meow::Exceptions::Rollback,
        Meow::Exceptions::Terminate,
        Meow::Exceptions::Retry)
{
    int exceptionType=0; /* 0 is no exception */
    switch(rc)
    {
    case SQLITE_OK:
        break;
    case SQLITE_ERROR:
        exceptionType=1;
        break;
    case SQLITE_INTERNAL:
        exceptionType=3;
        break;
    case SQLITE_PERM:
        exceptionType=3;
        break;
    case SQLITE_ABORT:
        exceptionType=2;
        break;
    case SQLITE_BUSY:
        exceptionType=4;
        break;
    case SQLITE_LOCKED:
        exceptionType=4;
        break;
    case SQLITE_NOMEM:
        exceptionType=2;
        break;
    case SQLITE_READONLY:
        exceptionType=2;
        break;
    case SQLITE_INTERRUPT:
        exceptionType=2;
        break;
    case SQLITE_IOERR:
        exceptionType=2;
        break;
    case SQLITE_CORRUPT:
        exceptionType=3;
        break;
    case SQLITE_FULL:
        exceptionType=2;
        break;
    case SQLITE_CANTOPEN:
        exceptionType=3;
        break;
    case SQLITE_EMPTY:
        exceptionType=2;
        break;
    case SQLITE_SCHEMA:
        exceptionType=2;
        break;
    case SQLITE_TOOBIG:
        exceptionType=1;
        break;
    case SQLITE_CONSTRAINT:
        exceptionType=1;
        break;
    case SQLITE_MISMATCH:
        exceptionType=1;
        break;
    case SQLITE_MISUSE:
        exceptionType=3;
        break;
    case SQLITE_AUTH:
        exceptionType=3;
        break;
    case SQLITE_FORMAT:
        exceptionType=3;
        break;
    case SQLITE_RANGE:
        exceptionType=3;
        break;
    case SQLITE_NOTADB:
        exceptionType=3;
        break;
    }
    Core::Types::Integer_ptr rcAsInt=new Core::Types::Integer(rc);
    switch(exceptionType)
    {
    case 1:
		{
			char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
			Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery/SQLITE","ssss","rc=",rcAsInt->GetValueAsString().c_str()," error=",errorMessage);
			Meow::Persistence::Exceptions::SQLError error;
			throw error;
			break; /* Not needed but so what */
		}
    case 2:
		{
			char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
			Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery/SQLITE","ssss","rc=",rcAsInt->GetValueAsString().c_str()," error=",errorMessage);
			Meow::Exceptions::Rollback error;
			throw error;
			break; /* Not needed but so what */
		}
    case 3:
		{
			char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
			Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery/SQLITE","ssss","rc=",rcAsInt->GetValueAsString().c_str()," error=",errorMessage);
			Meow::Exceptions::Terminate error;
			throw error;
			break;
		}
    case 4:
		{
			char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
			Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery/SQLITE","ssss","rc=",rcAsInt->GetValueAsString().c_str()," error=",errorMessage);
			Meow::Exceptions::Retry error;
			throw error;
			break;
		}
    }
}

Meow::Persistence::SQLResult_ptr  Meow::Persistence::Drivers::SQLITEDriver::ObjectQuery( Query_ptr sql )
throw(Meow::Persistence::Exceptions::SQLError,Meow::Persistence::Exceptions::IPCError,
      Meow::Exceptions::Rollback,
      Meow::Exceptions::Terminate,
      Meow::Exceptions::Retry)
{

    Initialise();

    Meow::Persistence::SQLResult_ptr retval;
    retval=new Meow::Persistence::SQLResult();
    if ( iCaller->GetIsConnected() )
    {
        char * zErrMsg;
        const char * tail;

        sqlite3_stmt * res;
        Core::Types::Array_ptr cols=new Core::Types::Array();
        if ( isunitemporal )
            sql->SetUniTemporal();

        std::string sqlasstring=sql->Generate(iCaller);
        if ( sql->IsInstanceOf("Persistence/Select") )
        {
		    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s",sqlasstring.c_str());
    		int rc=SQLITE_LOCKED;
			while (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
			{
            	 rc=sqlite3_prepare_v2((sqlite3*)driver,
                                      sqlasstring.c_str(),
                                      sqlasstring.length(),
                                      &res,
                                      &tail);
				if (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
				{
					Pause(1000);
				    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Paused for:",sqlasstring.c_str());
				}
			}
			if ( rc!=SQLITE_OK )
            {

                char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
                HandleException(rc);

                Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Error:",errorMessage);
                Meow::Persistence::Exceptions::SQLError error;
                throw error;
            }
			else
			{
            	retval->iDriver=this;
	            retval->iDriverDescriptor=res;
    	        retval->loadedRows=0;
    	        retval->iLastInsertID="";
			}
        }
        else
        {
    		int rc=SQLITE_LOCKED;
    		Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s",sqlasstring.c_str());
    
		    while (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
			{
	            rc=sqlite3_exec((sqlite3*)driver,
                                sqlasstring.c_str(),
                                0,
                                0,
                                &zErrMsg);
				if (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
				{
					Pause(1000);
				    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Paused for:",sqlasstring.c_str());
				}

			}
            if ( rc!=SQLITE_OK )
            {

                char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
                Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Error:",errorMessage);
                HandleException(rc);
                Meow::Persistence::Exceptions::SQLError error;
                throw error;
            }
            else
            {
                retval->iDriver=this;
                retval->iDriverDescriptor=NULL;
                retval->loadedRows=0;
                retval->iLastInsertID="";
                char pszBuf[256];
                sprintf(pszBuf,"%d",(int)sqlite3_last_insert_rowid((sqlite3*)driver));
                retval->iLastInsertID=pszBuf;
            }
        }
    }
    else
    	Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s","Not connected");
    return retval;
}

void Meow::Persistence::Drivers::SQLITEDriver::DeleteDatabase( )
{
    if ( iCaller->GetIsConnected() )
    {
        sqlite3_close((sqlite3*)driver);
        driver=NULL;
    }
    database=iCaller->GetAlias();
    database.append("_file");
    database=Configuration::GetConfiguration()->Get(database);
    unlink(database.c_str());
}

std::string Meow::Persistence::Drivers::SQLITEDriver::ConvertTypeForComparison( std::string typedefof )
{
    // TODO
    return typedefof;
}

Meow::Core::Types::Array_ptr Meow::Persistence::Drivers::SQLITEDriver::FetchRowFromResult( void * descriptor, int guideRow )
{
    sqlite3_stmt * res=(sqlite3_stmt*)descriptor;
    int col_count=sqlite3_column_count(res);
    int rowindex=0;
    if ( sqlite3_step(res)!= SQLITE_ROW )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        std::string message="Row not found in result set:" + guideRow;
        Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::SQLITEDriver::FetchRowFromResult","s",message.c_str());
        throw exception;
    }

    Core::Types::Array_ptr thisrow=new Core::Types::Array();
    for ( int i=0; i<col_count; i++ )
    {
        char * valptr=(char*)sqlite3_column_text(res,i);
        char * colptr=(char*)sqlite3_column_name(res,i);
        std::string val=valptr ? valptr : "";
        std::string col=colptr ? colptr : "";
        thisrow->Set(col,new Core::Types::String(val));
    }
    return thisrow;
}
void Meow::Persistence::Drivers::SQLITEDriver::FreeResult( void * descriptor )
{
    sqlite3_stmt * res=(sqlite3_stmt*)descriptor;
    sqlite3_finalize(res);

}
std::string Meow::Persistence::Drivers::SQLITEDriver::ConvertClass( std::string classname )
{
	// We should just turn / into _
	if ( Configuration::GetConfiguration()->IsSet("table_" + classname) )
		return Configuration::GetConfiguration()->Get("table_" + classname);
    Core::Types::ValueType_ptr retval=new Core::Types::String(classname);
	retval=retval->Replace("/","_");
	return retval->GetValueAsString();

}
std::string Meow::Persistence::Drivers::SQLITEDriver::ConvertAttribute( std::string classname )
{
	// Make it all lower case
	Core::Types::String_ptr retval=new Core::Types::String(classname);
	retval=retval->ToLowerCase();
	return retval->GetValueAsString();
}
void Meow::Persistence::Drivers::SQLITEDriver::Create( std::string classname, Core::Types::Array_ptr operationList ) 
					throw(Meow::Persistence::Exceptions::SQLError,
						  Meow::Persistence::Exceptions::IPCError,
						  Meow::Exceptions::Rollback,
						  Meow::Exceptions::Terminate)
{
	std::string keySQL;
	std::string attributeSQL;
	std::string SQL="CREATE TABLE " + ConvertClass(classname);
	bool firstKey=true;
	bool firstAttribute=true;
	for ( int i=0; i<operationList->Count(); i++ )
	{
		SchemaOperation_ptr operation=(SchemaOperation_ptr)operationList->Get(i);
		if ( operation->iVersion==0 )
		{
			if ( !firstAttribute ) attributeSQL+=",";
					
			switch (operation->iDefinition->iFieldType)
			{
			case Factory::Serial:
				attributeSQL+="`" + operation->iDefinition->iFieldName + "` INTEGER PRIMARY KEY AUTOINCREMENT";
				firstKey=false;
				firstAttribute=false;
				break;
			case Factory::Key:
				if ( !firstKey )
					keySQL+=",";
				keySQL+="`" +operation->iDefinition->iFieldName + "`";
				firstKey=false;
			case Factory::Attribute:
				{
					firstAttribute=false;
					std::string declaration="`" + operation->iDefinition->iFieldName + "` ";
					switch ( operation->iDefinition->iColumnType )
					{
					case Factory::Object:
						declaration+="VARCHAR(255)";			
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT \"\"";
						else
						{
							if ( operation->iDefinition->iDefaultValue->IsInstanceOf("ValueType") )
							{
								declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
							}
							else
							{
								declaration+=" DEFAULT \"" + ((Object_ptr)operation->iDefinition->iDefaultValue)->ToString() + "\"";
							}
						}
						break;
			        case Factory::String:
					case Factory::Enumeration:
						declaration+="VARCHAR(255)";			
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT \"\"";
						else
						{
							declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
						}
						break;
			        case Factory::Integer:
						declaration+="INTEGER";			
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT 0";
						else
						{
							declaration+=" DEFAULT " + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString();
						}
						break;
			        case Factory::Float:
						declaration+="FLOAT";			
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT 0";
						else
						{
							declaration+=" DEFAULT " + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString();
						}
						break;
			        case Factory::Boolean:
						declaration+="INTEGER";			
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT 0";
						else
						{
							declaration+=" DEFAULT " + Core::Types::ValueType::IntegerToString(((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsInteger());
						}
						break;
			        case Factory::Date:
						declaration+="INTEGER";			
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT 0";
						else
						{
							declaration+=" DEFAULT " + Core::Types::ValueType::IntegerToString(((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsInteger());
						}
						break;
					case Factory::Text:
					case Factory::XML:
					case Factory::HTML:
			        case Factory::Array:
					case Factory::Queue:
					case Factory::Stack:
					case Factory::Void:
					case Factory::ValueType:
					case Factory::Image:
					case Factory::File:
						declaration+="TEXT";
						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT \"\"";
						else
						{
							declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
						}
						break;
					}
					attributeSQL+=declaration;
					break;
				}
			}
		}
	}
	
	SQL+=" (" + attributeSQL + ",deleted INT DEFAULT 0, ___changed INT DEFAULT 0 ";
	if ( keySQL.length() > 0 )
		SQL+=",PRIMARY KEY(" + keySQL + ")";
	SQL+=")";
	// Call exec.
	Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Create","s",SQL.c_str());
    int rc=SQLITE_LOCKED;
    char * zErrMsg;
    const char * tail;

    sqlite3_stmt * res;
	while (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
	{

       	 rc=sqlite3_exec((sqlite3*)driver,
                       SQL.c_str(),
                                0,
                                0,
                                &zErrMsg);
		 if (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
			Pause(100);
	}
	if ( rc!=SQLITE_OK )
    {
        char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
        HandleException(rc);

        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Create","ss","Error:",errorMessage);
        Meow::Persistence::Exceptions::SQLError error;
        throw error;
    }	
}
void Meow::Persistence::Drivers::SQLITEDriver::Action( std::string classname, SchemaOperation_ptr operation ) 
					throw(Meow::Persistence::Exceptions::SQLError,
						  Meow::Persistence::Exceptions::IPCError,
						  Meow::Exceptions::Rollback,
						  Meow::Exceptions::Terminate)
{
	std::string declaration="";
	if ( operation->iOperation==1 )
	{
		// Not easily supported in SQLITE, have to create a new table without the column and transfer the 
		// data then rename the tables all in a single transaction.
	}
	else
	{
		declaration="ALTER TABLE " + ConvertClass(classname) + " ADD COLUMN `" + operation->iDefinition->iFieldName + "` ";
		switch ( operation->iDefinition->iColumnType )
		{
		case Factory::Object:
			declaration+="VARCHAR(255)";			
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT \"\"";
			else
			{
				if ( operation->iDefinition->iDefaultValue->IsInstanceOf("ValueType") )
				{
					declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
				}
				else
				{
					declaration+=" DEFAULT \"" + ((Object_ptr)operation->iDefinition->iDefaultValue)->ToString() + "\"";
				}
			}
			break;
        case Factory::String:
		case Factory::Enumeration:
			declaration+="VARCHAR(255)";			
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT \"\"";
			else
			{
				declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
			}
			break;
        case Factory::Integer:
			declaration+="INTEGER";			
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT 0";
			else
			{
				declaration+=" DEFAULT " + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString();
			}
			break;
        case Factory::Float:
			declaration+="FLOAT";			
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT 0";
			else
			{
				declaration+=" DEFAULT " + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString();
			}
			break;
        case Factory::Boolean:
			declaration+="INTEGER";			
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT 0";
			else
			{
				declaration+=" DEFAULT " + Core::Types::ValueType::IntegerToString(((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsInteger());
			}
			break;
        case Factory::Date:
			declaration+="INTEGER";			
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT 0";
			else
			{
				declaration+=" DEFAULT " + Core::Types::ValueType::IntegerToString(((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsInteger());
			}
			break;
		case Factory::Text:
		case Factory::XML:
		case Factory::HTML:
        case Factory::Array:
		case Factory::Queue:
		case Factory::Stack:
		case Factory::Void:
		case Factory::ValueType:

			declaration+="TEXT";
			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT \"\"";
			else
			{
				declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
			}
			break;
		}
	}
	Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Update","s",declaration.c_str());
    int rc=SQLITE_LOCKED;
    char * zErrMsg;
    const char * tail;

    sqlite3_stmt * res;
	while (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
	{
       	 rc=sqlite3_exec((sqlite3*)driver,
                       declaration.c_str(),
                                0,
                                0,
                                &zErrMsg);
		 if (rc==SQLITE_LOCKED || rc==SQLITE_BUSY )
			Pause(100);
	}
	if ( rc!=SQLITE_OK )
    {
        char * errorMessage=(char*)sqlite3_errmsg((sqlite3*)driver);
        HandleException(rc);

        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Update","ss","Error:",errorMessage);
        Meow::Persistence::Exceptions::SQLError error;
        throw error;
    }
	
}

