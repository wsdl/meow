#include <XML/DomainParser.h>
#include <Core/Types/Boolean.h>

Meow::XML::DomainParser::DomainParser()
{
	SetMeowClassName("XML/DomainParser");
	iBanned=new Core::Types::Array();
	iPermitted=new Core::Types::Array();
	iConfigurationFile="";
	iAvailable=false;
}
Meow::XML::DomainParser::~DomainParser()
{

}
void Meow::XML::DomainParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	iCurrentElement=element;
	iCurrentCharacterData="";
}
void Meow::XML::DomainParser::EndElement( string element )
{
	if ( element ==  "configuration" )
		iConfigurationFile=iCurrentCharacterData;
	else if ( element == "banned" )
		iBanned->Set(iCurrentCharacterData,new Core::Types::Boolean(true));
	else if ( element == "permitted" )
		iPermitted->Set(iCurrentCharacterData,new Core::Types::Boolean(true));
	else if ( element == "available" )
	{
		if ( iCurrentCharacterData == "true" )
			iAvailable=true;
	}
}
void Meow::XML::DomainParser::CharacterData( string data )
{
	iCurrentCharacterData.append(data);
}
Meow::Core::Types::Array_ptr Meow::XML::DomainParser::GetBanned()
{
	return iBanned;
}
Meow::Core::Types::Array_ptr Meow::XML::DomainParser::GetPermitted()
{
	return iPermitted;
}
std::string			   Meow::XML::DomainParser::GetConfigurationFile()
{
	return iConfigurationFile;
}
bool				   Meow::XML::DomainParser::GetAvailable()
{
	return iAvailable;
}
