#include <Core/Types/Boolean.h>
#include <XML/ServiceParser.h>
Meow::XML::ServiceRecord::ServiceRecord()
{
	SetMeowClassName("XML/ServiceRecord");
	iGroups=new Core::Types::Array();
	iMethods=new Core::Types::Array();
	isFactory=false;
}
Meow::XML::ServiceRecord::~ServiceRecord()
{

}

Meow::XML::ServiceParser::ServiceParser()
{
	SetMeowClassName("XML/ServiceParser");
	iServices=new Core::Types::Array();
}
Meow::XML::ServiceParser::~ServiceParser()
{

}
void Meow::XML::ServiceParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	iCurrentElement=element;
	iCurrentCharacterData="";
	if ( element == "service" )
	{
		Core::Types::String_ptr iClass=(Core::Types::String_ptr)attributes->Get("name");
		iCurrentServiceRecord=new ServiceRecord();
		iCurrentServiceRecord->iServiceName=iClass->GetValueAsString();
	}
	else if ( element == "method" )
	{
		Core::Types::String_ptr iId=(Core::Types::String_ptr)attributes->Get("id");
		iCurrentServiceRecord->iMethods->Set(iId->GetValueAsString(),new Core::Types::Array());
		iCurrentMethod=iId->GetValueAsString();
	}

}
void Meow::XML::ServiceParser::EndElement( string element )
{
	if ( element == "class" )
	{
		iCurrentServiceRecord->iClassName=iCurrentCharacterData;		
	}
	else if ( element == "factory" )
	{
		iCurrentServiceRecord->iClassName=iCurrentCharacterData;
		iCurrentServiceRecord->isFactory=true;
	}
	else if ( element == "service" )
	{
		iServices->Set(iCurrentServiceRecord->iClassName,(Core::ptr)iCurrentServiceRecord);
	}
	else if ( element == "group" )
	{
		if ( iCurrentMethod.length() == 0 )
			iCurrentServiceRecord->iGroups->Set(iCurrentServiceRecord->Count(),new Core::Types::String(iCurrentCharacterData));
		else
		{
			Core::Types::Array_ptr methodGroupList=(Core::Types::Array_ptr)iCurrentServiceRecord->iMethods->Get(iCurrentMethod);
            methodGroupList->Set(methodGroupList->Count(),new Core::Types::String(iCurrentCharacterData));
            

		}
	}
	else if ( element == "method" )
	{
		iCurrentMethod="";
	}
}
void Meow::XML::ServiceParser::CharacterData( string data )
{
	iCurrentCharacterData.append(data);
}
Meow::Core::Types::Array_ptr Meow::XML::ServiceParser::GetServices()
{
	return iServices;
}
