#include <Factory.h>
#include <Registry.h>
#include <iostream>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>
#include <Core/Types/Boolean.h>
#include <Core/Time/Date.h>
#include <inc/configuration.h>
#include <stdio.h>
#include <UncollectedFactory.h>
#include <Session.h>
#include <Security/User.h>
#include <Security/Group.h>
#include <PersistentObject.h>


Meow::Factory::Factory( )
{
    SetMeowClassName("Factory");
    ldContainer=new Core::Types::Array();
    Services=new Core::Types::Array();
    objectsHeld=0;
    Joins=new Core::Types::Array();
    iSchemeKeys=new Core::Types::Array();
    iSchemeAttributes=new Core::Types::Array();
    iSerial=false;
    iKeySet=false;
    iAttributeQueue=new Core::Types::Queue();
    iVersion=-1;
    iSchemeAttributesByAlias=new Core::Types::Array();
    iSchemeKeysByAlias=new Core::Types::Array();
    iSearchableParts=new Core::Types::Array();
    iCallableMethodList=new Core::Types::Array();
    iCallableFactoryMethodList=new Core::Types::Array();
    iDescendingOrder=false;
    iShowInLists=new Core::Types::Array();
    iOnCreateEvents=new Core::Types::Array();
    iOnDeleteEvents=new Core::Types::Array();
    iOnUpdateEvents=new Core::Types::Array();
//    iSecurityType=Factory::Plain;
}
Meow::Factory::~Factory( )
{

}
void Meow::Factory::OnCreate( std::string eventName )
{
    iOnCreateEvents->Set(iOnCreateEvents->Count(),new Core::Types::String(eventName));
}
void Meow::Factory::OnUpdate( std::string eventName )
{
    iOnUpdateEvents->Set(iOnUpdateEvents->Count(),new Core::Types::String(eventName));
}
void Meow::Factory::OnDelete( std::string eventName )
{
    iOnDeleteEvents->Set(iOnDeleteEvents->Count(),new Core::Types::String(eventName));
}
		
void Meow::Factory::LoadShowInListsDefault()
{
	// Get the company default
	bool somethingFound=false;
	if ( iShowInLists->Count() == 0 )
	{
		try
		{
			std::string key="factory." + GetMeowClassName() + ".ShowInLists";
			PersistentObject_ptr props=(PersistentObject_ptr)GetRegistry()->FindMe("Service/Props",1,key.c_str());
			Core::Types::ValueType_ptr propData=(Core::Types::ValueType_ptr)props->GetPublicVariable("Value");
			if ( !propData.IsNull() && propData->Count() > 0 )
			{
				iShowInLists=new Core::Types::Array();
				iShowInLists->Import(propData->GetValueAsString());	
				somethingFound=true;
				Core::Log::GetLog()->Warning("Meow::Factory::LoadShowInListsDefault","ss","Company overlay:",iShowInLists->GetValueAsString().c_str());
			}
		} catch ( Exceptions::NotFound &nf )
		{
			Core::Log::GetLog()->Warning("Meow::Factory::LoadShowInListsDefault","ss","Company overlay failed:",nf.Diagnostic().c_str());
		}	
	}
	// Who am I?
	Session_ptr session=(Session_ptr)GetRegistry()->GetCurrentSession();
	if ( !session.IsNull() )
	{
		Security::User_ptr user=(Security::User_ptr)session->GetCurrentUser();
		if ( !user.IsNull() )
		{
			Core::Types::ValueType_ptr userIdentifier=new Core::Types::Integer(user->GetUid());
			try
			{
				std::string key="factory." + GetMeowClassName() + ".ShowInLists";
				PersistentObject_ptr props=(PersistentObject_ptr)GetRegistry()->FindMe("Service/UserProps",2,userIdentifier->GetValueAsString().c_str(),key.c_str());
				Core::Types::ValueType_ptr propData=(Core::Types::ValueType_ptr)props->GetPublicVariable("Value");
				if ( !propData.IsNull() && propData->Count() > 0 )
				{
					iShowInLists=new Core::Types::Array();
					iShowInLists->Import(propData->GetValueAsString());	
					somethingFound=true;
					Core::Log::GetLog()->Warning("Meow::Factory::LoadShowInListsDefault","ss","User overlay:",iShowInLists->GetValueAsString().c_str());
				}
			} catch ( Exceptions::NotFound &nf )
			{
				Core::Log::GetLog()->Warning("Meow::Factory::LoadShowInListsDefault","ss","User overlay failed:",nf.Diagnostic().c_str());
			}
		}
		else
			Core::Log::GetLog()->Warning("Meow::Factory::LoadShowInListsDefault","s","Current user is null, so no user overlay loaded.");
	}
	else
		Core::Log::GetLog()->Warning("Meow::Factory::LoadShowInListsDefault","s","Session is null, so no user overlay loaded.");
	// OK so lets iterate, and if we find the field name , switch it on.
	if ( somethingFound )
	{
		Core::Types::Array_ptr attributes;
		attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());

			if ( iShowInLists->IsSet(field->iFieldName) )
			{
				Core::Types::ValueType_ptr setting=(Core::Types::ValueType_ptr)iShowInLists->Get(field->iFieldName);
				field->iShowInList=setting->GetValueAsBoolean();;				
			}
			else
				field->iShowInList=false;
		}
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();

		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());

			if ( iShowInLists->IsSet(field->iFieldName) )
			{
				Core::Types::ValueType_ptr setting=(Core::Types::ValueType_ptr)iShowInLists->Get(field->iFieldName);
				field->iShowInList=setting->GetValueAsBoolean();;				
			}
			else
				field->iShowInList=false;
		}
	}
}

void Meow::Factory::SaveShowInListSettingsForAll( Core::Types::Array_ptr settings )
{
	iShowInLists=settings;
	try
	{
		std::string key="factory." + GetMeowClassName() + ".ShowInLists";
		PersistentObject_ptr props=(PersistentObject_ptr)GetRegistry()->GetMe("Service/Props",1,key.c_str());
		props->SetPublicVariable("Value",(Core::ptr)settings);
		props->Passivate();
	} catch ( Exceptions::NotFound &nf )
	{
		Core::Log::GetLog()->Warning("Meow::Factory::SaveShowInListsSettingsForAll","ss","Company overlay save failed:",nf.Diagnostic().c_str());
	}
}
void Meow::Factory::SaveShowInListSettingsForCurrentUser( Core::Types::Array_ptr settings )
{
	iShowInLists=settings;	
	Session_ptr session=(Session_ptr)GetRegistry()->GetCurrentSession();
	if ( !session.IsNull() )
	{
		Security::User_ptr user=(Security::User_ptr)session->GetCurrentUser();
		if ( !user.IsNull() )
		{
			Core::Types::ValueType_ptr userIdentifier=new Core::Types::Integer(user->GetUid());
			try
			{
				std::string key="factory." + GetMeowClassName() + ".ShowInLists";
				PersistentObject_ptr props=(PersistentObject_ptr)GetRegistry()->GetMe("Service/UserProps",2,userIdentifier->GetValueAsString().c_str(),key.c_str());
				props->SetPublicVariable("Value",(Core::ptr)settings);
				props->Passivate();
			} catch ( Exceptions::NotFound &nf )
			{
				Core::Log::GetLog()->Warning("Meow::Factory::SaveShowInListsSettingsForCurrentUser","ss","User overlay saved:",nf.Diagnostic().c_str());
			}
		}
		else
			Core::Log::GetLog()->Warning("Meow::Factory::SaveShowInListsSettingsForCurrentUser","s","Current user is null, so no user overlay saved.");
	}
	else
		Core::Log::GetLog()->Warning("Meow::Factory::SaveShowInListsSettingsForCurrentUser","s","Session is null, so no user overlay saved.");

}
Meow::Core::Types::Array_ptr Meow::Factory::GetShowInListsSettings()
{
	return iShowInLists;
}
bool Meow::Factory::GetSerial()
{
	return iSerial;
}
bool Meow::Factory::GetKeySet()
{
	return iKeySet;
}
Meow::Core::Types::Array_ptr Meow::Factory::GetSchemeKeys()
{
	return iSchemeKeys;
}
Meow::Core::Types::Array_ptr Meow::Factory::GetSchemeAttributes()
{
	return iSchemeAttributes;
}

