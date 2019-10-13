#include <Persistence/Update.h>
#include <Persistence/Database.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>
#include <Core/Types/Boolean.h>

Meow::Persistence::UpdateFactory::UpdateFactory()
{
    SetMeowClassName("Persistence/UpdateFactory");
}
Meow::Persistence::UpdateFactory::~UpdateFactory()
{
}
void Meow::Persistence::UpdateFactory::__Destroy( )
{
}
Meow::LifecycleObject_ptr
Meow::Persistence::UpdateFactory::GetInstance( std::string service )
throw(Meow::Exceptions::NotFound)
{
    return new Persistence::Update();
}
Meow::Persistence::Update::Update()
{
    SetMeowClassName("Persistence/Update");
    iAssigns=new Core::Types::Array();
    iFrom="";
}
Meow::Persistence::Update::~Update()
{

}
std::string Meow::Persistence::Update::GetWhat()
{
    return iFrom;
}
Meow::Persistence::Where_ptr Meow::Persistence::Update::GetWhere()
{
    if ( iWhere.IsNull() )
        iWhere=new Where();
    return iWhere;
}

void  Meow::Persistence::Update::SetWhat( std::string table )
{
    iFrom=table;
}

void  Meow::Persistence::Update::AddAssign( std::string column, std::string value )
{
    Core::Types::String_ptr value_as_ptr=new Core::Types::String();
    value_as_ptr->SetValue(value);
    iAssigns->Set(column,(Core::ptr)value_as_ptr);
}
void Meow::Persistence::Update::AddAssign( std::string column, char * value )
{
      Core::Types::String_ptr value_as_ptr=new Core::Types::String();
    value_as_ptr->SetValue(value);
    iAssigns->Set(column,(Core::ptr)value_as_ptr);
}

void Meow::Persistence::Update::AddAssign( std::string column, bool value )
{
	Core::Types::Boolean_ptr value_as_object=new Core::Types::Boolean(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, float value )
{
	Core::Types::Float_ptr value_as_object=new Core::Types::Float(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, double value )
{
	Core::Types::Float_ptr value_as_object=new Core::Types::Float(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, int value )
{ 
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, unsigned int value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, long value )
{ 
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, unsigned long value )
{
	Core::Types::Integer_ptr value_as_object=new Core::Types::Integer(value);
    AddAssign(column,(Core::Types::ValueType_ptr)value_as_object);
}
void Meow::Persistence::Update::AddAssign( std::string column, Core::Types::ValueType_ptr value )
{ 
    AddAssign(column,value->GetValueAsString());
}
void Meow::Persistence::Update::SetWhere( Meow::Persistence::Where_ptr where )
{
    iWhere=where;
}

std::string Meow::Persistence::Update::Generate( Meow::Persistence::Database_ptr database )
{
    std::string query="UPDATE ";
    query.append(database->SanitizeTableName(database->ConvertClass(iFrom)));
    query.append(" SET ");
    Core::Types::Stack_ptr keys=iAssigns->GetKeys();
    int i=0;
    while ( !keys->IsEmpty() )
    {
    	Core::Types::String_ptr column=(Core::Types::String_ptr)keys->Pop();
    	Core::Types::String_ptr value=(Core::Types::String_ptr)iAssigns->Get(column->GetValue());
        if (i>0)
            query.append(",");

        query.append(database->SanitizeColumnName(column->GetValue()));
        query.append("=");
        query.append(database->Sanitize(value->GetValue()));
        i++;
    }
    if ( !iWhere.IsNull() )
    {
        query.append(iWhere->Generate(database));
    }
    query.append(";");
    return query;
}
Meow::Persistence::SQLResult_ptr Meow::Persistence::Update::Execute( Meow::Persistence::Database_ptr database )
{
    Query_ptr query=(Query*)this;
    return database->ObjectQuery(query);
}
std::string Meow::Persistence::Update::ToXML( )
{
    std::string retval="<Query type=\"Update\"><Table>" + iFrom + "</Table>";
    if ( !iWhere.IsNull() )
        retval=retval + iWhere->ToXML();
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
