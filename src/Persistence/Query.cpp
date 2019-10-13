#include <Persistence/Query.h>
#include <Core/Time/Time.h>
#include <Core/Time/Date.h>
#include <Core/Types/ValueType.h>
#include <Registry.h>
#include <Persistence/Delete.h>
#include <Persistence/Select.h>
#include <Persistence/Update.h>
#include <Persistence/Insert.h>
#include <Persistence/Where.h>

Meow::Persistence::QueryFactory::QueryFactory()
{
    SetMeowClassName("Persistence/QueryFactory");
}
Meow::Persistence::QueryFactory::~QueryFactory()
{
}
void Meow::Persistence::QueryFactory::__Destroy( )
{
}
Meow::LifecycleObject_ptr Meow::Persistence::QueryFactory::GetInstance( std::string service ) throw(Meow::Exceptions::NotFound)
{
    return new Persistence::Query();
}
Meow::Persistence::Query_ptr Meow::Persistence::QueryFactory::RebuildFromXML( std::string xmlDoc )
{
    XMLReconstruction_ptr reconstruct=new XMLReconstruction();
    reconstruct->ParseString(xmlDoc);
    return reconstruct->GetQuery();
}
Meow::Persistence::Query::Query()
{
    SetMeowClassName("Persistence/Query");
    Unitemporal=false;
    if ( Registry::GetRegistry()->IsSessionReady() )
    {
        try
        {
            Core::Types::ValueType_ptr baseLinePtr=(Core::Types::ValueType_ptr)Registry::GetRegistry()->GetCurrentSession()->GetVariable("Baseline");
            Baseline=baseLinePtr->GetValueAsString();
        } catch ( Exception &e )
        {
            Core::Time::Date_ptr base=new Core::Time::Date();
            Baseline=base->GetValueAsString();
            Registry::GetRegistry()->GetCurrentSession()->SetVariable("Baseline",(Core::ptr)base);
        }
    }
}
Meow::Persistence::Query::~Query()
{
}
void Meow::Persistence::Query::SetUniTemporal()
{
    Unitemporal=true;
}
void Meow::Persistence::Query::SetBaseline( std::string baseline )
{
    Baseline=baseline;
}