void Meow::Factory::InitialiseSecurity() 
{
	try
	{
		SetMinimumSecurity((Object_ptr)GetRegistry()->FindMe("Security/Group",1,Configuration::GetConfiguration()->Get(GetMeowClassName() + "." + "minimum_security").c_str()));
		SetFindSecurity((Object_ptr)GetRegistry()->FindMe("Security/Group",1,Configuration::GetConfiguration()->Get(GetMeowClassName() + "." + "find_security").c_str()));
		SetCreateSecurity((Object_ptr)GetRegistry()->FindMe("Security/Group",1,Configuration::GetConfiguration()->Get(GetMeowClassName() + "." + "create_security").c_str()));
		SetDeleteSecurity((Object_ptr)GetRegistry()->FindMe("Security/Group",1,Configuration::GetConfiguration()->Get(GetMeowClassName() + "." + "delete_security").c_str()));
		SetUpdateSecurity((Object_ptr)GetRegistry()->FindMe("Security/Group",1,Configuration::GetConfiguration()->Get(GetMeowClassName() + "." + "update_security").c_str()));

	} catch ( Exceptions::NotFound &e )
	{

	}
}

void  Meow::Factory::OnEvent( std::string eventname, Core::Types::Array_ptr parameters )
{
}

void Meow::Factory::__Destroy( )
{
    Meow::Object::__Destroy();
}

Meow::Core::Types::Array_ptr Meow::Factory::GetServices( )
{
    return Services;
}

bool Meow::Factory::KeyCheck( Meow::Core::Types::Array_ptr key )
{
    return true;
}

void Meow::Factory::Passivate( )
{
    PassivateChildren(ldContainer);
}
void Meow::Factory::PassivateChildren( Core::Types::Array_ptr children )
{
    Core::Types::Stack_ptr Keys=children->GetKeys();
    while ( !Keys->IsEmpty() )
    {
        Core::Types::String_ptr thiskey=(Core::Types::String_ptr)Keys->Pop();
        Core::ptr Current=children->Get(thiskey->GetValue());
        LifecycleObject_ptr child=(LifecycleObject_ptr)Current;
	if ( !child.IsNull() )
	{
	    if ( child->IsDirty() )
	        child->Passivate();
	}
    }
}
void Meow::Factory::Purge( )
{
    PurgeChildren(ldContainer);
}
void Meow::Factory::PurgeChildren( Core::Types::Array_ptr children )
{
    Core::Types::Stack_ptr Keys=children->GetKeys();
    while ( !Keys->IsEmpty() )
    {
        Core::Types::String_ptr thiskey=(Core::Types::String_ptr)Keys->Pop();
        Core::ptr Current=children->Get(thiskey->GetValue());
        LifecycleObject_ptr child=(LifecycleObject_ptr)Current;
		if ( !child.IsNull() )
		{
		    if ( child->IsDirty() )
		        child->Passivate();
			// So we should have a reference count of <= 2 to purge it.
			if ( child->GetRefCount() <=2 )
			{
		        children->Unset(thiskey->GetValue());
			//	Core::Log::GetLog()->Information("Meow::Factory::PurgeChildren","sssd","About to lose my reference to ",child->ToString().c_str(),"=",child->GetRefCount());
				child=NULL;
		        objectsHeld--;
		    }
//			else
	//			Core::Log::GetLog()->Information("Meow::Factory::PurgeChildren","sssd","Get Ref Count of ",child->ToString().c_str(),"=",child->GetRefCount());
		}
    }
}
void Meow::Factory::Flush( )
{
    FlushChildren(ldContainer);
}
void Meow::Factory::FlushChildren( Core::Types::Array_ptr children )
{
    PurgeChildren(children);
}
void Meow::Factory::GarbageCollect( )
{
	if ( CountObjectsHeld() > 10 )
	{
		Core::Log::GetLog()->Information("Meow::Factory::GarbageCollect","ss","Garbage collection start on ",GetMeowClassName().c_str());
	    GarbageCollectionChildren(ldContainer);
		Core::Log::GetLog()->Information("Meow::Factory::GarbageCollect","ss","Garbage collection complete on ",GetMeowClassName().c_str());		
	}
}
void Meow::Factory::GarbageCollectionChildren( Core::Types::Array_ptr children )
{
    PurgeChildren(children);
}
unsigned long Meow::Factory::CountObjectsHeld( )
{
    return objectsHeld;
}

Meow::LifecycleObject_ptr Meow::Factory::GetInstance( std::string service )
{
    Meow::Exceptions::NotFound exception;
    throw exception;
}

Meow::LifecycleObject_ptr Meow::Factory::FindByPrimaryKey( int argcount, ... )
{


    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    va_list ap;
    int i;
    va_start(ap, argcount);
    for (i = 0; i<argcount; i++)
    {
        Core::Types::String_ptr data=new Core::Types::String(va_arg(ap, char *));
        key->Set(i,(Core::ptr)data);
    }
    va_end(ap);
    return FindByPrimaryKey(key);
}

Meow::LifecycleObject_ptr Meow::Factory::FindByPrimaryKey( std::string uniquekey )
{
    return FindByPrimaryKey(Registry::DeCompileKey(uniquekey));
}

Meow::LifecycleObject_ptr Meow::Factory::FindByPrimaryKey( Meow::Core::Types::Array_ptr uniquekey )
{
    Meow::LifecycleObject_ptr retval;
    CanRead();
	if ( ! IsKeyInContainer(uniquekey) )
    {
        try
        {
            LifecycleObject_ptr temp;
            temp=GetInstance("");
            if ( !temp.IsNull() )
            {
                temp->SetUniqueKey(uniquekey);
                
                temp->iFactory=this;
                // DS - attempt to fix the cyclical redundancy issue
                AddInstanceToContainer(temp);
                try
                {
                	temp->Restore();
                	temp->MakeBeforeImage();
                	temp->IsReady();
                	return temp;
                } catch ( Meow::Exceptions::NotFound &excep )
                {
                	// DS remove if it doesn't work, rather than only place
                	// in container if it does.
                	RemoveKeyFromContainer(uniquekey);
                	throw excep;
                }
            }
            else
            {            	
                Meow::Exceptions::NotFound exception;
                exception.SetMessage("Instance is NULL");
                throw exception;
            }
        } catch ( Meow::Exceptions::NotFound & exception )
        {
            throw exception;
        }
    }
    else
    {
    	// cout << "Calling find." << endl;
        retval=FindInstanceInContainer(uniquekey);
    }
    return retval;
}
Meow::LifecycleObject_ptr
	Meow::Factory::FindByPrimaryKeyOrRestore( Meow::Core::Types::Array_ptr uniquekey, Meow::Core::Types::Array_ptr attributes )
{
    Meow::LifecycleObject_ptr retval;
    CanRead();
    if ( !KeyCheck(uniquekey) )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Key specified does not match the object schema");
        throw exception;
    }
    if ( ! IsKeyInContainer(uniquekey) )
    {
        try
        {
            LifecycleObject_ptr temp;
            temp=GetInstance("");
            if ( !temp.IsNull() )
            {
                temp->SetUniqueKey(uniquekey);
                temp->iFactory=this;
                temp->RestoreFromAttributes(attributes);
                temp->IsReady();
                AddInstanceToContainer(temp);
                return temp;
            }
            else
            {
                Meow::Exceptions::NotFound exception;
                exception.SetMessage("Instance is NULL");
                throw exception;
            }
        } catch ( Meow::Exceptions::NotFound & exception )
        {
            throw exception;
        }
    }
    else
    {
        retval=FindInstanceInContainer(uniquekey);
    }
    return retval;
}


