#include <Persistence/SQLResult.h>
#include <Persistence/DatabaseDriver.h>
#include <Core/Log.h>

Meow::Persistence::SQLResultFactory::SQLResultFactory()
{
    SetMeowClassName("Persistence/SQLResultFactory");
}
Meow::Persistence::SQLResultFactory::~SQLResultFactory()
{
}
void Meow::Persistence::SQLResultFactory::__Destroy( )
{
}
Meow::LifecycleObject_ptr Meow::Persistence::SQLResultFactory::GetInstance( std::string service ) throw(Meow::Exceptions::NotFound)
{
    return new Persistence::SQLResult();
}
Meow::Persistence::SQLResult_ptr Meow::Persistence::SQLResultFactory::DoFromXML( std::string xmldoc )
{
    XMLResultReconstruction_ptr reconstruct=new XMLResultReconstruction();
    reconstruct->ParseString(xmldoc);
    return reconstruct->GetResult();
}
Meow::Persistence::XMLResultReconstruction::XMLResultReconstruction()
{
    SetMeowClassName("Persistence/XMLResultReconstruction");
    iResult=new SQLResult();
}
Meow::Persistence::XMLResultReconstruction::~XMLResultReconstruction()
{
}
void Meow::Persistence::XMLResultReconstruction::StartElement( string element, Core::Types::Array_ptr attributes )
{
	iCurrentElement=element;
    iCurrentElementData="";
    iCurrentAttributes=attributes;
    if ( iCurrentElement.compare("Row") )
    {
    	Core::Types::Array_ptr row=new Core::Types::Array();
        iResult->Internal->Set(row->Count(),(Core::ptr)row);
    }
}
void Meow::Persistence::XMLResultReconstruction::EndElement( string element )
{
    if ( !iCurrentElement.compare("LastInsertID") )
        iResult->iLastInsertID=iCurrentElementData;
    else if ( !iCurrentElement.compare("Cell") )
    {
    	Core::Types::String_ptr name=(Core::Types::String_ptr)iCurrentAttributes->Get("name");
    	Core::Types::String_ptr valueEncoded=new Core::Types::String(iCurrentElementData);
    	Core::Types::String_ptr valueDecoded=(Core::Types::String_ptr)valueEncoded->Base64Decode();
    	Core::Types::Array_ptr row=(Core::Types::Array_ptr)iResult->Internal->Get(row->Count()-1);
        row->Set(name->GetValueAsString(),(Core::ptr)valueDecoded);
    }
}
void Meow::Persistence::XMLResultReconstruction::CharacterData( string data )
{
    iCurrentElementData.append(data);
}
Meow::Persistence::SQLResult_ptr Meow::Persistence::XMLResultReconstruction::GetResult()
{
    return iResult;
}

Meow::Persistence::SQLResult::SQLResult()
{
	recordcount=0;
    SetMeowClassName("Persistence/SQLResult");
    Internal=new Core::Types::Array();
    loadedRows=0;
    iDriverDescriptor=NULL;
}
Meow::Persistence::SQLResult::~SQLResult()
{
    Release();
}
std::string Meow::Persistence::SQLResult::FetchCellData( unsigned long row, std::string index ) throw(Meow::Exceptions::NotFound)
{
    std::string retval;
    if ( iDriverDescriptor!=NULL && loadedRows<=row )
    {
        for ( int i=loadedRows; i<=row; i++ )
        {
        	Core::Types::Array_ptr row=iDriver->FetchRowFromResult(iDriverDescriptor,i);
            Internal->Set(i,(Core::ptr)row);
        }
        loadedRows=Internal->Count();
    }
    if ( row>Internal->Count() )
    {
        // Throw Not Found
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        throw exception;
    }
    Meow::Core::Types::Array_ptr rowdata=(Meow::Core::Types::Array_ptr)Internal->Get(row);
    if ( rowdata.IsNull() )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row is NULL");
        throw exception;
    }
    if ( ! rowdata->IsSet(index) )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Column not found");
        throw exception;
    }
    Core::Types::String_ptr value=(Core::Types::String_ptr)rowdata->Get(index);
    retval=value->GetValue();
    return retval;
}

Meow::Core::Types::ValueType_ptr Meow::Persistence::SQLResult::FetchCellDataAsValue( unsigned long row, std::string index ) throw(Meow::Exceptions::NotFound)
{

    // Top level value type
	Core::Types::String_ptr retval=new Core::Types::String();
    if ( iDriverDescriptor!=NULL && loadedRows<=row )
    {
        // Bring in from the driver itself the missing rows
        for ( int i=loadedRows; i<=row; i++ )
        {
        	Core::Types::Array_ptr row=iDriver->FetchRowFromResult(iDriverDescriptor,i);
            Internal->Set(i,(Core::ptr)row);
        }
        loadedRows=Internal->Count();
    }
    if ( row>Internal->Count() )
    {
        // Throw Not Found
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        std::string message="Row not found in query:" + row;
        throw exception;
    }
    Meow::Core::Types::Array_ptr rowdata=(Meow::Core::Types::Array_ptr)Internal->Get(row);
    if ( rowdata.IsNull() )
    {
        std::string message="Row not found in query.";
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        throw exception;
    }
    if ( ! rowdata->IsSet(index) )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Column not found");
        std::string message="Column not found in query:(" + row + std::string(",") + index + std::string(")");
        throw exception;
    }
    return (Core::Types::ValueType_ptr)rowdata->Get(index);
}

Meow::Core::Types::Array_ptr Meow::Persistence::SQLResult::FetchRowAsArray( unsigned long row ) throw(Meow::Exceptions::NotFound)
{
    if ( iDriverDescriptor!=NULL && loadedRows<=row )
    {
        for ( int i=loadedRows; i<=row; i++ )
        {
        	Core::Types::Array_ptr row=iDriver->FetchRowFromResult(iDriverDescriptor,i);
            Internal->Set(i,(Core::ptr)row);
        }
        loadedRows=Internal->Count();
    }
    if ( row>Internal->Count() )
    {
        // Throw Not Found
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Row not found");
        std::string message="Row not found in query:" + row;
        throw exception;
    }
    Meow::Core::Types::Array_ptr rowdata=(Meow::Core::Types::Array_ptr)Internal->Get(row);
    return rowdata;
}

std::string Meow::Persistence::SQLResult::GetLastInsertID()
{
    return iLastInsertID;
}

std::string Meow::Persistence::SQLResult::ToXML( )
{
    std::string retval="<SQLResult><LastInsertID>" + iLastInsertID + "</LastInsertID><Cells>";
    for ( unsigned long i=0; i<Internal->Count(); i++ )
    {
        retval=retval + "<Row>";
        Core::Types::Array_ptr row=(Core::Types::Array_ptr)Internal->Get(i);
        Core::Types::Stack_ptr keys=(Core::Types::Stack_ptr)Internal->GetKeys();
        while ( !keys->IsEmpty() )
        {
        	Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
        	Core::Types::String_ptr value=(Core::Types::String_ptr)Internal->Get(key->GetValue());
        	Core::Types::String_ptr valueAsBase64=(Core::Types::String_ptr)Internal->Base64Encode();
            retval=retval + "<Cell name=\"" + key->GetValueAsString() + "\">" + valueAsBase64->GetValueAsString() + "</Cell>";
        }
        retval=retval + "</Row>";
    }
    retval=retval + "</Cells></SQLResult>";
    return retval;
}

void Meow::Persistence::SQLResult::Release( )
{
    if (iDriverDescriptor!=NULL)    
        iDriver->FreeResult(iDriverDescriptor);
    iDriverDescriptor=NULL;
}
