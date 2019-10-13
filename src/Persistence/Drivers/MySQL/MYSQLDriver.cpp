#include <Persistence/DatabaseDriver.h>
#include <Persistence/Drivers/MySQL/MYSQLDriver.h>
#include <Persistence/Update.h>
#include <Registry.h>
#include <Security/User.h>
#include <Core/Log.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Array.h>
#include <Core/Types/Queue.h>
#include <inc/configuration.h>
#ifdef WIN32
	#include <winsock.h>
#endif
#ifdef USEMARIADB
    #include <mariadb/mysql.h>
#else
    #include <mysql/mysql.h>
#endif
#include <string.h>
#include <stdio.h>
#include <unistd.h>

Meow::Persistence::Drivers::MYSQLDriver::MYSQLDriver() :
    driver(NULL)
{
    SetMeowClassName("Meow/Persistence/MYSQLDriver");
}
Meow::Persistence::Drivers::MYSQLDriver::~MYSQLDriver()
{
}
void Meow::Persistence::Drivers::MYSQLDriver::Initialise()
{
    if ( !iCaller->GetIsConnected() )
    {
        hostname=iCaller->GetAlias();
        hostname.append("_hostname");
        hostname=Configuration::GetConfiguration()->Get(hostname);

        username=iCaller->GetAlias();
        username.append("_username");
        username=Configuration::GetConfiguration()->Get(username);

        password=iCaller->GetAlias();
        password.append("_password");
        password=Configuration::GetConfiguration()->Get(password);

        databasename=iCaller->GetAlias();
        databasename.append("_database");
        databasename=Configuration::GetConfiguration()->Get(databasename);
        databasenamecache=databasename;

        std::string unitemporalt=iCaller->GetAlias();
        unitemporalt.append("_unitemporal");
        unitemporalt=Configuration::GetConfiguration()->Get(unitemporalt);
        isunitemporal=!unitemporalt.compare("true");

        driver=malloc(sizeof(MYSQL));
        mysql_init((MYSQL*)driver);
        mysql_real_connect((MYSQL*)driver,
                           hostname.c_str(),
                           username.c_str(),
                           password.c_str(),
                           databasename.c_str(),
                           0,
                           NULL,
                           0);
        iCaller->SetIsConnected(true);
    }
}
void Meow::Persistence::Drivers::MYSQLDriver::Close()
{
    if ( driver!=NULL )
    {
        mysql_close((MYSQL*)driver);
        free(driver);
    }
}
void Meow::Persistence::Drivers::MYSQLDriver::BeginTransaction()
{
    iCaller->Initialise();
    if ( iCaller->GetIsConnected() )
    {
	 Core::Log::GetLog()->Transaction("Meow::Persistence::Database::BeginTransaction","s","BEGIN");
	int ret=mysql_real_query((MYSQL*)driver,"BEGIN;",6);
	if ( ret )
            Core::Log::GetLog()->Transaction("Meow::Persistence::Database::BeginTransaction","ss","SQL Error running Begin:",mysql_error((MYSQL*)driver));
    }
}
void Meow::Persistence::Drivers::MYSQLDriver::CommitTransaction()
{
    iCaller->Initialise();
    if ( iCaller->GetIsConnected() )
    {
        Core::Log::GetLog()->Transaction("Meow::Persistence::Database::CommitTransaction","s","COMMIT");
        int ret=mysql_real_query((MYSQL*)driver,"COMMIT;",7);
	if ( ret )
            Core::Log::GetLog()->Transaction("Meow::Persistence::Database::CommitTransaction","ss","SQL Error running COMMIT:",mysql_error((MYSQL*)driver));

    }
}
void Meow::Persistence::Drivers::MYSQLDriver::RollbackTransaction()
{
    iCaller->Initialise();
    if ( iCaller->GetIsConnected() )
    {
	Core::Log::GetLog()->Transaction("Meow::Persistence::Database::RollbackTransaction","s","ROLLBACK");
        int ret=mysql_real_query((MYSQL*)driver,"ROLLBACK;",9);
	if ( ret )
            Core::Log::GetLog()->Transaction("Meow::Persistence::Database::RollbackTransaction","ss","SQL Error running ROLLBACK:",mysql_error((MYSQL*)driver));

    }
}
std::string Meow::Persistence::Drivers::MYSQLDriver::Sanitize( std::string data, bool quoteit )
{
    std::string retval;
    iCaller->Initialise();
    if ( iCaller->GetIsConnected() )
    {
        if (quoteit)  retval.append("\"");
        if ( data.length()>0)
        {
            char * query;
            query=(char*)malloc((data.length()*2)+1);
            strcpy(query,"");
            mysql_real_escape_string((MYSQL*)driver,
                                     query,
                                     data.c_str(),
                                     data.length());
            retval.append(query);
            free(query);
        }
        if (quoteit)
            retval.append("\"");
    }
    return retval;
}
std::string Meow::Persistence::Drivers::MYSQLDriver::SanitizeColumnName( std::string data,bool notas )
{
    std::string retval;
    retval="`";
    retval.append(data);
    retval.append("`");
    return retval;
}
std::string Meow::Persistence::Drivers::MYSQLDriver::SanitizeTableName( std::string data )
{
    data=data.substr(0,62);
    return data;
}
std::string		Meow::Persistence::Drivers::MYSQLDriver::CastToString( std::string data )
{
	return data;
}
 std::string		Meow::Persistence::Drivers::MYSQLDriver::CastToInteger( std::string data )
{
	return data;
}