Meow::LifecycleObject_ptr Meow::Factory::CreateByPrimaryKey( std::string service, int argcount, ... )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    va_list ap;
    int i;
    va_start(ap, argcount);
    for (i = 0; i<argcount; i++)
    {
        Core::Types::String_ptr data=new Core::Types::String(va_arg(ap, char *));
        key->Set(i,(Core::ptr)data);
    }
    va_end(ap);
    return CreateByPrimaryKey(key,service);
}

Meow::LifecycleObject_ptr Meow::Factory::CreateByPrimaryKey( std::string uniquekey, std::string service ) 
{
    return CreateByPrimaryKey(Registry::InterpretKey(uniquekey),service);
}


Meow::LifecycleObject_ptr Meow::Factory::CreateByPrimaryKey(
    Meow::Core::Types::Array_ptr uniquekey,
    std::string service )
{
	CanCreate();
    Meow::LifecycleObject_ptr instance;
    if ( !KeyCheck(uniquekey) )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Key specified does not match the object schema");
        throw exception;
    }
    try
    {
        Meow::Exceptions::Duplicate exception;
        instance=FindByPrimaryKey(uniquekey);
        throw exception;
    } catch ( Meow::Exceptions::NotFound & exception )
    {

    }
    instance=GetInstance(service);
    instance->SetUniqueKey(uniquekey);
    instance->iFactory=this;
    Core::Log::GetLog()->Warning("Meow::Factory::CreateByPrimaryKey","s","Calling Create.");
    instance->Create();
    instance->MakeBeforeImage();

    // Now call any events we need to 
    for ( int i=0; i<iOnCreateEvents->Count(); i++ )
    {
    	    Core::Types::ValueType_ptr iEventName=(Core::Types::ValueType_ptr)iOnCreateEvents->Get(i);
    	    Core::Types::Array_ptr evParms=new Core::Types::Array();
    	    evParms->Set("Current",(Core::ptr)instance->GetAttributeImage());
    	    Registry::GetRegistry()->PostEvent(iEventName->GetValueAsString(),evParms);
    }
    AddInstanceToContainer(instance);
    Core::Log::GetLog()->Warning("Meow::Factory::CreateByPrimaryKey","s","Done.");
    return instance;
}


void Meow::Factory::DestroyByPrimaryKey( int argcount, ... )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    va_list ap;
    int i;
    va_start(ap, argcount);
    for (i = 0; i<argcount; i++)
    {
        Meow::Core::Types::String_ptr data=new Meow::Core::Types::String(va_arg(ap, char *));
        key->Set(i,(Core::ptr)data);
    }
    va_end(ap);
    DestroyByPrimaryKey(key);
}


void Meow::Factory::DestroyByPrimaryKey( std::string uniquekey ) 
{
    return DestroyByPrimaryKey(Registry::InterpretKey(uniquekey));
}


void Meow::Factory::DestroyByPrimaryKey( Meow::Core::Types::Array_ptr uniquekey )
{
	CanDelete();

    if ( !KeyCheck(uniquekey) )
    {
        Meow::Exceptions::NotFound exception;
        exception.SetMessage("Key specified does not match the object schema");
        throw exception;
    }
    LifecycleObject_ptr instance=FindByPrimaryKey(uniquekey);
    std::string alias=CompoundKeyHashing(uniquekey);
    instance->iFactory=this;
    // Now call any events we need to
    for ( int i=0; i<iOnDeleteEvents->Count(); i++ )
    {
    	    Core::Types::ValueType_ptr iEventName=(Core::Types::ValueType_ptr)iOnDeleteEvents->Get(i);
    	    Core::Types::Array_ptr evParms=new Core::Types::Array();
    	    evParms->Set("Previous",(Core::ptr)instance->GetAttributeImage());
    	    Registry::GetRegistry()->PostEvent(iEventName->GetValueAsString(),evParms);
    }
    instance->_Delete();
    instance->__Destroy();
    ldContainer->Unset(alias);
}
void Meow::Factory::PostUpdateEvents( Core::Types::Array_ptr before, Core::Types::Array_ptr current )
{
    for ( int i=0; i<iOnUpdateEvents->Count(); i++ )
    {
    	    Core::Types::ValueType_ptr iEventName=(Core::Types::ValueType_ptr)iOnUpdateEvents->Get(i);
    	    Core::Types::Array_ptr evParms=new Core::Types::Array();
    	    evParms->Set("Current",(Core::ptr)current);
    	    evParms->Set("Previous",(Core::ptr)before);
    	    Registry::GetRegistry()->PostEvent(iEventName->GetValueAsString(),evParms);
    }	
}

std::string Meow::Factory::CompoundKeyHashing( Meow::Core::Types::Array_ptr key )
{    
    return Registry::CompileKey(key);
}
void Meow::Factory::AddInstanceToContainer( LifecycleObject_ptr object )
{
    if ( object->GetUniqueKey()->Count()==0 )
    {
        objectsHeld++;
        ldContainer->Set("",(Core::ptr)object);
    }
    else
    {
        ldContainer->Set(CompoundKeyHashing(object->GetUniqueKey()),(Core::ptr)object	);
        objectsHeld++;
    }
}

void Meow::Factory::RemoveKeyFromContainer( Meow::Core::Types::Array_ptr key )
{
    if ( key->Count()==0 )
        return ldContainer->Unset("");
    else
    {
        objectsHeld--;
        ldContainer->Unset(CompoundKeyHashing(key));
    }
}

bool Meow::Factory::IsKeyInContainer( Meow::Core::Types::Array_ptr key )
{

    if ( key->Count()==0 )
        return ldContainer->IsSet("");
    return ldContainer->IsSet(CompoundKeyHashing(key));
}

Meow::LifecycleObject_ptr Meow::Factory::FindInstanceInContainer( Meow::Core::Types::Array_ptr key )
{
    LifecycleObject_ptr instance;
    if ( key->Count()==0 )
        instance=(LifecycleObject_ptr)ldContainer->Get("");
    else
        instance=(LifecycleObject_ptr)ldContainer->Get(CompoundKeyHashing(key));
    return instance;
}

void Meow::Factory::MarkChangedExternally( Meow::Core::Types::Array_ptr key )
{
	CanWrite();

    LifecycleObject_ptr instance;

    instance=FindInstanceInContainer(key);
    instance->ReadRequired( );
}

Meow::LifecycleObject_ptr Meow::Factory::FindByPrimaryKey(char * key [] )
{
    int i=0;
    Meow::Core::Types::Array_ptr realkey=new Meow::Core::Types::Array();
    char * keyptr=key[i];
    while ( keyptr!=NULL )
    {
        string key_val=keyptr;
        Meow::Core::Types::String_ptr stringval=new Meow::Core::Types::String(key_val);
        realkey->Set(i,(Core::ptr)stringval);
        i++;
        keyptr=key[i];
    }
    return FindByPrimaryKey(realkey);
}

Meow::LifecycleObject_ptr Meow::Factory::CreateByPrimaryKey(char * key[], std::string service )
{
    int i=0;
    Meow::Core::Types::Array_ptr realkey=new Meow::Core::Types::Array();
    char * keyptr=key[i];
    while ( keyptr!=NULL )
    {
        string key_val=keyptr;
        Meow::Core::Types::String_ptr stringval=new Meow::Core::Types::String(key_val);
        realkey->Set(i,(Meow::Core::ptr)stringval);
        i++;
        keyptr=key[i];
    }
    return CreateByPrimaryKey(realkey,service);
}


void Meow::Factory::DestroyByPrimaryKey(char * key [] )
{
    int i=0;
    Meow::Core::Types::Array_ptr realkey=new Meow::Core::Types::Array();
    char * keyptr=key[i];
    while ( keyptr!=NULL )
    {
        string key_val=keyptr;
        Meow::Core::Types::String_ptr stringval=new Meow::Core::Types::String(key_val);
        realkey->Set(i,(Core::ptr)stringval);

        i++;
        keyptr=key[i];
    }
    DestroyByPrimaryKey(realkey);
}

Meow::Registry_ptr Meow::Factory::GetRegistry( )
{
    // return Meow::Registry::GetRegistry();
    return iRegistry;
}

