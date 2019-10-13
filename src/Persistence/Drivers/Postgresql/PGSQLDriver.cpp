/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#include <Persistence/DatabaseDriver.h>
#include <Persistence/Drivers/Postgresql/PGSQLDriver.h>
#include <Persistence/Update.h>
#include <Registry.h>
#include <Security/User.h>
#include <Core/Log.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Queue.h>
#include <inc/configuration.h>
// #include <postgresql/postgres.h>
#include <postgresql/postgres_ext.h>
#include <postgresql/libpq-fe.h>
#include <iostream>
#include <cstring>

Meow::Persistence::Drivers::PGSQLDriver::PGSQLDriver() :
    driver(NULL)
{
    SetMeowClassName("Meow/Persistence/PGSQLDriver");
}
Meow::Persistence::Drivers::PGSQLDriver::~PGSQLDriver()
{
}
void Meow::Persistence::Drivers::PGSQLDriver::Initialise()
{
    if ( !iCaller->GetIsConnected() )
    {

        connectstring=iCaller->GetAlias();
        connectstring.append("_connectstring");
        connectstring=Configuration::GetConfiguration()->Get(connectstring);
        driver=(void*)PQconnectdb(connectstring.c_str());
        if ( PQstatus((PGconn*)driver)==CONNECTION_OK)
        {
            iCaller->SetIsConnected(true);
        }
        else
        {
        	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::PGSQLDriver::Initialise","ss","Failed to connect due to Postgres error ",PQerrorMessage((PGconn*)driver));
        }
    }
}
void Meow::Persistence::Drivers::PGSQLDriver::Close()
{
    if ( driver!=NULL )
    {
        PQfinish((PGconn*)driver);
        driver=NULL;
    }
}
void Meow::Persistence::Drivers::PGSQLDriver::BeginTransaction()
{

    PGresult * res=PQexec((PGconn*)driver,"BEGIN;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string error=PQerrorMessage((PGconn*)driver);

        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::BeginTransaction","ss","Error ",error.c_str());
    }
    /* Patch for lastval selection */
    res=PQexec((PGconn*)driver,"CREATE SEQUENCE seq;");
    res=PQexec((PGconn*)driver,"SELECT nextval('seq');");

}
void Meow::Persistence::Drivers::PGSQLDriver::CommitTransaction()
{
   /* Patch for lastval selection */
   PGresult * res=PQexec((PGconn*)driver,"DROP SEQUENCE seq;");
   res=PQexec((PGconn*)driver,"COMMIT;");
   if (PQresultStatus(res) != PGRES_COMMAND_OK)
   {
        std::string error=PQerrorMessage((PGconn*)driver);
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::CommitTransaction","ss","Error ",error.c_str());
   }
}
void Meow::Persistence::Drivers::PGSQLDriver::RollbackTransaction()
{
    PGresult * res=PQexec((PGconn*)driver,"ROLLBACK;");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string error=PQerrorMessage((PGconn*)driver);
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::RollbackTransaction","ss","Error ",error.c_str());
    }
}
std::string Meow::Persistence::Drivers::PGSQLDriver::Sanitize( std::string data, bool quoteit )
{
    std::string retval;
    iCaller->Initialise();
    if ( iCaller->GetIsConnected() )
    {
        char *query;
        query=(char*)malloc((data.length()*2)+1);
        strcpy(query,"");
        int error;
        size_t finalsize=PQescapeStringConn((PGconn *)driver,query,data.c_str(),data.length(),&error);
        retval=std::string("'") + query + std::string("'");
        free(query);
    }
    return retval;
}
std::string Meow::Persistence::Drivers::PGSQLDriver::SanitizeColumnName( std::string data, bool notas )
{
    std::string retval;
    retval="\"";
    if ( notas )
    {
        Core::Types::String_ptr dataAsString=new Core::Types::String(data);
        Core::Types::Array_ptr dataAsStringComponents=dataAsString->Explode(".");
        if ( dataAsStringComponents->Count()==1 )
        {
            Core::Types::String_ptr dataAsLowerCaseString=dataAsString->ToLowerCase();
            retval.append(dataAsLowerCaseString->GetValue());
            retval.append("\"");
        }
        else if ( dataAsStringComponents->Count()==2 )
        {
        	Core::Types::String_ptr className=(Core::Types::String_ptr)dataAsStringComponents->Get(0);
        	Core::Types::String_ptr columnName=(Core::Types::String_ptr)dataAsStringComponents->Get(1);
            retval.append(className->GetValueAsString());
            retval.append("\".\"");
            Core::Types::String_ptr dataAsLowerCaseString=columnName->ToLowerCase();
            retval.append(dataAsLowerCaseString->GetValue());
            retval.append("\"");
        }
    }
    else
    {
        retval.append(data);
        retval.append("\"");
    }
    return retval;
}
std::string Meow::Persistence::Drivers::PGSQLDriver::SanitizeTableName( std::string data )
{
    return std::string("\"") + data.substr(0,62) + std::string("\"");
}
std::string		Meow::Persistence::Drivers::PGSQLDriver::CastToString( std::string data )
{
	return std::string("(") + data + ")::varchar";
}
 std::string		Meow::Persistence::Drivers::PGSQLDriver::CastToInteger( std::string data )
{
	return std::string("(") + data + ")::int";
}
Meow::Persistence::SQLResult_ptr  Meow::Persistence::Drivers::PGSQLDriver::ObjectQuery( Query_ptr sql )
throw(Meow::Persistence::Exceptions::SQLError,
      Meow::Persistence::Exceptions::IPCError,
      Meow::Exceptions::Rollback,
      Meow::Exceptions::Terminate,
      Meow::Exceptions::Retry)
{
    Initialise();
#ifdef MEOW_PERFORMANCE_ON	
	Core::Types::Array_ptr params=new Core::Types::Array();
	MEOW_PERFORMANCE_ENTRY(handlePersistenceMethodTimer,"PGSQLDriver::ObjectQuery",PERFORMANCE_SUBSYS_DATA,params);
#endif
    Meow::Persistence::SQLResult_ptr retval;
    retval=new Meow::Persistence::SQLResult();
    if ( iCaller->GetIsConnected() )
    {

        int fields;
        int tuples;
        Core::Types::Array_ptr cols=new Core::Types::Array();
        std::string sqlasstring=sql->Generate(iCaller);
#ifdef MEOW_PERFORMANCE_ON	
		params->Set(0,new String(sqlasstring));
#endif
        PGresult *result=PQexec((PGconn*)driver,sqlasstring.c_str());
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s",sqlasstring.c_str());


        if ( PQresultStatus(result) == PGRES_COMMAND_OK || PQresultStatus(result) == PGRES_TUPLES_OK )
        {
           retval->iDriver=this;
           retval->iDriverDescriptor=result;
           retval->loadedRows=0;
           if ( sql->IsInstanceOf("Persistence/Insert") )
		   {
				Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s","SELECT lastval();");
				result=PQexec((PGconn*)driver,"SELECT lastval();");
				if ( PQresultStatus(result) == PGRES_COMMAND_OK || PQresultStatus(result) == PGRES_TUPLES_OK )
				{
					tuples=PQntuples(result);
					if ( tuples>0 )
					{
						retval->iLastInsertID=PQgetvalue(result,0,0)? PQgetvalue(result,0,0) : "";
					}
				}
				else
				{
					std::string error=PQerrorMessage((PGconn*)driver);
					Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery-Error String",error.c_str());
				}
			}
        }
        else
        {
            std::string error=PQerrorMessage((PGconn*)driver);
            Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Error String",error.c_str());
            Meow::Persistence::Exceptions::SQLError sqlerror;
            sqlerror.SetMessage(error);
            throw sqlerror;
        }
    }
 #ifdef MEOW_PERFORMANCE_ON	
    MEOW_PERFORMANCE_EXIT(handlePersistenceMethodTimer);
#endif
    return retval;
}

