																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													#include <Core/Types/Boolean.h>
#include <XML/UserParser.h>


Meow::XML::UserParser::UserParser()
{																					
	SetMeowClassName("XML/UserParser");
	iUsers=new Core::Types::Array();
}
Meow::XML::UserParser::~UserParser()
{

}
void Meow::XML::UserParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	iCurrentElement=element;
	iCurrentCharacterData="";
	if ( element == "user" )
	{

		iCurrentUserId="";
		iCurrentUserName="";
		iCurrentPassword="";
		iCurrentEmail="";
		iCurrentGroups=new Core::Types::Array();
		iCurrentObjectReference="";
	}
}
void Meow::XML::UserParser::EndElement( string element )
{
	if ( element == "user" )
	{
		Core::Types::Array_ptr record=new Core::Types::Array();
		record->Set("uid",new Core::Types::String(iCurrentUserId));
		record->Set("username",new Core::Types::String(iCurrentUserName));
		record->Set("password",new Core::Types::String(iCurrentPassword));
		record->Set("email",new Core::Types::String(iCurrentEmail));
		record->Set("memberships",(Core::ptr)iCurrentGroups);
		record->Set("objectreference",new Core::Types::String(iCurrentObjectReference));
		iUsers->Set(iCurrentUserId,(Core::ptr)record);
	}
	else if ( element == "uid" )
	{
		iCurrentUserId=iCurrentCharacterData;
	}
	else if ( element == "username" )
	{
		iCurrentUserName=iCurrentCharacterData;
	}
	else if ( element == "email" )
	{
		iCurrentEmail=iCurrentCharacterData;
	}
	else if ( element == "password" )
	{
		iCurrentPassword=iCurrentCharacterData;
	}
	else if ( element == "group" )
	{
		iCurrentGroups->Set(iCurrentCharacterData,new Core::Types::Boolean(true));
	}
	else if ( element == "objectreference" )
	{
		iCurrentObjectReference=iCurrentCharacterData;
	}
}
void Meow::XML::UserParser::CharacterData( string data )
{
	iCurrentCharacterData.append(data);
}
Meow::Core::Types::Array_ptr Meow::XML::UserParser::GetUsers()
{
	return iUsers;
}