Meow::Core::Types::Array_ptr Meow::Factory::DeCompileKey( std::string compiled_key )
{
	return GetRegistry()->DeCompileKey(compiled_key);

}

std::string Meow::Factory::GetClassNameFromKey( std::string compiled_key )
{
    std::string class_and_key=compiled_key;
    std::string class_name=class_and_key.substr(0,class_name.find_first_of(":"));
    return class_name;
}
Meow::Core::Types::Array_ptr Meow::Factory::GetAnonymousKey( )
{
    Meow::Core::Types::Array_ptr retval=new Meow::Core::Types::Array();
    char buffer[256];
    sprintf(buffer,"%d",(int)ldContainer->Count());
    Core::Types::String_ptr value=new Core::Types::String(buffer);
    retval->Set(0,(Core::ptr)value);
    return retval;
}
void  Meow::Factory::SetFactoryOnObject( Meow::LifecycleObject_ptr object )
{
    object->iFactory=this;
}

void Meow::Factory::RegisterEvents( )
{
}
Meow::Object_ptr Meow::Factory::GetMinimumSecurity()
{
    return iMinimumSecurity;
}
void Meow::Factory::SetMinimumSecurity( Meow::Object_ptr sec )
{
    iMinimumSecurity=sec;
}
Meow::Object_ptr Meow::Factory::GetFindSecurity()
{
    return iFindSecurity;
}
void Meow::Factory::SetFindSecurity( Meow::Object_ptr sec )
{
    iFindSecurity=sec;
}
Meow::Object_ptr Meow::Factory::GetCreateSecurity()
{
    return iCreateSecurity;
}
void Meow::Factory::SetCreateSecurity( Meow::Object_ptr sec )
{
    iCreateSecurity=sec;
}
Meow::Object_ptr Meow::Factory::GetDeleteSecurity()
{
    return iDeleteSecurity;
}
void Meow::Factory::SetDeleteSecurity( Meow::Object_ptr sec )
{
    iDeleteSecurity=sec;
}
Meow::Object_ptr Meow::Factory::GetUpdateSecurity()
{
    return iUpdateSecurity;
}
void Meow::Factory::SetUpdateSecurity( Meow::Object_ptr sec )
{
    iUpdateSecurity=sec;
}



void
Meow::UncollectedFactory::MarkChangedExternally( Core::Types::Array_ptr key )
{
}

bool Meow::Factory::CanJoin( std::string otherclass )
{
     return Joins->IsSet(otherclass);
}

Meow::JoinSpec_ptr Meow::Factory::GetJoinSpec( std::string relationshipname )
{
    return (Meow::JoinSpec_ptr)Joins->Get(relationshipname);
}
void Meow::Factory::AddJoin( std::string relationshipname, JoinSpec_ptr joins )
{
    Joins->Set(relationshipname,(Core::ptr)joins);
}
Meow::Core::Types::Stack_ptr Meow::Factory::GetJoinList( )
{
    return Joins->GetKeys();
}


Meow::LifecycleObject_ptr Meow::Factory::GetFromXML( std::string service, std::string xmldocument ) 
{
    /* TODO */
	Meow::LifecycleObject_ptr object;
	return object;
}

bool Meow::Factory::SecondPhasePurge()
{
    return true;
}

void Meow::Factory::FromXML( std::string xmlDocument )
{
    /* TODO */
    // Parse the XML using collection parser
    CollectionParser_ptr parser=new CollectionParser();
    if ( !parser->ParseString(xmlDocument) )
    {
        GetLog()->Error("Meow::Factory::FromXML","s",parser->GetLastError( ).c_str());
    }
    else
    {
        // Find out the tablespec for the class
        // For each member of the collection, work out the key fields and do a Find/Create on it and set the attributes using SetVariable
    }
}
void Meow::Factory::SetVariable( std::string varname, Core::ptr data )
{
    if ( iHashMap.IsNull() ) iHashMap=new Core::Types::Array();
    	iHashMap->Set(varname,data);
}
Meow::Core::ptr Meow::Factory::GetVariable( std::string varname )
{
    if ( iHashMap.IsNull() ) iHashMap=new Core::Types::Array();
    return iHashMap->Get(varname);
}
bool Meow::Factory::ExistsVariable( std::string varname )
{
    if ( iHashMap.IsNull() ) iHashMap=new Core::Types::Array();
    return iHashMap->IsSet(varname);
}
Meow::Core::Types::Stack_ptr Meow::Factory::GetVariableKeys() 
{
    if ( iHashMap.IsNull() ) iHashMap=new Core::Types::Array();
    return iHashMap->GetKeys();
}


void Meow::Factory::CanWrite() 
{
	if ( GetRegistry()->GetCurrentSession().IsNull() )
		return;
	try
	{
		Security::User_ptr user=(Security::User_ptr)((Session_ptr)GetRegistry()->GetCurrentSession())->GetCurrentUser();
		if ( !user.IsNull() )
		{
			if ( !iMinimumSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iMinimumSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage(std::string("User ") + user->GetUsername() + std::string(" is not authorised to use factory ") + GetMeowClassName() + std::string(" because it is not a member of ") + ((Security::Group_ptr)iMinimumSecurity)->GetName());
					throw security;
				}
			}
			if ( !iUpdateSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iUpdateSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage(std::string("User ") + user->GetUsername() + std::string(" is not authorised to use factory ") + GetMeowClassName() + std::string(" because it is not a member of ") + ((Security::Group_ptr)iUpdateSecurity)->GetName());
					throw security;
				}
			}
		}
	} catch ( Exceptions::NotFound &nf )
	{
		Core::Log::GetLog()->Warning("Meow::Factory::CanWrite","sss","Not ready to check security on ",GetMeowClassName().c_str()," yet so so absorbing not found exception.");
	}

}
void Meow::Factory::CanRead()
{
	if ( GetRegistry()->GetCurrentSession().IsNull() )
		return;
	try
	{
		Security::User_ptr user=(Security::User_ptr)((Session_ptr)GetRegistry()->GetCurrentSession())->GetCurrentUser();
		if ( !user.IsNull() )
		{
			if ( ! iMinimumSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iMinimumSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage(std::string("User ") + user->GetUsername() + std::string(" is not authorised to use factory ") + GetMeowClassName() + " because it is not a member of " + ((Security::Group_ptr)iMinimumSecurity)->GetName());
					throw security;
				}
			}
			if ( ! iFindSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iFindSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage("User " + user->GetUsername() + " is not authorised to use factory " + GetMeowClassName() + " because it is not a member of " + ((Security::Group_ptr)iFindSecurity)->GetName());
					throw security;
				}
			}
		}
	} catch ( Exceptions::NotFound &nf )
	{
		Core::Log::GetLog()->Warning("Meow::Factory::CanRead","sss","Not ready to check security on ",GetMeowClassName().c_str()," yet so so absorbing not found exception.");
	}
}
void Meow::Factory::CanCreate()
{
	
	if ( GetRegistry()->GetCurrentSession().IsNull() )
		return;
	try
	{
		Security::User_ptr user=(Security::User_ptr)((Session_ptr)GetRegistry()->GetCurrentSession())->GetCurrentUser();
		if ( !user.IsNull() )
		{
			if ( ! iMinimumSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iMinimumSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage("User " + user->GetUsername() + " is not authorised to use factory " + GetMeowClassName() + " because it is not a member of " + ((Security::Group_ptr)iMinimumSecurity)->GetName());
					throw security;
				}
			}
			if ( ! iCreateSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iCreateSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage("User " + user->GetUsername() + " is not authorised to use factory " + GetMeowClassName() + " because it is not a member of " + ((Security::Group_ptr)iCreateSecurity)->GetName());
					throw security;
				}
			}
		}
	} catch ( Exceptions::NotFound &nf )
	{
		Core::Log::GetLog()->Warning("Meow::Factory::CanCreate","sss","Not ready to check security on ",GetMeowClassName().c_str()," yet so so absorbing not found exception.");
	}
}
void Meow::Factory::CanDelete()
{
	
	if ( GetRegistry()->GetCurrentSession().IsNull() )
		return;
	try
	{
		Security::User_ptr user=(Security::User_ptr)((Session_ptr)GetRegistry()->GetCurrentSession())->GetCurrentUser();
		if ( !user.IsNull() )
		{
			if ( ! iMinimumSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iMinimumSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage("User " + user->GetUsername() + " is not authorised to use factory " + GetMeowClassName() + " because it is not a member of " + ((Security::Group_ptr)iMinimumSecurity)->GetName());
					throw security;
				}
			}
			if ( !iDeleteSecurity.IsNull() )
			{
				if ( !((Security::Group_ptr)iDeleteSecurity)->IsAuthorised(user))
				{
					Meow::Exceptions::Security security;
					security.SetMessage("User " + user->GetUsername() + " is not authorised to use factory " + GetMeowClassName() + " because it is not a member of " + ((Security::Group_ptr)iDeleteSecurity)->GetName());
					throw security;
				}
			}
		}
	} catch ( Exceptions::NotFound &nf )
	{
		Core::Log::GetLog()->Warning("Meow::Factory::CanDelete","sss","Not ready to check security on ",GetMeowClassName().c_str()," yet so so absorbing not found exception.");
	}
}
Meow::CollectionParser::CollectionParser()
{
    collection=new Core::Types::Array();
    depth=0;
}
Meow::CollectionParser::~CollectionParser()
{
}
Meow::Core::Types::Array_ptr Meow::CollectionParser::GetCollection()
{
    return collection;
}
void Meow::CollectionParser::StartElement( string element, Meow::Core::Types::Array_ptr attributes )
{
    // Element depth is 0 -> envelope
    if ( depth==1 )
    {
        collection->Set(collection->Count(),new Core::Types::Array());
        classname=element;
    }
    // Element depth is 1 -> new member of the collection
    else if ( depth == 2 )
    {
        attributedata=element;
        characterdata="";
    }
    // Element depth is 2 -> attribute of the member (record character data)
    depth++;
}
void Meow::CollectionParser::EndElement( string element )
{
    // Element depth is 3 -> set attribute
    if ( depth == 3 )
    {
        Core::Types::Array_ptr attributes=(Core::Types::Array_ptr)collection->Get(collection->Count());
        attributes->Set(attributedata,new Core::Types::String(characterdata));
    }
    depth--;
}