std::string Meow::Persistence::Drivers::PGSQLDriver::ConvertTypeForComparison( std::string typedefof )
{
    if ( !typedefof.compare("INT") )
        return "integer";
    if ( !typedefof.compare("VARCHAR(255)") )
        return "character varying";
    if ( !typedefof.compare("FLOAT") )
        return "real";
    if ( !typedefof.compare("TEXT") )
        return "text";
    if ( !typedefof.compare("LONGTEXT") )
        return "text";
    return typedefof;
}
Meow::Core::Types::Array_ptr Meow::Persistence::Drivers::PGSQLDriver::FetchRowFromResult( void * descriptor, int guideRow )
{
    PGresult *result=(PGresult *)descriptor;
    if ( PQntuples(result)<=guideRow )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::PGSQLDriver::FetchRowFromResult","sls","Row ",guideRow," not found in result set.");
        throw exception;
    }
    int	fields=PQnfields(result);
    Core::Types::Array_ptr cols=new Core::Types::Array();
    for ( int i=0; i<fields; i++ )
    {
        Core::Types::String_ptr colname=new Core::Types::String(PQfname(result,i));
        cols->Set(i,(Core::ptr)colname);
    }
    Core::Types::Array_ptr thisrow=new Core::Types::Array();
    for ( int j=0; j<fields; j++ )
    {
        const char * rowValue=PQgetvalue(result,guideRow,j)? PQgetvalue(result,guideRow,j) : "";
        Core::Types::String_ptr thisvalue=new Core::Types::String(rowValue);
        Core::Types::String_ptr col=(Core::Types::String_ptr)cols->Get(j);
        thisrow->Set(col->GetValue(),(Core::ptr)thisvalue);
    }
    return thisrow;
}