Meow::Persistence::SQLResult_ptr  Meow::Persistence::Drivers::MYSQLDriver::ObjectQuery( Query_ptr query )
throw(Meow::Persistence::Exceptions::SQLError,
      Meow::Persistence::Exceptions::IPCError,
      Meow::Exceptions::Rollback,
      Meow::Exceptions::Terminate,
      Meow::Exceptions::Retry)
{
    Initialise();
    Meow::Persistence::SQLResult_ptr retval;
    retval=new Meow::Persistence::SQLResult();
    if ( iCaller->GetIsConnected() )
    {

        MYSQL_RES *res;
        MYSQL_FIELD *field;
        MYSQL_ROW row;
        unsigned int fields;
        unsigned int rowindex;
        Core::Types::Array_ptr cols=new Core::Types::Array();
        if ( isunitemporal )
            query->SetUniTemporal();
        std::string sqlasstring=query->Generate(iCaller);
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","mysql_real_query:",sqlasstring.c_str());

        int ret=mysql_real_query((MYSQL*)driver,sqlasstring.c_str(),(unsigned int)sqlasstring.length());
        if ( ret )
            Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","SQL Error running query:",mysql_error((MYSQL*)driver));
        else
        {
            retval->iLastInsertID="";
			if ( query->IsInstanceOf("Persistence/Select") )
			{
		        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","s","Copy result");
    	        res=mysql_store_result((MYSQL*)driver);
    	        if ( res )
    	        {
    	            retval->iDriver=this;
    	            retval->iDriverDescriptor=res;
    	            retval->loadedRows=0;
    	        }
			}		
			else if ( query->IsInstanceOf("Persistence/Insert") )
			{
            	char pszBuf[256];

    	        sprintf(pszBuf,"%d",(int)mysql_insert_id((MYSQL*)driver));
		        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::ObjectQuery","ss","Last Insert ID:",pszBuf);
    	        retval->iLastInsertID=pszBuf;
			}
        }
    }
    return retval;
}
std::string Meow::Persistence::Drivers::MYSQLDriver::ConvertTypeForComparison( std::string typedefof )
{
    if ( !typedefof.compare("INT") )
        return "int(11)";
    if ( !typedefof.compare("VARCHAR(255)") )
        return "varchar(255)";
    if ( !typedefof.compare("FLOAT") )
        return "float";
    if ( !typedefof.compare("TEXT") )
        return "text";
    if ( !typedefof.compare("LONGTEXT") )
        return "longtext";
}

