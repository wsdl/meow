#include <Core/Types/Boolean.h>
#include <XML/RestParser.h>


Meow::XML::RestParser::RestParser()
{
	SetMeowClassName("XML/RestParser");
	iEnabled=false;
}
Meow::XML::RestParser::~RestParser()
{

}
void Meow::XML::RestParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	iCurrentElement=element;
	iCurrentCharacterData="";

}
void Meow::XML::RestParser::EndElement( string element )
{
	if ( element == "enabled" )
	{
		if ( iCurrentCharacterData == "true" )
			iEnabled=true;
	}

}
void Meow::XML::RestParser::CharacterData( string data )
{
	iCurrentCharacterData.append(data);
}
bool Meow::XML::RestParser::GetEnabled()
{
	return iEnabled;
}
