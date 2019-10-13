#include <XML/ObjectParser.h>

Meow::XML::ObjectAttribute::ObjectAttribute()
{
	SetMeowClassName("XML/ObjectAttribute");
	iIsKey=false;
}
Meow::XML::ObjectAttribute::~ObjectAttribute()
{

}
Meow::XML::ObjectRelationship::ObjectRelationship()
{
	SetMeowClassName("XML/ObjectRelationship");
	iBindings=new Core::Types::Array();
}

Meow::XML::ObjectRelationship::~ObjectRelationship()
{
}
Meow::XML::ObjectBond::ObjectBond()
{
	SetMeowClassName("XML/ObjectBond");
	iBindingType=attributebind;
}
Meow::XML::ObjectBond::~ObjectBond()
{

}
Meow::XML::ObjectParser::ObjectParser()
{
	SetMeowClassName("XML/ObjectParser");
}
Meow::XML::ObjectParser::~ObjectParser()
{
}
void Meow::XML::ObjectParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
}
void Meow::XML::ObjectParser::EndElement( string element )
{
}
void Meow::XML::ObjectParser::CharacterData( string data )
{
}
std::string  Meow::XML::ObjectParser::GetDefinitionClassName()
{
	return iDefinitionClassName;
}
Meow::Core::Types::Array_ptr Meow::XML::ObjectParser::GetAttributes()
{
	return iAttributes;
}
Meow::Core::Types::Array_ptr Meow::XML::ObjectParser::GetRelationships()
{
	return iRelationships;
}