void Meow::CollectionParser::CharacterData( string data )
{
    characterdata.append(data);
}

std::string Meow::CollectionParser::GetMeowClassName()
{
    return classname;
}

Meow::InstanceParser::InstanceParser()
{
    properties=new Core::Types::Array();
    depth=0;
}
Meow::InstanceParser::~InstanceParser()
{
}
Meow::Core::Types::Array_ptr Meow::InstanceParser::GetProperties()
{
    return properties;
}
void Meow::InstanceParser::StartElement( string element, Meow::Core::Types::Array_ptr attributes )
{
    if ( depth==1 )
    {
        attributedata=element;
        characterdata="";
    }
    depth++;
}
void Meow::InstanceParser::EndElement( string element )
{
    if ( depth==2 )
    {
        properties->Set(attributedata,new Core::Types::String(characterdata));
    }
    depth--;
}
void Meow::InstanceParser::CharacterData( string data )
{
    characterdata.append(data);
}

Meow::SchemaField::SchemaField() : iVersion(0), iOffset(0)
{
	SetMeowClassName("SchemaField");
	iFieldType=Factory::Serial;
	iColumnType=Factory::Integer;
	iIndex=true;
	iSerial=true;
	iDefaultValue=new Core::Types::Integer();
	iChanged=false;
	iShowInList=false;
	iPrivate=false;
}
Meow::SchemaField::~SchemaField()
{
}

