#include <Persistence/Delete.h>
#include <Persistence/Database.h>

Meow::Persistence::DeleteFactory::DeleteFactory()
{
    SetMeowClassName("Persistence/DeleteFactory");
}
Meow::Persistence::DeleteFactory::~DeleteFactory()
{
}
void Meow::Persistence::DeleteFactory::__Destroy( )
{
}
Meow::LifecycleObject_ptr
Meow::Persistence::DeleteFactory::GetInstance( std::string service )
throw(Meow::Exceptions::NotFound)
{
    return new Persistence::Delete();
}
Meow::Persistence::Delete::Delete()
{
    SetMeowClassName("Persistence/Delete");
}
Meow::Persistence::Delete::~Delete()
{

    iFrom="";
}

void  Meow::Persistence::Delete::SetFrom( std::string table )
{
    iFrom=table;
}
void Meow::Persistence::Delete::SetWhere( Meow::Persistence::Where_ptr where )
{
    iWhere=where;
}

std::string Meow::Persistence::Delete::Generate( Meow::Persistence::Database_ptr database )
{
    std::string query;

    if ( Unitemporal )
    {

        query="UPDATE " + database->SanitizeTableName(database->ConvertClass(iFrom));
        query.append(" SET VTEnd=");
        query.append(Baseline);
        if ( !iWhere.IsNull() )
        {
            iWhere->SetUniTemporal();
            query.append(iWhere->Generate(database));
        }
        else
        {
            query.append(" WHERE ( VTEnd >=" + Baseline + " AND VTBegin <=" + Baseline + " )");
        }
        query.append(";");
    }
    else
    {
        query="DELETE ";
        query.append(" FROM ");
        query.append(database->SanitizeTableName(database->ConvertClass(iFrom)));
        if ( !iWhere.IsNull() )
            query.append(iWhere->Generate(database));
        query.append(";");
    }
    return query;
}
Meow::Persistence::SQLResult_ptr Meow::Persistence::Delete::Execute( Meow::Persistence::Database_ptr database )
{
    Query_ptr query=(Query*)this;
    return database->ObjectQuery(query);
}
std::string Meow::Persistence::Delete::ToXML( )
{
    std::string retval="<Query type=\"Delete\"><Table>" + iFrom + "</Table>";

    if ( !iWhere.IsNull() )
        retval=retval + iWhere->ToXML();
    if ( Unitemporal )
    {
        retval=retval + "<Unitemporal>true</Unitemporal>";
        retval=retval + "<Baseline>" + Baseline + "</Baseline>";
    }
    retval=retval + "</Query>";
    return retval;
}

void   Meow::Persistence::Delete::__Destroy( )
{
}
bool   Meow::Persistence::Delete::IsPurgeable( )
{
    return false;
}
void   Meow::Persistence::Delete::Restore( )
throw(Meow::Exceptions::NotFound,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate)
{
    Meow::Exceptions::NotFound exception;
    exception.SetMessage("Row not found");
    throw exception;
}
void   Meow::Persistence::Delete::Passivate( )
throw(Meow::Exceptions::NotFound,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate)
{
}
void  Meow::Persistence::Delete::_Delete( )
throw(Meow::Exceptions::NotFound,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate)
{
}
void  Meow::Persistence::Delete::Create( )
throw(Meow::Exceptions::Duplicate,Meow::Exceptions::Rollback,Meow::Exceptions::Terminate)
{
}