std::string Meow::Persistence::Query::Generate( Database_ptr database )
{

}
Meow::Persistence::SQLResult_ptr Meow::Persistence::Query::Execute( Database_ptr database )
{

}
string Meow::Persistence::Query::ToXML( )
{
    string retval="<Query type=\"Unknown\">";
    if ( Unitemporal )
    {
        retval=retval + "<Unitemporal>true</Unitemporal>";
        retval=retval + "<Baseline>" + Baseline + "</Baseline>";
    }
    return retval;
}
Meow::Persistence::XMLReconstruction::XMLReconstruction()
{
    SetMeowClassName("Persistence/XMLReconstruction");
}
Meow::Persistence::XMLReconstruction::~XMLReconstruction()
{
}
void Meow::Persistence::XMLReconstruction::StartElement( string element, Core::Types::Array_ptr attributes )
{
    if ( !element.compare("Query") )
    {
    	Core::Types::ValueType_ptr type=(Core::Types::ValueType_ptr)attributes->Get("type");
        if ( !type->GetValueAsString().compare("Select") )
            iQuery=new Select();
        else if ( !type->GetValueAsString().compare("Update") )
            iQuery=new Update();
        else if ( !type->GetValueAsString().compare("Delete") )
            iQuery=new Delete();
        else if ( !type->GetValueAsString().compare("Insert") )
            iQuery=new Insert();
        iQueryType=type->GetValueAsString();
    }
    else if ( !element.compare("Where") )
    {
        iWhere=new Where();
    }
    else if ( !element.compare("Join") )
    {
        if ( !iQueryType.compare("Select") )
        {
            Select_ptr Statement=(Select_ptr)iQuery;
            Core::Types::ValueType_ptr jointype=(Core::Types::ValueType_ptr)attributes->Get("mode");
            Core::Types::ValueType_ptr tablename=(Core::Types::ValueType_ptr)attributes->Get("table");
            Core::Types::ValueType_ptr leftkey=(Core::Types::ValueType_ptr)attributes->Get("leftkey");
            Core::Types::ValueType_ptr rightkey=(Core::Types::ValueType_ptr)attributes->Get("rightkey");
            Statement->AddJoin(jointype->GetValueAsString(),tablename->GetValueAsString(),leftkey->GetValueAsString(),rightkey->GetValueAsString());
        }
        else
        {
            std::string message="Intercepted a JOIN call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    else if ( !element.compare("Column") )
    {
        if ( !iQueryType.compare("Select") )
        {
            Select_ptr Statement=(Select_ptr)iQuery;
            Core::Types::ValueType_ptr name=(Core::Types::ValueType_ptr)attributes->Get("name");
            Core::Types::ValueType_ptr as=(Core::Types::ValueType_ptr)attributes->Get("as");
            Statement->AddColumn(name->GetValueAsString(),as->GetValueAsString());
        }
        else
        {
            std::string message="Intercepted a Column call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    iCurrentElement=element;
    iCurrentAttributes=attributes;
    iCurrentElementData="";

}
void Meow::Persistence::XMLReconstruction::EndElement( string element )
{
    if ( !element.compare("Table") )
    {
        if ( !iQueryType.compare("Select") )
        {
            Select_ptr Statement=(Select_ptr)iQuery;
            Statement->SetFrom(iCurrentElementData);
        }
        else if ( !iQueryType.compare("Update") )
        {
            Update_ptr Statement=(Update_ptr)iQuery;
            Statement->SetWhat(iCurrentElementData);
        }
        else if ( !iQueryType.compare("Delete") )
        {
            Delete_ptr Statement=(Delete_ptr)iQuery;
            Statement->SetFrom(iCurrentElementData);
        }
        else if ( !iQueryType.compare("Insert") )
        {
            Insert_ptr Statement=(Insert_ptr)iQuery;
            Statement->SetInto(iCurrentElementData);
        }
        else
        {
            std::string message="Intercepted a Table call to a Persistence layer statement of the wrong format:" + iQueryType;
        }

    }
    else if ( !element.compare("Where") )
    {
        if ( !iQueryType.compare("Select") )
        {
            Select_ptr Statement=(Select_ptr)iQuery;
            Statement->SetWhere((Where_ptr)iWhere);
        }
        else if ( !iQueryType.compare("Update") )
        {
            Update_ptr Statement=(Update_ptr)iQuery;
            Statement->SetWhere((Where_ptr)iWhere);
        }
        else if ( !iQueryType.compare("Delete") )
        {
            Delete_ptr Statement=(Delete_ptr)iQuery;
            Statement->SetWhere((Where_ptr)iWhere);
        }
        else
        {
            std::string message="Intercepted a Where call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    else if ( !element.compare("Clause") && !iWhere.IsNull() )
    {
    	Core::Types::ValueType_ptr type=(Core::Types::ValueType_ptr)iCurrentAttributes->Get("type");
    	Core::Types::ValueType_ptr column=(Core::Types::ValueType_ptr)iCurrentAttributes->Get("column");
    	Core::Types::ValueType_ptr op=(Core::Types::ValueType_ptr)iCurrentAttributes->Get("op");
    	Core::Types::ValueType_ptr valuetype=(Core::Types::ValueType_ptr)iCurrentAttributes->Get("valuetype");
    	Core::Types::ValueType_ptr currentData=new Core::Types::String(iCurrentElementData);
    	Core::Types::ValueType_ptr value=currentData->Base64Decode();
        if ( !type->GetValueAsString().compare("bra") )
            ((Where_ptr)iWhere)->Bra();
        else if ( !type->GetValueAsString().compare("ket") )
            ((Where_ptr)iWhere)->Ket();
        else if ( !type->GetValueAsString().compare("and") )
            ((Where_ptr)iWhere)->LogicalAnd();
        else if ( !type->GetValueAsString().compare("or") )
            ((Where_ptr)iWhere)->LogicalOr();
        else
            ((Where_ptr)iWhere)->Expression(column->GetValueAsString(),op->GetValueAsString(),(Core::ptr)value,valuetype->GetValueAsString());
    }
    else if ( !element.compare("Limited") )
    {
        if ( !iCurrentElementData.compare("true") )
        {
            if ( !iQueryType.compare("Select") )
            {
                Select_ptr Statement=(Select_ptr)iQuery;
                Statement->iLimited=true;
            }
            else
            {
                std::string message="Intercepted a Limited call to a Persistence layer statement of the wrong format:" + iQueryType;
            }
        }
    }
    else if ( !element.compare("Offset") )
    {
        if ( !iQueryType.compare("Select") )
        {
        	Core::Types::String_ptr Offset=new Core::Types::String(iCurrentElementData);
            Select_ptr Statement=(Select_ptr)iQuery;
            Statement->iOffset=Offset->GetValueAsInteger();
        }
        else
        {
            std::string message="Intercepted an Offset call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    else if ( !element.compare("Limit") )
    {
        if ( !iQueryType.compare("Select") )
        {
        	Core::Types::String_ptr Limit=new Core::Types::String(iCurrentElementData);
            Select_ptr Statement=(Select_ptr)iQuery;
            Statement->iLimit=Limit->GetValueAsInteger();
        }
        else
        {
            std::string message="Intercepted a Limit call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    else if ( !element.compare("Distinct") )
    {
        if ( !iCurrentElementData.compare("true") )
        {
            if ( !iQueryType.compare("Select") )
            {
                Select_ptr Statement=(Select_ptr)iQuery;
                Statement->iDistinct=true;
            }
            else
            {
                std::string message="Intercepted a Distinct call to a Persistence layer statement of the wrong format:" + iQueryType;
            }
        }
    }
    else if ( !element.compare("Ordered") )
    {
        if ( !iCurrentElementData.compare("true") )
        {
            if ( !iQueryType.compare("Select") )
            {
                Select_ptr Statement=(Select_ptr)iQuery;
                Statement->iOrdered=true;
            }
            else
            {
                std::string message="Intercepted an Ordered call to a Persistence layer statement of the wrong format:" + iQueryType;
            }
        }
    }
    else if ( !element.compare("OrderColumn") )
    {
        if ( !iQueryType.compare("Select") )
        {
            Select_ptr Statement=(Select_ptr)iQuery;
            Statement->iOrderColumnOriginal=iCurrentElementData;
            Statement->iOrderByColumn->Set(Statement->iOrderByColumn->Count(),new Core::Types::String(iCurrentElementData));
        }
        else
        {
            std::string message="Intercepted an OrderColumn call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    else if ( !element.compare("OrderDirection") )
    {
        if ( !iQueryType.compare("Select") )
        {
            Select_ptr Statement=(Select_ptr)iQuery;
            Statement->iOrderDirection=iCurrentElementData;
        }
        else
        {
            std::string message="Intercepted an OrderDirection call to a Persistence layer statement of the wrong format:" + iQueryType;
        }
    }
    else if ( !element.compare("Unitemporal") )
    {
        if ( !iCurrentElementData.compare("true") )
        {
            iQuery->Unitemporal=true;
        }
    }
    else if ( !element.compare("Assign") )
    {
    	Core::Types::ValueType_ptr name=(Core::Types::ValueType_ptr)iCurrentAttributes;
    }
    else if ( !element.compare("Baseline") )
    {
        iQuery->Baseline=iCurrentElementData;
    }
}
void Meow::Persistence::XMLReconstruction::CharacterData( string data )
{
    iCurrentElementData.append(data);
}
Meow::Persistence::Query_ptr Meow::Persistence::XMLReconstruction::GetQuery()
{
    return iQuery;
}

