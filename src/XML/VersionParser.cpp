#include <XML/VersionParser.h>
#include <Core/Types/Boolean.h>
#include <Core/Time/Date.h>

Meow::XML::VersionParser::VersionParser()
{
	SetMeowClassName("XML/VersionParser");
	iComponents=new Core::Types::Array();

}
Meow::XML::VersionParser::~VersionParser()
{
	
}
void Meow::XML::VersionParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	if ( element ==  "component" )
	{
		Core::Types::ValueType_ptr name=(Core::Types::ValueType_ptr)attributes->Get("name");	
		Core::Types::ValueType_ptr currentVersion=(Core::Types::ValueType_ptr)attributes->Get("currentversion");
		Core::Types::ValueType_ptr installed=(Core::Types::ValueType_ptr)attributes->Get("installed");
		Version_ptr componentRecord=new Version();
		componentRecord->currentVersion=currentVersion->GetValueAsInteger();
		componentRecord->installed=installed->GetValueAsInteger();
		iComponents->Set(name->GetValueAsString(),(Core::ptr)componentRecord);
	}
}
void Meow::XML::VersionParser::EndElement( string element )
{
		
}
void Meow::XML::VersionParser::CharacterData( string data )
{
}
Meow::Core::Types::Array_ptr Meow::XML::VersionParser::GetComponents()
{
	return iComponents;
}
void Meow::XML::VersionParser::UpdateComponent( std::string key, int version )
{
	Core::Time::Date_ptr current=Core::Time::Date::GetCurrentTime();
	if ( iComponents->IsSet(key) )
	{
		Version_ptr record=(Version_ptr)iComponents->Get(key);
		record->currentVersion=version;
		record->installed=current->GetValueAsInteger();
	}
	else
	{
		Version_ptr record=new Version();
		record->currentVersion=version;
		record->installed=current->GetValueAsInteger();
		iComponents->Set(key,(Core::ptr)record);
	}
	
}
std::string Meow::XML::VersionParser::ToString()
{
	std:string xml="<versions>";
	Core::Types::Stack_ptr keys=iComponents->GetKeys();
	while ( !keys->IsEmpty() )
	{
		Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
		Version_ptr record=(Version_ptr)iComponents->Get(key->GetValueAsString());
		xml+="<component name=\"" + key->GetValueAsString() + "\" currentversion=\"" + 
			Core::Types::ValueType::IntegerToString(record->currentVersion) + "\" installed=\"" +
				Core::Types::ValueType::IntegerToString(record->installed) + "\"/>";		
	}
	xml+="</versions>";
	return xml;
}
int Meow::XML::VersionParser::GetVersion( std::string key )
{
	int retval=-1;
	Version_ptr record=(Version_ptr)iComponents->Get(key);
	if ( !record.IsNull() )
	{
		retval=record->currentVersion;
	}
	return retval;
}
Meow::XML::Version::Version()
{
	SetMeowClassName("XML/Version");
}
Meow::XML::Version::~Version()
{
}