bool Meow::Factory::AttributeValidation( std::string fieldname, Core::ptr value )
{

	if ( ! iSchemeAttributes->IsSet(fieldname) && ! iSchemeKeys->IsSet(fieldname) )
	{
		Core::Log::GetLog()->Error("Meow::Factory::AttributeValidation","ssss","Factory ",GetMeowClassName().c_str()," validation failed for non existent field ",fieldname.c_str());
		return false;
	}
	if ( iSchemeKeys->IsSet(fieldname) )
	{
		SchemaField_ptr field=(SchemaField_ptr)iSchemeKeys->Get(fieldname);
		if ( field->iColumnType==Factory::Object )
		{
			if ( value.IsNull() )
				return true;
			else
			{
				std::string valueAsString="";
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
				if ( valueAsString.length()> 0 )
				{
					// Ultimate test is, can we find it?
					try
					{
						LifecycleObject_ptr instance=GetRegistry()->FindMeByObjectReference(valueAsString);
						return true;
					} catch ( Exceptions::NotFound &nf )
					{
						Core::Log::GetLog()->Error("Meow::Factory::AttributeValidation","ssss","Failed to validate field ",fieldname.c_str()," could not find object ",valueAsString.c_str());
						return false;
					}
				}
				else
					return true; // null
			}
		}
		else
		{
			std::string valueAsString="";
			if ( !value.IsNull() )
			{
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
			}
			
			return field->iConstraint->Test(new Core::Types::String(valueAsString));
		}
	}
	else
	{
		SchemaField_ptr field=(SchemaField_ptr)iSchemeAttributes->Get(fieldname);
		if ( field->iColumnType==Factory::Object )
		{
			if ( value.IsNull() )
				return true;
			else
			{
				std::string valueAsString="";
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
				// Ultimate test is, can we find it?
				if ( valueAsString.length() > 0 )
				{
					try
					{
						LifecycleObject_ptr instance=GetRegistry()->FindMeByObjectReference(valueAsString);
						return true;
					} catch ( Exceptions::NotFound &nf )
					{
						Core::Log::GetLog()->Error("Meow::Factory::AttributeValidation","ssss","Failed to validate field ",fieldname.c_str()," could not find object ",valueAsString.c_str());
						return false;
					}
				}
				else
					return true;
			}
		}
		else
		{
			std::string valueAsString="";
			if ( !value.IsNull() )
			{
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
			}
			Core::Log::GetLog()->Information("Meow::Factory::AttributeValidation","ssss","Testing field ",fieldname.c_str()," against value ",valueAsString.c_str());
			return field->iConstraint->Test(new Core::Types::String(valueAsString));
		}
	}
}
std::string Meow::Factory::AttributeFullValidation( std::string fieldname, Core::ptr value )
{

	std::string retval;
	if ( ! iSchemeAttributes->IsSet(fieldname) && ! iSchemeKeys->IsSet(fieldname) )
	{
		Core::Log::GetLog()->Error("Meow::Factory::AttributeValidation","ssss","Factory ",GetMeowClassName().c_str()," validation failed for non existent field ",fieldname.c_str());
		retval.append("Failure");
		retval.append("|");
		retval.append("Field not found in scheme");
		return retval;
	}
	if ( iSchemeKeys->IsSet(fieldname) )
	{
		SchemaField_ptr field=(SchemaField_ptr)iSchemeKeys->Get(fieldname);
		if ( field->iColumnType==Factory::Object )
		{
			if ( value.IsNull() )
			{
				retval.append("Success");
				return retval;
			}
			else
			{
				std::string valueAsString="";
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
				if ( valueAsString.length()> 0 )
				{
					// Ultimate test is, can we find it?
					try
					{
						LifecycleObject_ptr instance=GetRegistry()->FindMeByObjectReference(valueAsString);
						retval.append("Success");
						return retval;
					} catch ( Exceptions::NotFound &nf )
					{
						Core::Log::GetLog()->Error("Meow::Factory::AttributeValidation","ssss","Failed to validate field ",fieldname.c_str()," could not find object ",valueAsString.c_str());
						retval.append("Failure");
						retval.append("|");
						retval.append("Failed To validate.Could not find object");
						return retval;
					}
				}
				else
				{
					retval.append("Success");
					return retval;
				}
			}
		}
		else
		{
			std::string valueAsString="";
			if ( !value.IsNull() )
			{
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
			}

			if(field->iConstraint->Test(new Core::Types::String(valueAsString)))
			{
				retval.append("Success");
				return retval;
			}
			else
			{
				retval.append("Failure");
				retval.append("|");
				retval.append("Field Constraint test failed");
				return retval;
			}
		}
	}
	else
	{
		SchemaField_ptr field=(SchemaField_ptr)iSchemeAttributes->Get(fieldname);
		if ( field->iColumnType==Factory::Object )
		{
			if ( value.IsNull() )
			{
				retval.append("Success");
				return retval;
			}
			else
			{
				std::string valueAsString="";
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
				// Ultimate test is, can we find it?
				if ( valueAsString.length() > 0 )
				{
					try
					{
						LifecycleObject_ptr instance=GetRegistry()->FindMeByObjectReference(valueAsString);
						retval.append("Success");
						return retval;
					} catch ( Exceptions::NotFound &nf )
					{
						Core::Log::GetLog()->Error("Meow::Factory::AttributeValidation","ssss","Failed to validate field ",fieldname.c_str()," could not find object ",valueAsString.c_str());
						retval.append("Failure");
						retval.append("|");
						retval.append("Failed To validate.Could not find object");
						return retval;
					}
				}
				else
				{
					retval.append("Success");
					return retval;
				}
			}
		}
		else
		{
			std::string valueAsString="";
			if ( !value.IsNull() )
			{
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVT=(Core::Types::ValueType_ptr)value;
					valueAsString=valueAsVT->GetValueAsString();
				}
				else
				{
					Object_ptr valueAsOb=(Object_ptr)value;
					valueAsString=valueAsOb->ToString();
				}
			}
			Core::Log::GetLog()->Information("Meow::Factory::AttributeValidation","ssss","Testing field ",fieldname.c_str()," against value ",valueAsString.c_str());
			if(field->iConstraint->Test(new Core::Types::String(valueAsString)))
			{
				retval.append("Success");
				return retval;
			}
			else
			{
				retval.append("Failure");
				retval.append("|");
				retval.append("Field Constraint test failed");
				return retval;
			}
		}
	}
}
Meow::Core::ptr Meow::Factory::AttributeConversion( std::string fieldname, Core::Types::ValueType_ptr value )
{
	Core::ptr retval;
	if ( ! iSchemeAttributes->IsSet(fieldname) )
	{
		Exceptions::BadParameter exception;
		exception.SetMessage("Conversion failed for non existent field " + fieldname);
		Core::Log::GetLog()->Error("Meow::Factory::AttributeConversion","ssss","Factory ",GetMeowClassName().c_str()," Conversion failed for non existent field ",fieldname.c_str());
		throw exception;
	}
	Core::Log::GetLog()->Information("Meow::Factory::AttributeConversion","ssss","Converting field:",fieldname.c_str()," value:",value->GetValueAsString().c_str());
	SchemaField_ptr field=(SchemaField_ptr)iSchemeAttributes->Get(fieldname);

	switch ( field->iColumnType )
	{
	case Factory::Object:
		if ( value->Count()>0 )
		{
			try
			{
				retval=GetRegistry()->FindMeByObjectReference(value->GetValueAsString());
			}
			catch ( Exceptions::NotFound &nf )
			{
				Core::Log::GetLog()->Error("Meow::Factory::AttributeConversion","ssss","Could not set ",fieldname.c_str()," as could not find ",value->GetValueAsString().c_str());
			}
		}
		else
			retval=Core::Object::Null();
		break;
	case Factory::Text:
	case Factory::HTML:
	case Factory::XML:
	        retval=new Core::Types::String(value->GetValueAsString());
                break;
	case Factory::Password:
	case Factory::String:
		{
			retval=new Core::Types::String(value->GetValueAsString());
		}
		break;
	case Factory::Integer:
		{
		retval=new Core::Types::Integer(value->GetValueAsInteger());
		}
		break;
	case Factory::Float:
		{
		retval=new Core::Types::Float(value->GetValueAsFloat());
		}
		break;
	case Factory::Boolean:
		{
		retval=new Core::Types::Boolean(value->GetValueAsBoolean());	
		}	
		break;
	case Factory::Date:
		{
			// Dates are currently interpreted as 20/06/2015 16:00 from display
			if(value->GetValueAsString().length() == 16)
				retval=new Core::Time::Date(value->GetValueAsString(),"dd/mm/yyyy HH:MM");
			else
				retval=new Core::Time::Date(value->GetValueAsString());
		}
		break;
	case Factory::Array:
	case Factory::Queue:
	case Factory::Stack:
	case Factory::Void:
	case Factory::ValueType:
	case Factory::Image:
	case Factory::File:
		retval=value;
		break;
	default:
       retval=value;
	}
	return retval;
}


void Meow::Factory::AddSerialKeyField( std::string fieldname, std::string publicName, std::string title, bool publicSearchable, std::string description ) 
{

	if ( iSerial )
	{
		Exceptions::BadParameter e;
		e.SetMessage("Cannot add a serial field [" + fieldname + "] because there is already a serial field defined." );
		throw e;
	}
	if ( iKeySet )
	{
		Exceptions::BadParameter e;
		e.SetMessage("Cannot add a serial field [" + fieldname + "] because there is already a compound key set." );
		throw e;
	}
	iSerial=true;
	SchemaField_ptr schema=new SchemaField();
	schema->iFieldName=fieldname;
	schema->iIndex=false;
	schema->iPublicName=publicName;
	schema->iTitle=title;
	schema->iOffset=iSchemeKeys->Count();
	schema->iDescription=description;
	iSchemeKeys->Set(fieldname,(Core::ptr)schema);
	SchemaOperation_ptr schemaContainer=new SchemaOperation();
	schemaContainer->iVersion=0;
	schemaContainer->iDefinition=schema;
	iAttributeQueue->Push((Core::ptr)schemaContainer);
	iSchemeKeysByAlias->Set(publicName,(Core::ptr)schema);
	if ( publicSearchable )
		iSearchableParts->Set(fieldname,(Core::ptr)schema);
}
void Meow::Factory::AddCompoundKeyField( std::string fieldname, Factory::SchemaColumnType type, bool indexible, std::string publicName, std::string title, bool publicSearchable, std::string additionalConstraints , std::string description)
{
	if ( iSerial )
	{
		Exceptions::BadParameter e;
		e.SetMessage("Cannot add a compound key field [" + fieldname + "] because there is already a serial field defined." );
		throw e;
	}
	iSerial=false;
	iKeySet=true;
	SchemaField_ptr schema=new SchemaField();
	schema->iFieldName=fieldname;
	schema->iIndex=true;
	schema->iSerial=true;
	schema->iColumnType=type;
	schema->iIndex=indexible;
	schema->iFieldType=Key;
	schema->iPublicName=publicName;
	schema->iTitle=title;
	schema->iOffset=iSchemeKeys->Count();
	schema->iEnumerationValues=additionalConstraints; // Re-use this field if its Factory::Object
	schema->iDescription=description;
	iSchemeKeys->Set(fieldname,(Core::ptr)schema);
	SchemaOperation_ptr schemaContainer=new SchemaOperation();
	schemaContainer->iVersion=0;
	schemaContainer->iDefinition=schema;
	iAttributeQueue->Push((Core::ptr)schemaContainer);
	iSchemeKeysByAlias->Set(publicName,(Core::ptr)schema);
	if ( publicSearchable )
		iSearchableParts->Set(fieldname,(Core::ptr)schema);
	switch(schema->iColumnType)
	{
	case Factory::Text:
	case Factory::HTML:
	case Factory::XML:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_TEXT);
		break;
	case Factory::Password:
	case Factory::String:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
		break;
	case Factory::Integer:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_INTEGER);
		break;
	case Factory::Float:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_FLOAT);
		break;
	case Factory::Boolean:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_BOOLEAN);
		break;
	case Factory::Date:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_NULLABLEDATETIME);
		break;
	case Factory::Array:
		// TODO: Regular expression for array format.
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
		break;
	case Factory::Queue:
		// TODO: Regular expression for array format.
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
		break;
	case Factory::Stack:
		// TODO: Regular expression for array format.
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
		
		break;
	case Factory::Void:
		// TODO: Regular expression for array format.
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);		
		break;
	case Factory::ValueType:
		// TODO: Regular expression for array format.
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);			
		break; 
	case Factory::Enumeration:
		schema->iConstraint=new Core::Types::Constraint("^(" + schema->iEnumerationValues + ")$");
		break;
	case Factory::Image:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
		break;
	case Factory::File:
		schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
		break;
	case Factory::Object:
		if ( additionalConstraints.length() > 0 )
			{
				Meow::Registry::GetRegistry()->AddClassSubscription(additionalConstraints,this,schema->iFieldName);
			}
		break;
	}
}
void Meow::Factory::ShowInLists( std::string fieldname )
{
	SchemaField_ptr schema=(SchemaField_ptr)iSchemeAttributes->Get(fieldname);
	if ( schema.IsNull() )
		schema=(SchemaField_ptr)iSchemeKeys->Get(fieldname);
	if ( !schema.IsNull() )
		schema->iShowInList=true;
}
void Meow::Factory::AddAttributeField( std::string fieldname,
		Factory::SchemaColumnType type,
		Core::ptr defaultValue,
		int version,
		std::string publicName,
		std::string securityGroupRead,
		std::string securityGroupWrite,
		std::string title,
		bool publicSearchable,
		Core::Types::Constraint_ptr constraint,
		std::string enumerationValues,
		Factory::SchemeSecurityType securityType,
		std::string description,
		std::string section) //,
