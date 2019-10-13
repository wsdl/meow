#include <Persistence/Insert.h>
#include <Persistence/Database.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Boolean.h>
#include <Core/Types/Float.h>
#include <Registry.h>
#include <Security/User.h>
#include <Session.h>

Meow::Persistence::InsertFactory::InsertFactory()
{
    SetMeowClassName("Persistence/InsertFactory");
}
Meow::Persistence::InsertFactory::~InsertFactory()
{
}
void Meow::Persistence::InsertFactory::__Destroy( )
{
}
Meow::LifecycleObject_ptr Meow::Persistence::InsertFactory::GetInstance( std::string service )
	throw(Meow::Exceptions::NotFound)
{
    return new Persistence::Insert();
}
Meow::Persistence::Insert::Insert()
{
    SetMeowClassName("Persistence/Insert");
    iAssigns=new Core::Types::Array();
    iFrom="";
}
Meow::Persistence::Insert::~Insert()
{
}

void  Meow::Persistence::Insert::SetInto( std::string table )
{
    iFrom=table;
}
void Meow::Persistence::Insert::AddAssign( std::string column, char * value )
{
    Core::Types::String_ptr value_as_ptr=new Core::Types::String();
    value_as_ptr->SetValue(value);
    iAssigns->Set(column,(Core::ptr)value_as_ptr);
}

void Meow::Persistence::Insert::AddAssign( std::string column, std::string value )
{

	Core::Types::String_ptr value_as_ptr=new Core::Types::String();
    value_as_ptr->SetValue(value);
    iAssigns->Set(column,(Core::ptr)value_as_ptr);
}

void Meow::Persistence::Insert::AddAssign( std::string column, bool value )
{

	Core::Types::Boolean_ptr value_as_object=new Core::Types::Boolean(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, float value )
{
	Core::Types::Float_ptr value_as_object=new Core::Types::Float(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, double value )
{

	Core::Types::Float_ptr value_as_object=new Core::Types::Float(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, int value )
{

	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, unsigned int value )
{

	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, long value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, unsigned long value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Insert::AddAssign( std::string column, Core::Types::ValueType_ptr value )
{
    AddAssign(column,value->GetValueAsString());
}

std::string Meow::Persistence::Insert::Generate( Meow::Persistence::Database_ptr database )
{
    std::string query="INSERT INTO ";
    std::string values="(";
    query.append(database->SanitizeTableName(database->ConvertClass(iFrom)));
    query.append(" (");
    int count=0;
    if ( Unitemporal )
    {
        query.append("User,VTBegin");
        std::string currentUser="0";
        if ( !((Session_ptr)Registry::GetRegistry()->GetCurrentSession())->GetCurrentUser().IsNull() )
        {
            Core::Types::ValueType_ptr uid=(Core::Types::ValueType_ptr)((Session_ptr)Registry::GetRegistry()->GetCurrentSession())->GetCurrentUser()->GetUniqueKey()->Get(0);
            currentUser=uid->GetValueAsString();
        }
		
        values.append(currentUser + std::string(",") + Baseline);
        count++;
    }

    Core::Types::Stack_ptr keys=iAssigns->GetKeys( );

    while ( !keys->IsEmpty() )
    {
    	Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
    	Core::Types::String_ptr value=(Core::Types::String_ptr)iAssigns->Get(key->GetValue());
	
        if ( count > 0 )
        {
            query.append(",");
            values.append(",");
        }
        query.append(database->SanitizeColumnName(database->ConvertAttribute(key->GetValue())));

        values.append(database->Sanitize(value->GetValueAsString()));
        count++;
    }
    query.append(") VALUES ");
    values.append(")");
    query.append(values);
    query.append(";");
    return query;
}
Meow::Persistence::SQLResult_ptr Meow::Persistence::Insert::Execute( Meow::Persistence::Database_ptr database )
{
    Query_ptr query=(Query*)this;
    return database->ObjectQuery(query);
}

std::string Meow::Persistence::Insert::ToXML( )
{
    std::string retval="<Query type=\"Insert\"><Table>" + iFrom + "</Table>";
    if ( Unitemporal )
    {
        retval=retval + "<Unitemporal>true</Unitemporal>";
        retval=retval + "<Baseline>" + Baseline + "</Baseline>";
    }
    Core::Types::Stack_ptr keys=iAssigns->GetKeys();
    while ( !keys->IsEmpty() )
    {
    	Core::Types::String_ptr column=(Core::Types::String_ptr)keys->Pop();
    	Core::Types::String_ptr value=(Core::Types::String_ptr)iAssigns->Get(column->GetValue());
    	Core::Types::ValueType_ptr valueAsBase64=value->Base64Encode();
        retval=retval + "<Assign name=\"" + column->GetValueAsString() + "\">" + valueAsBase64->GetValueAsString() + "</Assign>";
    }
    retval=retval + "</Query>";
    return retval;
}
