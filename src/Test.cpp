/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 * Stub for testing your classes.
 */

#include <Registry.h>
#include <PersistentObject.h>
#include <PersistenceFactory.h>
#include <Exception.h>
#include <Core/Types/String.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>
#include <Core/Types/Boolean.h>
#include <Core/Time/Date.h>
#include <Core/Log.h>
#include <Persistence/Query.h>
#include <Persistence/Database.h>
#include <Persistence/Insert.h>
#include <Persistence/Select.h>
#include <Persistence/Update.h>
#include <Persistence/Delete.h>
#include <Persistence/Where.h>
#include <Security/Session.h>
#include <Security/User.h>
#include <Security/Group.h>
#include <Security/GroupMember.h>
#include <Security/Log.h>
#include <Core/IO/Client.h>
#include <inc/configuration.h>
#include <iostream>
#include <Session.h>
#include <Core/Tools/XMLParser.h>
using namespace Meow;

class MyParser;
typedef Reference<MyParser> MyParser_ptr;
class MyParser : public Core::Tools::XMLParser
{
public:
	MyParser() { SetMeowClassName("MyParser"); }
	~MyParser() { }
	virtual void StartElement( string element, Core::Types::Array_ptr attributes )
	{
		cout << "StartElement(" << element << "," << attributes->GetValueAsString() << ")" << endl;
	}
	virtual void CharacterData( string data )
	{
		cout << "CharacterData(" << data << ")" << endl;
	}
	virtual void EndElement( string element )
	{
		Core::Types::Stack_ptr stack=GetElementStack();
		Core::Tools::XMLElement_ptr currentElement=(Core::Tools::XMLElement_ptr)stack->Top();
		cout << "Element!" << endl;
		cout << "name:" << currentElement->elementName << endl;
		cout << "attributes:" <<  currentElement->elementAttributes->GetValueAsString() << endl;
		cout << "character data:" << currentElement->characterData << endl;
	}
};

int main( int argc, char * * argv )
{
	Configuration_ptr conf=Configuration::GetConfiguration();
#ifdef WIN32
    	// We assume we are running with our working directory set - this should be read
    	// from the windows registry
    	conf->InstallFrom("etc\\meow\\ini.cnf");
    	conf->InstallFrom("etc\\meow\\commandline.cnf");
#else	
    	conf->InstallFrom("/etc/meow/ini.cnf");
    	conf->InstallFrom("/etc/meow/commandline.cnf");
#endif
	// Reconfigure the log based on what we know from the configuration file.
	Core::Log::GetLog()->Reconfigure();

	// Install the registry.
	Registry_ptr registry=Registry::GetRegistry();
	// Add the built in classes
	registry->RegisterMe(new SessionFactory());
	registry->RegisterMe(new Persistence::DatabaseFactory());
	registry->RegisterMe(new Persistence::InsertFactory());
	registry->RegisterMe(new Persistence::SelectFactory());
	registry->RegisterMe(new Persistence::UpdateFactory());
	registry->RegisterMe(new Persistence::DeleteFactory());
	registry->RegisterMe(new Persistence::WhereFactory());
	registry->RegisterMe(new Security::UserFactory());
	registry->RegisterMe(new Security::GroupFactory());
	registry->RegisterMe(new Security::GroupMemberFactory());
	registry->RegisterMe(new Security::LogFactory());
	registry->RegisterMe(new Security::SessionFactory());

	registry->InstallExtensions();
	// Set up the session
	Session_ptr session=new Session();
	session->PushPrivileges(new Security::User());
	registry->SetCurrentSession((Object_ptr)session);
	
	try
	{
		registry->Begin()
		// START YOUR CODE HERE
		MyParser_ptr parser=new MyParser();
		
	
	} catch ( Exception &excep )
	{
		cout << excep.Diagnostic() << endl; 
		registry->Rollback();
	} 
	registry->FireDelayedEvents();     
    registry->Passivate();
    registry->Cleanup( );
}