//		std::string securityConfiguration )
{
	if ( version>iVersion )
		iVersion=version;
	SchemaField_ptr schema=new SchemaField();
	schema->iFieldName=fieldname;
	schema->iIndex=false;
	schema->iSerial=false;
	schema->iColumnType=type;
	schema->iIndex=false;
	schema->iFieldType=Attribute;
	schema->iDefaultValue=defaultValue;
	schema->iVersion=version;
	schema->iPublicName=publicName;
	schema->iSecurityGroupRead=securityGroupRead;
	schema->iSecurityGroupWrite=securityGroupWrite;
	schema->iTitle=title;
	schema->iEnumerationValues=enumerationValues; // Also gets used by Factory::Object
	schema->iSecurityType=securityType;
	schema->iDescription=description;
	schema->iSection=section;
//	schema->iSecurityConfiguration=securityConfiguration;
	if ( !constraint.IsNull() )
		schema->iConstraint=constraint;
	else
	{
		switch(schema->iColumnType)
		{	
		case Factory::Text:
		case Factory::XML:
		case Factory::HTML:
			        schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_TEXT);
                break;
		case Factory::Password:
		case Factory::String:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			break;
		case Factory::Integer:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_INTEGER);
			break;
		case Factory::Float:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_FLOAT);
			break;
		case Factory::Boolean:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_BOOLEAN);
			break;
		case Factory::Date:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_NULLABLEDATETIME);
			break;
		case Factory::Array:
			// TODO: Regular expression for array format.
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			break;
		case Factory::Queue:
			// TODO: Regular expression for array format.
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			break;
		case Factory::Stack:
			// TODO: Regular expression for array format.
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			
			break;
		case Factory::Void:
			// TODO: Regular expression for array format.
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			break;
		case Factory::ValueType:
			// TODO: Regular expression for array format.
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);			
			break; 
		case Factory::Enumeration:
			schema->iConstraint=new Core::Types::Constraint("^(" + schema->iEnumerationValues + ")$");
			break;
		case Factory::Image:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			break;
		case Factory::File:
			schema->iConstraint=new Core::Types::Constraint(CONSTRAINT_STRING);
			break;
		case Factory::Object:
			if ( schema->iEnumerationValues.length() > 0 )
			{
				Meow::Registry::GetRegistry()->AddClassSubscription(schema->iEnumerationValues,this,schema->iPublicName);
			}
			break;
		}
	}
	iSchemeAttributes->Set(fieldname,(Core::ptr)schema);
	SchemaOperation_ptr schemaContainer=new SchemaOperation();
	schemaContainer->iVersion=version;
	schemaContainer->iDefinition=schema;
	iAttributeQueue->Push((Core::ptr)schemaContainer);
	if ( iVersion < version )
		version=iVersion;
	iSchemeAttributesByAlias->Set(publicName,(Core::ptr)schema);
	iSchemeKeysByAlias->Set(publicName,(Core::ptr)schema);
	if ( publicSearchable )
		iSearchableParts->Set(fieldname,(Core::ptr)schema);
}

void  Meow::Factory::DeleteAttributeField( std::string fieldname, int version )
{
	if ( version>iVersion )
		iVersion=version;

	

	SchemaOperation_ptr schemaContainer=new SchemaOperation();
	schemaContainer->iVersion=version;
	schemaContainer->iOperation=1;
	schemaContainer->iDefinition=(SchemaField_ptr)iSchemeAttributes->Get(fieldname);
	if ( schemaContainer->iDefinition->iColumnType == Factory::Object && 
			schemaContainer->iDefinition->iEnumerationValues.length() > 0 )
		Meow::Registry::GetRegistry()->RemoveClassSubscription(schemaContainer->iDefinition->iEnumerationValues,
				this,
					schemaContainer->iDefinition->iPublicName);
	iAttributeQueue->Push((Core::ptr)schemaContainer);
	iSchemeAttributes->Unset(fieldname);
	if ( iVersion < version )
			version=iVersion;
	iSchemeAttributesByAlias->Unset(schemaContainer->iDefinition->iPublicName);
	iSchemeKeysByAlias->Unset(fieldname);
	iSearchableParts->Unset(fieldname);
}

void  Meow::Factory::MakePrivateField( std::string fieldname )
{
	SchemaField_ptr field=(SchemaField_ptr)iSchemeAttributes->Get(fieldname);
	if ( !field.IsNull() )
		field->iPrivate=true;
	
}

bool Meow::Factory::Upgrade( int currentVersion )
{
	return true;	
}

// GetChanges
// ------------------------------
// Work out the change list to make to the storage area.
Meow::Core::Types::Queue_ptr Meow::Factory::GetChanges( int currentVersion )
{
	Core::Types::Queue_ptr retval=new Core::Types::Queue();
	Core::Types::Queue_ptr unwoundQueue=new Core::Types::Queue();

	while ( !iAttributeQueue->IsEmpty() )
	{
		SchemaOperation_ptr attribute=(SchemaOperation_ptr)iAttributeQueue->Pull();
		if ( attribute->iVersion > currentVersion )
		{
			retval->Push((Core::ptr)attribute);
		}
		unwoundQueue->Push((Core::ptr)attribute);
	}
	// Put it all back again. 
	while ( !unwoundQueue->IsEmpty() )
		iAttributeQueue->Push(unwoundQueue->Pull());		
	return retval;
}

