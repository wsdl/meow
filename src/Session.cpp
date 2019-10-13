#include <Session.h>
#include <Registry.h>
#include <Security/User.h>
#include <Core/Types/Integer.h>
#include <iostream>
#include <Core/Time/Date.h>
#include <Core/Types/Boolean.h>
#include <Persistence/Where.h>
#include <Persistence/Insert.h>
#include <Persistence/Update.h>
#include <Persistence/Delete.h>
#include <Persistence/Select.h>
#include <inc/configuration.h>
#include <Core/System/Thread.h>


Meow::SessionFactory::SessionFactory()
{
    SetMeowClassName("SessionFactory");

}

Meow::SessionFactory::~SessionFactory()
{
}

Meow::LifecycleObject_ptr Meow::SessionFactory::GetInstance( std::string service )
{
    LifecycleObject_ptr instance=(LifecycleObject*)new Session();
    return instance;
}

Meow::Session_ptr Meow::SessionFactory::GetCurrentSession()
{
    Session_ptr retval;
    if ( iCurrent.IsNull() )
    {
    	bool postAccessExpired=false;
		Core::ptr From;
		Core::ptr To;
        std::string sessionid;
        Core::Time::Date_ptr now=Core::Time::Date::GetCurrentTime();
        bool createsession=false;
        Core::Object_ptr threadStorage=Core::System::Thread::GetContext();
				
        if ( threadStorage.IsNull() )
        {
			Core::Log::GetLog()->Information("Meow::SessionFactory::GetCurrentSession","s","Thread Storage is null");
        	threadStorage=new Core::Types::Array();
        	Core::System::Thread::SetContext(threadStorage);
        }
				
        Core::Types::Array_ptr threadStorageAsArray=(Core::Types::Array_ptr)threadStorage;
        // We can't leave here without a valid session.
        while ( retval.IsNull() )
        {
			if ( !threadStorageAsArray->IsSet("session.id") )
			{
				Core::Log::GetLog()->Information("Meow::SessionFactory::GetCurrentSession","s","Thread Storage, need to create session id");
				char q;
				bool retry=true;
				while ( retry )
				{
					Core::Types::Integer_ptr random=Core::Types::Integer::GetRandomNumber();
					sessionid="";
					sessionid.append(now->GetValueAsString());
					sessionid.append(random->GetValueAsString());
					Core::Types::String_ptr sessionSHA=new Core::Types::String(sessionid);
					sessionid=sessionSHA->SHA256();
					threadStorageAsArray->Set("session.id",new Core::Types::String(sessionid));
					Core::Log::GetLog()->Information("Meow::SessionFactory::GetCurrentSession","s","Thread Storage, Set new session id");
					try
					{           	    		
						retval=(Session_ptr)GetRegistry()->CreateMe("Session",1,sessionid.c_str());
						retry=false;
					} catch ( Exceptions::Duplicate &dup )
					{
						Core::Log::GetLog()->Warning("Meow::SessionFactory::GetCurrentSession","ss","Session id exists:",sessionid.c_str());
					}           	    	
			   }
			}
			else
			{
				Core::Types::ValueType_ptr tlsSessionId=(Core::Types::ValueType_ptr)threadStorageAsArray->Get("session.id");
				sessionid=tlsSessionId->GetValueAsString();
				try
				{
					retval=(Session_ptr)GetRegistry()->FindMe("Session",1,sessionid.c_str());
				} catch ( Exceptions::NotFound &nf )
				{
					// OK we have to invalidate the sessionid
					threadStorageAsArray->Unset("session.id");
				}
			}
		}
		Core::Log::GetLog()->Information("Meow::SessionFactory::GetCurrentSession","s","SessionReady");
        GetRegistry()->SessionReady();
        iCurrent=retval;
		if ( postAccessExpired )
		{
				Core::Types::Array_ptr parameters=new Core::Types::Array();
				parameters->Set("From",From);
				parameters->Set("To",To);
				GetRegistry()->PostEvent("AccessExpired",parameters);
		}
		try
		{
			Core::Types::ValueType_ptr uid=(Core::Types::ValueType_ptr)retval->GetVariable("current.uid");
            if ( !uid.IsNull() )
            {
            	Meow::Core::Log::GetLog()->ReconfigureFrom("user." + uid->GetValueAsString() );
            	Core::Types::Array_ptr evParms=new Core::Types::Array();
          		evParms->Set("current.uid",(Core::ptr)uid);
          		Registry::GetRegistry()->PostEvent("UserSetup",evParms);
			}						
		} catch ( Exceptions::NotFound &nf )
		{
			
		}        
    }
    else
        retval=iCurrent;
    return retval;
}