Meow::Core::Types::Array_ptr Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult( void * descriptor, int guideRow )
{

    MYSQL_RES * res=(MYSQL_RES *)descriptor;
 	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","s","Calling mysql_num_rows");
    if ( mysql_num_rows(res)<guideRow )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","sls","Row ",guideRow," not found in result set.");
        throw exception;
    }

    MYSQL_FIELD *field;
    MYSQL_ROW row;
    unsigned int fields=0;
    Core::Types::Array_ptr cols=new Core::Types::Array();

    Core::Types::Array_ptr thisrow=new Core::Types::Array();
 	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","s","Calling mysql_num_fields");
    fields=mysql_num_fields(res);
    unsigned int fieldindex=0;
 	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","s","Calling mysql_fetch_field");
	mysql_field_seek(res,0);
    while (field=mysql_fetch_field(res))
    {
        Core::Types::String_ptr colname=new Core::Types::String(field->name);
		Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","sdss","Field:",fieldindex,"=",field->name);	
        cols->Set(fieldindex,(Core::ptr)colname);
        fieldindex++;
    }
 	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","s","Calling mysql_fetch_row");
    if (  row=mysql_fetch_row(res))
    {
	 	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","s","Process Row");
        for (int i=0; i<fields; i++ )
        {
            Core::Types::String_ptr thisvalue=new Core::Types::String(row[i] ? row[i] : "");
			
            Core::Types::String_ptr col=(Core::Types::String_ptr)cols->Get(i);
			Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","ssss","Col:",col->GetValueAsString().c_str(),"=",thisvalue->GetValueAsString().c_str());
            thisrow->Set(col->GetValue(),(Core::ptr)thisvalue);
        }
    }
	else
	 	Core::Log::GetLog()->Sequel("Meow::Persistence::Drivers::MYSQLDriver::FetchRowFromResult","s","No row :(");
    return thisrow;
}
void Meow::Persistence::Drivers::MYSQLDriver::FreeResult( void * descriptor )
{
    mysql_free_result((MYSQL_RES*)descriptor);
}
std::string Meow::Persistence::Drivers::MYSQLDriver::ConvertClass( std::string classname )
{
	// We should just turn / into _
	if ( Configuration::GetConfiguration()->IsSet("table_" + classname) )
		return Configuration::GetConfiguration()->Get("table_" + classname);
	Core::Types::ValueType_ptr retval=new Core::Types::String(classname);
	retval=retval->Replace("/","_");
	return retval->GetValueAsString();
}
std::string Meow::Persistence::Drivers::MYSQLDriver::ConvertAttribute( std::string classname )
{
	// Make it all lower case
	Core::Types::String_ptr retval=new Core::Types::String(classname);
	retval=retval->ToLowerCase();
	return retval->GetValueAsString();
}
void Meow::Persistence::Drivers::MYSQLDriver::Create( std::string classname, Core::Types::Array_ptr operationList ) 
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
					
			switch (operation->iDefinition->iFieldType)
			{
			case Factory::Serial:
				attributeSQL+="`" + operation->iDefinition->iFieldName + "` INTEGER AUTO_INCREMENT PRIMARY KEY";
				firstKey=false;
				firstAttribute=false;
				break;
			case Factory::Key:
				if ( !firstKey )
					keySQL+=",";
				keySQL+="`" + operation->iDefinition->iFieldName + "` ";
				firstKey=false;
			case Factory::Attribute:
				{
					if ( !firstAttribute ) attributeSQL+=",";

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
			        case Factory::Array:
					case Factory::Queue:
					case Factory::Stack:
					case Factory::Void:
					case Factory::Text:
					case Factory::HTML:
					case Factory::XML:
					case Factory::ValueType:
						declaration+="TEXT";
/*						if ( operation->iDefinition->iDefaultValue.IsNull() )
							declaration+=" DEFAULT \"\"";
						else
						{
							declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
						} */
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
	MYSQL_RES *res;
    int ret=mysql_real_query((MYSQL*)driver,SQL.c_str(),(unsigned int)SQL.length());
    if ( ret )
    {
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Create","ss","SQL Error running query:",mysql_error((MYSQL*)driver));
    }
    else
	    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Create","s","Success");
}
void Meow::Persistence::Drivers::MYSQLDriver::Action( std::string classname, SchemaOperation_ptr operation ) 
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
		declaration="ALTER TABLE " + ConvertClass(classname) + " DROP COLUMN `" + operation->iDefinition->iFieldName + "`";
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
        case Factory::Array:
		case Factory::Queue:
		case Factory::Stack:
		case Factory::Void:
		case Factory::ValueType:
		case Factory::Image:
		case Factory::File:
		case Factory::HTML:
		case Factory::XML:
			declaration+="TEXT";
/*			if ( operation->iDefinition->iDefaultValue.IsNull() )
				declaration+=" DEFAULT \"\"";
			else
			{
				declaration+=" DEFAULT \"" + ((Core::Types::ValueType_ptr)operation->iDefinition->iDefaultValue)->GetValueAsString() + "\"";
			} */
			break;
		}
	}
    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Action","s",declaration.c_str());
	MYSQL_RES *res;
    int ret=mysql_real_query((MYSQL*)driver,declaration.c_str(),(unsigned int)declaration.length());
    if ( ret )
    {
        Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Action","ss","SQL Error running query:",mysql_error((MYSQL*)driver));
    }
    else
	    Core::Log::GetLog()->Sequel("Meow::Persistence::Database::Action","s","Success");
}