void Meow::Persistence::Drivers::PGSQLDriver::FreeResult( void * descriptor )
{

    PGresult *result=(PGresult *)descriptor;
    PQclear(result);

}
std::string Meow::Persistence::Drivers::PGSQLDriver::ConvertClass( std::string classname )
{
	// We should just turn / into _
	if ( Configuration::GetConfiguration()->IsSet("table_" + classname) )
		return Configuration::GetConfiguration()->Get("table_" + classname);
	Core::Types::ValueType_ptr retval=new Core::Types::String(classname);
	retval=retval->Replace("/","_");
	return retval->GetValueAsString();
}
std::string Meow::Persistence::Drivers::PGSQLDriver::ConvertAttribute( std::string classname )
{
	// Make it all lower case
	Core::Types::String_ptr retval=new Core::Types::String(classname);
	retval=retval->ToLowerCase();
	return retval->GetValueAsString();
}
void Meow::Persistence::Drivers::PGSQLDriver::Create( std::string classname, Core::Types::Array_ptr operationList ) 
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
			if ( !firstKey ) keySQL+=",";
					
			switch (operation->iDefinition->iFieldType)
			{
			case Factory::Serial:
				attributeSQL+=operation->iDefinition->iFieldName + " SERIAL";
				firstKey=false;
				firstAttribute=false;
				break;
			case Factory::Key:
				if ( !firstKey )
					keySQL+=",";
				keySQL+=operation->iDefinition->iFieldName + "";
				firstKey=false;
			case Factory::Attribute:
				{
					if ( !firstAttribute ) attributeSQL+=",";
					firstAttribute=false;
					std::string declaration=operation->iDefinition->iFieldName + " ";
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
					case Factory::HTML:
					case Factory::Text:
					case Factory::XML:
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
    PGresult * result=PQexec((PGconn*)driver,"DROP SEQUENCE seq;");
	if ( PQresultStatus(result) == PGRES_COMMAND_OK || PQresultStatus(result) == PGRES_TUPLES_OK )
    {
	    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s","Success");
    }
    else
    {
        std::string error=PQerrorMessage((PGconn*)driver);
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Error String",error.c_str());
        Meow::Persistence::Exceptions::SQLError sqlerror;
        sqlerror.SetMessage(error);
        throw sqlerror;
    }
}
void Meow::Persistence::Drivers::PGSQLDriver::Action( std::string classname, SchemaOperation_ptr operation ) 
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
		declaration="ALTER TABLE " + ConvertClass(classname) + " DROP COLUMN " + operation->iDefinition->iFieldName;
	}
	else
	{
		declaration="ALTER TABLE " + ConvertClass(classname) + " ADD COLUMN " + operation->iDefinition->iFieldName + " ";
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

    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Create","s",declaration.c_str());
    PGresult * result=PQexec((PGconn*)driver,"DROP SEQUENCE seq;");
	if ( PQresultStatus(result) == PGRES_COMMAND_OK || PQresultStatus(result) == PGRES_TUPLES_OK )
    {
	    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s","Success");
    }
    else
    {
        std::string error=PQerrorMessage((PGconn*)driver);
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Error String",error.c_str());
        Meow::Persistence::Exceptions::SQLError sqlerror;
        sqlerror.SetMessage(error);
        throw sqlerror;
    }
}