void Meow::SessionFactory::DestroyAllSessionsForUser( Meow::Object_ptr user )
{
// TODO: Implement this
}
void Meow::SessionFactory::CanWrite() 

{
}
void Meow::SessionFactory::CanRead()
{
}
void Meow::SessionFactory::CanCreate()
{
}
void Meow::SessionFactory::CanDelete()
{
}

Meow::Session::Session()
{
    SetMeowClassName("Session");
    Core::Time::Date_ptr base=new Core::Time::Date();
    iPrivs=new Core::Types::Stack();
	iFrameworkMessages=new Core::Types::Queue();
	iDataCache=new Core::Types::Array();
    SetVariable("Baseline",(Core::ptr)base);

}
Meow::Session::~Session()
{
}

std::string Meow::Session::GetSessionID()
{
    Core::Types::ValueType_ptr id=(Core::Types::ValueType_ptr)GetUniqueKey()->Get(0);
    if ( !id.IsNull() )
        return id->GetValueAsString();
    else
        return "n/a";
}

Meow::Object_ptr  Meow::Session::GetCurrentUser()
{
	Meow::Object_ptr user;
	if ( iPrivs.IsNull() )
	{
		return user;
	}
	if ( !iPrivs->IsEmpty() )
	{
		user=(Meow::Object_ptr)iPrivs->Top();
	}
	if ( user.IsNull() )
	{
		if ( ExistsVariable("current.user") )
		{
			Core::Types::ValueType_ptr currentUserAsString=(Core::Types::ValueType_ptr)GetVariable("current.user");
			try
			{
				user=GetFactory()->GetRegistry()->FindMeByObjectReference(currentUserAsString->GetValueAsString());
				iPrivs->Push((Core::ptr)user);
			} catch ( Exceptions::NotFound &nf )
			{
				Core::Log::GetLog()->Error("Meow::Session::GetCurrentUser","ssss","Current user ",currentUserAsString->GetValueAsString().c_str()," cannot be restored:",nf.Diagnostic().c_str());
			}
		}
		else
		{
			Core::Log::GetLog()->Error("Meow::Session::GetCurrentUser","s","No current user installed from session, user will be treated anonymously ");
			
		}
	}
	return user;
}
void Meow::Session::PushPrivileges( Meow::Object_ptr user )
{
	SetVariable("current.user",new Core::Types::String(user->ToString()));
	iPrivs->Push((Core::ptr)user);
}
void Meow::Session::PopPrivileges( )
{
	Core::ptr lUser;
	if ( !iPrivs->IsEmpty() )
	{
		lUser=iPrivs->Pop();
		if ( !iPrivs->IsEmpty() )
		{
			lUser=iPrivs->Top();
			SetVariable("current.user",new Core::Types::String(((Meow::Object_ptr)lUser)->ToString()));
		}
		else
			SetVariable("current.user",new Core::Types::String(""));	
	}
}
void Meow::Session::PushFrameworkMessage( std::string component, std::string eventname, std::string message )
{
	FrameworkMessage_ptr lMessage=new FrameworkMessage();
	lMessage->iComponent=component;
	lMessage->iEventName=eventname;
	lMessage->iMessage=message;
	iFrameworkMessages->Push((Core::ptr)lMessage);
}
std::string Meow::Session::ToString()
{
	std::string retval;
	while ( !iFrameworkMessages->IsEmpty() )
	{
		FrameworkMessage_ptr message=(FrameworkMessage_ptr)iFrameworkMessages->Pull();
		retval.append(message->ToString());
	}
	return retval;
}
void Meow::Session::ClearFrameworkMessages()
{
	iFrameworkMessages=new Core::Types::Queue();
}
Meow::FrameworkMessage::FrameworkMessage()
{
	SetMeowClassName("FrameworkMessage");
}
Meow::FrameworkMessage::~FrameworkMessage()
{

}
std::string Meow::FrameworkMessage::ToString()
{
	std::string retval="<SM>";
		retval.append("<SMC>" + iComponent + "</SMC>");
		retval.append("<SME>" + iEventName + "</SME>");
		retval.append("<SMD>" + iMessage + "</SMD>");
	retval.append("</SM>");
	return retval;	
}
bool Meow::Session::IsPurgeable( )
{
	return true;
}
void    Meow::Session::Restore( )
{
	Core::Log::GetLog()->Information("Meow::Session::Restore","s","Restore");
	std::string passivator="Security/Session";
	if ( Configuration::GetConfiguration()->IsSet("session.passivator") )
		passivator=Configuration::GetConfiguration()->Get("session.passivator");
	if ( passivator.length() > 0 )
	{
		PersistentObject_ptr sessionObject=(PersistentObject_ptr)GetFactory()->GetRegistry()->FindMe(passivator,1,GetSessionID().c_str());
		sessionObject->SetPublicVariable("LastTouched",new Core::Time::Date());
		Core::Types::Array_ptr sessionData=(Core::Types::Array_ptr)sessionObject->GetPublicVariable("SessionData");
		Core::Types::Stack_ptr variableKeys=sessionData->GetKeys();
		// Unpack the session
		while ( !variableKeys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)variableKeys->Pop();
			SetVariable(key->GetValueAsString(),sessionData->Get(key->GetValueAsString()));
		}
	}
	else
		Core::Log::GetLog()->Error("Meow::Session::Restore","s","Passivator duff.");
}
void    Meow::Session::Passivate( )
{
	std::string passivator="Security/Session";
	if ( Configuration::GetConfiguration()->IsSet("session.passivator") )
		passivator=Configuration::GetConfiguration()->Get("session.passivator");
	if ( passivator.length() > 0 )
	{
		PersistentObject_ptr sessionObject=(PersistentObject_ptr)GetFactory()->GetRegistry()->FindMe(passivator,1,GetSessionID().c_str());
		sessionObject->SetPublicVariable("LastTouched",new Core::Time::Date());
//			Core::Types::Array_ptr sessionData=(Core::Types::Array_ptr)sessionObject->GetPublicVariable("SessionData");
		Core::Types::Array_ptr sessionData=new Core::Types::Array();

		Core::Types::Stack_ptr variableKeys=GetVariableKeys();
		while ( !variableKeys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)variableKeys->Pop();
			sessionData->Set(key->GetValueAsString(),GetVariable(key->GetValueAsString()));
		}
		sessionObject->SetPublicVariable("SessionData",(Core::ptr)sessionData);
		sessionObject->Passivate();
	}
	else
		Core::Log::GetLog()->Error("Meow::Session::Restore","s","Passivator duff.");
}
void Meow::Session::_Delete( )
{
	std::string passivator="Security/Session";
	if ( Configuration::GetConfiguration()->IsSet("session.passivator") )
		passivator=Configuration::GetConfiguration()->Get("session.passivator");
	if ( passivator.length() > 0 )
	{
		GetFactory()->GetRegistry()->DestroyMe(passivator,1,GetSessionID().c_str());		
	}
	else
		Core::Log::GetLog()->Error("Meow::Session::_Delete","s","Passivator duff.");
}
void Meow::Session::Create( )
{
	
	std::string passivator="Security/Session";
	if ( Configuration::GetConfiguration()->IsSet("session.passivator") )
		passivator=Configuration::GetConfiguration()->Get("session.passivator");
	if ( passivator.length() > 0 )
	{
		PersistentObject_ptr sessionObject=(PersistentObject_ptr)GetFactory()->GetRegistry()->CreateMe(passivator,1,GetSessionID().c_str());
		sessionObject->SetPublicVariable("LastTouched",new Core::Time::Date());
		Core::Types::Array_ptr sessionData=(Core::Types::Array_ptr)sessionObject->GetPublicVariable("SessionData");
		Core::Types::Stack_ptr variableKeys=sessionData->GetKeys();
		// Unpack the session
		while ( !variableKeys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)variableKeys->Pop();
			SetVariable(key->GetValueAsString(),sessionData->Get(key->GetValueAsString()));
		}
	}
	else
		Core::Log::GetLog()->Error("Meow::Session::Create","s","Passivator duff.");
}


void Meow::Session::SetVariable( std::string varname, Core::ptr value )
{
	MarkDirty();
	iDataCache->Set(varname,value);
}

void Meow::Session::UnSetVariable( std::string varname ) 
{
	MarkDirty();
	iDataCache->Unset(varname);
}

Meow::Core::ptr Meow::Session::GetVariable( std::string varname ) 
{

	return iDataCache->Get(varname);
}
bool Meow::Session::ExistsVariable( std::string varname ) 
{
	return iDataCache->IsSet(varname);
}
Meow::Core::Types::Stack_ptr Meow::Session::GetVariableKeys() 
{
	return iDataCache->GetKeys();
}
Meow::Core::Types::Queue_ptr Meow::Session::GetFrameworkMessages()
{
    return iFrameworkMessages;
}
