#include <XML/GroupParser.h>
#include <Core/Types/Boolean.h>

Meow::XML::GroupParser::GroupParser()
{
	SetMeowClassName("XML/GroupParser");
	iGroups=new Core::Types::Array();
}
Meow::XML::GroupParser::~GroupParser()
{

}
void Meow::XML::GroupParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	iCurrentElement=element;
	iCurrentCharacterData="";
	if ( element == "group" )
	{
		Core::Types::String_ptr id=new Core::Types::String();
		if ( attributes->IsSet("id") )
		{
			id=(Core::Types::String_ptr)attributes->Get("id");
		}
		Core::Types::String_ptr description=new Core::Types::String();
		if ( attributes->IsSet("description") )
		{
			description=(Core::Types::String_ptr)attributes->Get("description");
		}
		iGroups->Set(id->GetValueAsString(),(Core::ptr)description);
	}
}
void Meow::XML::GroupParser::EndElement( string element )
{

}
void Meow::XML::GroupParser::CharacterData( string data )
{
	iCurrentCharacterData.append(data);
}
Meow::Core::Types::Array_ptr Meow::XML::GroupParser::GetGroups()
{
	return iGroups;
}