// GetAttributes
// ------------------------------
// For the current security user, work out the attribute list.
Meow::Core::Types::Array_ptr Meow::Factory::GetAttributes( )
{
	Meow::Core::Types::Array_ptr retval=new Core::Types::Array();
	Core::Types::Stack_ptr keys=iSchemeAttributes->GetKeys();
	Session_ptr currentSession=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
	if ( !currentSession.IsNull() )
	{
		Security::User_ptr currentUser=(Security::User_ptr)currentSession->GetCurrentUser();
		if ( !currentUser.IsNull() )
		{
			while ( !keys->IsEmpty() )
			{
				Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
				SchemaField_ptr field=(SchemaField_ptr)iSchemeAttributes->Get(key->GetValueAsString());
				bool canSeeAttribute=false;
				try
				{
					Security::Group_ptr group=(Security::Group_ptr)Registry::GetRegistry()->FindMe("Security/Group",1,field->iSecurityGroupRead.c_str());
					if ( group->IsAuthorised(currentUser) )
						canSeeAttribute=true;
				} catch ( Exceptions::NotFound &nf )
				{
					Core::Log::GetLog()->Error("Meow::Factory::GetAttributes","sss","Can't find ",field->iSecurityGroupRead.c_str()," when parsing a security group read declaration.");
				}
				try
				{
					Security::Group_ptr group=(Security::Group_ptr)Registry::GetRegistry()->FindMe("Security/Group",1,field->iSecurityGroupWrite.c_str());
					if ( group->IsAuthorised(currentUser) )
						canSeeAttribute=true;
				} catch ( Exceptions::NotFound &nf )
				{
					Core::Log::GetLog()->Error("Meow::Factory::GetAttributes","sss","Can't find ",field->iSecurityGroupWrite.c_str()," when parsing a security group write declaration.");
				}
				if ( canSeeAttribute )
					retval->Set(key->GetValueAsString(),(Core::ptr)field);
			}
		}
	}
	return retval;
}

bool Meow::Factory::QuietCanWrite()
{
	bool retval=false;
	try
	{
		CanWrite();
		retval=true;
	} catch ( ... )
	{

	}
	return retval;
}
bool Meow::Factory::QuietCanRead()
{
	bool retval=false;
	try
	{
		CanRead();
		retval=true;
	} catch ( ... )
	{

	}
	return retval;
}
bool Meow::Factory::QuietCanCreate()
{
	bool retval=false;
	try
	{
		CanCreate();
		retval=true;
	} catch ( ... )
	{

	}
	return retval;
}
bool Meow::Factory::QuietCanDelete()
{
	bool retval=false;
	try
	{
		CanDelete();
		retval=true;
	} catch ( ... )
	{

	}
	return retval;
}
int Meow::Factory::GetVersion()
{
	return iVersion;
}
Meow::Core::Types::Array_ptr Meow::Factory::GetSearchableParts( )
{
	return iSearchableParts;	
}
std::string Meow::Factory::GetAliasFormat()
{
	return "";
}
Meow::Core::Types::Array_ptr Meow::Factory::Search( std::string , unsigned long offset, unsigned long limit, std::string service, Core::Types::Array_ptr constraints )
{
	Core::Types::Array_ptr retval=new Core::Types::Array();
	return retval;
}
unsigned long Meow::Factory::CountSearch( std::string, std::string,Core::Types::Array_ptr constraints )
{
	return 0;
}

Meow::SchemaField_ptr Meow::Factory::GetAttributeByAlias( std::string alias )
{
	return (Meow::SchemaField_ptr)iSchemeAttributesByAlias->Get(alias);
}
Meow::SchemaField_ptr Meow::Factory::GetKeyAttributeByAlias( std::string alias )
{
	return (Meow::SchemaField_ptr)iSchemeKeysByAlias->Get(alias);
}
void Meow::Factory::AddCallableMethod( 
				std::string method, 
				Core::Types::Array_ptr parameters,
				std::string label
		)
{
	Method_ptr methodptr=new Method;
	methodptr->iParameters=parameters;
	methodptr->iButtonLabel=label;
	methodptr->iMethodName=method;
	iCallableMethodList->Set(method,(Core::ptr)methodptr);
}
Meow::Core::Types::Array_ptr Meow::Factory::GetCallableMethodList()
{
	return iCallableMethodList;
}
void Meow::Factory::AddCallableFactoryMethod( 
				std::string method, 
				Core::Types::Array_ptr parameters,
				std::string label
		)
{
	Method_ptr methodptr=new Method;
	methodptr->iParameters=parameters;
	methodptr->iButtonLabel=label;
	methodptr->iMethodName=method;
	iCallableFactoryMethodList->Set(method,(Core::ptr)methodptr);
}
void Meow::Factory::SetOrderByColumn( std::string columnname )
{
	iDefaultOrderBy=columnname;
}
void Meow::Factory::DescendingOrder()
{
	iDescendingOrder=true;
}
void Meow::Factory::AscendingOrder()
{
	iDescendingOrder=false;
}
bool Meow::Factory::GetDescending()
{
	return iDescendingOrder;
}
std::string Meow::Factory::GetOrderByColumn()
{
	return iDefaultOrderBy;
}
std::string Meow::Factory::GetInterface()
{
	std::string xml="<Interface>";
	xml.append("<Meta>");
		xml.append("<Class>" + GetMeowClassName() + "</Class>");
	xml.append("</Meta>");
	xml.append("<Factory>");
	Core::Types::Stack_ptr methodNames=iCallableFactoryMethodList->GetKeys();
	while ( !methodNames->IsEmpty() )
	{
		Core::Types::ValueType_ptr methodName=(Core::Types::ValueType_ptr)methodNames->Pop();
		xml.append("<Method>");
		Method_ptr method=(Method_ptr)iCallableFactoryMethodList->Get(methodName->GetValueAsString());
		xml.append("<MethodName>" + method->iMethodName + "</MethodName>");
		xml.append("<ButtonLabel>" + method->iButtonLabel + "</ButtonLabel>");
		Core::Types::Stack_ptr parameterNames=(Core::Types::Stack_ptr)method->iParameters->GetKeys();
		while ( !parameterNames->IsEmpty() )
		{
			Core::Types::ValueType_ptr parameterName=(Core::Types::ValueType_ptr)parameterNames->Pop();
			Core::Types::ValueType_ptr parameterType=(Core::Types::ValueType_ptr)method->iParameters->Get(parameterNames->GetValueAsString());
			xml.append("<Parameter>");
			xml.append("<ParameterName>" + parameterName->GetValueAsString() + "</ParameterName>");
			xml.append("<ParameterType>" + parameterType->GetValueAsString() + "</ParameterType>");
			xml.append("</Parameter>");
		}
		xml.append("</Method>");
	}
	xml.append("</Factory>");
	xml.append("<Instance>");
	Core::Types::Stack_ptr methodNames=iCallableFactoryMethodList->GetKeys();
	while ( !methodNames->IsEmpty() )
	{
		Core::Types::ValueType_ptr methodName=(Core::Types::ValueType_ptr)methodNames->Pop();
		xml.append("<Method>");
		Method_ptr method=(Method_ptr)iCallableFactoryMethodList->Get(methodName->GetValueAsString());
		xml.append("<MethodName>" + method->iMethodName + "</MethodName>");
		xml.append("<ButtonLabel>" + method->iButtonLabel + "</ButtonLabel>");
		Core::Types::Stack_ptr parameterNames=(Core::Types::Stack_ptr)method->iParameters->GetKeys();
		while ( !parameterNames->IsEmpty() )
		{
			Core::Types::ValueType_ptr parameterName=(Core::Types::ValueType_ptr)parameterNames->Pop();
			Core::Types::ValueType_ptr parameterType=(Core::Types::ValueType_ptr)method->iParameters->Get(parameterNames->GetValueAsString());
			xml.append("<Parameter>");
			xml.append("<ParameterName>" + parameterName->GetValueAsString() + "</ParameterName>");
			xml.append("<ParameterType>" + parameterType->GetValueAsString() + "</ParameterType>");
			xml.append("</Parameter>");
		}
		xml.append("</Method>");
	}
	xml.append("<KeyElements>");
		
	xml.append("</KeyElements>");
	xml.append("</Instance>");
	xml.append("</Interface>");
	return xml;
}
Meow::Core::Types::Array_ptr Meow::Factory::GetCallableFactoryMethodList()
{
	return iCallableFactoryMethodList;
}
std::string Meow::Factory::GetFriendlyName()
{
	return GetMeowClassName();
}
bool Meow::Factory::IsPublicApplication()
{
	return false;
}
Meow::SchemaOperation::SchemaOperation() : iVersion(0), iOperation(0)
{
	SetMeowClassName("SchemaOperation");
}
Meow::SchemaOperation::~SchemaOperation()
{
}
Meow::Method::Method()
{
	SetMeowClassName("Method");
}
Meow::Method::~Method()
{
}

