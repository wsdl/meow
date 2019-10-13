#include <Security/Log.h>
#include <Registry.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Array.h>
#include <Core/Types/Void.h>
#include <Core/Time/Date.h>

Meow::Security::LogFactory::LogFactory()
{
    SetMeowClassName("Security/LogFactory");
    AddSerialKeyField("internalkey","InternalKey","Unique ID");
    AddAttributeField("when",Factory::Date,new Core::Time::Date(),0,"When","Guests","Guests","When it happenned");
    AddAttributeField("action",Factory::String,new Core::Types::String(""),0,"Action","Guests","Guests","Action",true);
    AddAttributeField("username",Factory::String,new Core::Types::String(""),0,"Username","Guests","Guests","Username",true);
    AddAttributeField("outcome",Factory::String,new Core::Types::String(""),0,"Outcome","Guests","Guests","Outcome",true);
	Core::Types::Array_ptr recordParameters=new Core::Types::Array();
	recordParameters->Set("action",new Core::Types::String("String"));
	recordParameters->Set("username",new Core::Types::String("String"));
	recordParameters->Set("outcome",new Core::Types::String("String"));
	AddCallableMethod("Record",recordParameters);

}
Meow::Security::LogFactory::~LogFactory()
{
	
}
Meow::LifecycleObject_ptr Meow::Security::LogFactory::GetInstance( std::string service )
			
{
	return new Meow::Security::Log();
}
std::string Meow::Security::LogFactory::GetFriendlyName()
{
	return "Security Access Log";
}
bool Meow::Security::LogFactory::IsPublicApplication()
{
	return true;
}
void Meow::Security::LogFactory::RegisterEvents()
{
    Core::Types::Array_ptr parameters=new Core::Types::Array();
    Factory_ptr factory=this;
    GetRegistry()->Subscribe("LoginOutcome",factory,parameters);
}
void Meow::Security::LogFactory::OnEvent( std::string event, Meow::Core::Types::Array_ptr parameters )
{
	if ( !event.compare("LoginOutcome") )
		RecordLoginAttempt(parameters);
}

void Meow::Security::LogFactory::RecordLoginAttempt( Core::Types::Array_ptr parameters )
{
	if ( parameters->IsSet("Action") && parameters->IsSet("Username") && parameters->IsSet("Outcome") )
	{
		Core::Types::ValueType_ptr Action=(Core::Types::ValueType_ptr)parameters->Get("Action");
		Core::Types::ValueType_ptr Username=(Core::Types::ValueType_ptr)parameters->Get("Username");
		Core::Types::ValueType_ptr Outcome=(Core::Types::ValueType_ptr)parameters->Get("Outcome");
		Meow::Security::Log_ptr retval = (Meow::Security::Log_ptr)Factory::GetRegistry()->GetMe("Security/Log");
		retval->Record(Action->GetValueAsString(),Username->GetValueAsString(),Outcome->GetValueAsString());
	}
}

Meow::Security::Log::Log()
{
	SetMeowClassName("Security/Log");
}
Meow::Security::Log::~Log()
{
}

void Meow::Security::Log::SetWhen( Core::Time::Date_ptr whentime )
{
	Setter();
	Object::SetVariable("when",(Core::ptr)whentime);
	MarkDirty();
}
void Meow::Security::Log::SetAction( std::string action )
{
	Setter();
	Object::SetVariable("action",new Core::Types::String(action));
	MarkDirty();
}
void Meow::Security::Log::SetUsername( std::string uname )
{
	Setter();
	Object::SetVariable("username",new Core::Types::String(uname));
	MarkDirty();
}
void Meow::Security::Log::SetOutcome( std::string outcome )
{
	Setter();
	Object::SetVariable("outcome",new Core::Types::String(outcome));
	MarkDirty();
}

Meow::Core::Time::Date_ptr Meow::Security::Log::GetWhen()
{
	Getter();
	Core::Time::Date_ptr retval=(Core::Time::Date_ptr)Object::GetVariable("when");
	return retval;
}
std::string Meow::Security::Log::GetAction()
{
	Getter();
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)Object::GetVariable("action");
	return retval->GetValueAsString();
}
std::string Meow::Security::Log::GetUsername()
{
	Getter();
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)Object::GetVariable("username");
	return retval->GetValueAsString();
}
std::string Meow::Security::Log::GetOutcome( )
{
	Getter();
	Core::Types::ValueType_ptr retval=(Core::Types::ValueType_ptr)Object::GetVariable("outcome");
	return retval->GetValueAsString();
}
void Meow::Security::Log::Record( std::string action, std::string username, std::string outcome )
{
	SetWhen(new Core::Time::Date());
	SetAction(action);
	SetUsername(username);
	SetOutcome(outcome);
}
Meow::Core::ptr Meow::Security::Log::ExecuteMethod( std::string methodname, Meow::Core::Types::Array_ptr parameters )
{
	Meow::Core::ptr retval;
	if ( !methodname.compare("Record") )
	{
		if ( ! parameters->IsSet(0) && ! parameters->IsSet(1) && ! parameters->IsSet(2) )
		{
			Exceptions::NotFound nf;
			nf.SetMessage("Insufficient arguments for Record operation");
		}
		Core::Types::ValueType_ptr parameter0=(Core::Types::ValueType_ptr)parameters->Get(0);
		Core::Types::ValueType_ptr parameter1=(Core::Types::ValueType_ptr)parameters->Get(1);
		Core::Types::ValueType_ptr parameter2=(Core::Types::ValueType_ptr)parameters->Get(2);
		Record(parameter0->GetValueAsString(),parameter1->GetValueAsString(),parameter1->GetValueAsString());
	}
	else
		PersistentObject::ExecuteMethod(methodname,parameters);

	return retval;
}
