#include <Registry.h>
#include <Core/Types/Stack.h>
#include <Persistence/Update.h>
#include <Persistence/Database.h>
#include <Persistence/Insert.h>
#include <Persistence/Select.h>
#include <Persistence/Delete.h>
#include <Security/User.h>
#include <Security/Group.h>
#include <inc/configuration.h>
#include <Core/Log.h>
#include <iostream>
#include <Core/Storage/File.h>
#include <Core/Storage/Directory.h>
#include <Core/NameValueStore.h>
#include <Session.h>
#include <Core/Types/Boolean.h>
#include <XML/ObjectParser.h>

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif



Meow::PerformanceEntry::PerformanceEntry()
{
	SetMeowClassName("PerformanceEntry");
	recordType=-1;
	eyecatcher="";
	start=0;
	end=0;
	StartTimer();
}
Meow::PerformanceEntry::~PerformanceEntry()
{
	
}

void Meow::PerformanceEntry::StartTimer()
{
#ifdef WIN32
#else
	struct timeval t;
	gettimeofday(&t,NULL);
	start=t.tv_sec*1000000;
	start+=t.tv_usec;
#endif
}
void Meow::PerformanceEntry::EndTimer()
{
#ifdef WIN32
#else
	struct timeval t;
	gettimeofday(&t,NULL);
	end=t.tv_sec*1000000;
	end+=t.tv_usec;
#endif
}
void Meow::PerformanceEntry::Test()
{
	switch (recordType)
	{
	  case PERFORMANCE_REGISTRY_FINDFACTORY:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_FINDFACTORY && Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_FINDFACTORY>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_REGISTRY_FINDOBJECT:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_FINDOBJECT && Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_FINDOBJECT>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_REGISTRY_DELETEOBJECT:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_DELETEOBJECT && Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_DELETEOBJECT>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		  break;
	  case PERFORMANCE_REGISTRY_CREATEOBJECT:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_CREATEOBJECT && Meow::Registry::GetRegistry()->iPERFORMANCE_REGISTRY_CREATEOBJECT>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_CALL:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_CALL && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_CALL>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_ENUMERATE :
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_ENUMERATE && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_ENUMERATE>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_COUNT:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_COUNT && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_COUNT>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_METHOD:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_METHOD && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_METHOD>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_RESTORE:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_RESTORE && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_RESTORE>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_INSERT:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_INSERT && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_INSERT>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_UPDATE:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_UPDATE && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_UPDATE>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OBJECT_DELETE:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_DELETE && Meow::Registry::GetRegistry()->iPERFORMANCE_OBJECT_DELETE>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_SUBSYS_IO:
		   Meow::Registry::GetRegistry()->iIO+=(end-start);
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_SUBSYS_IO && Meow::Registry::GetRegistry()->iPERFORMANCE_SUBSYS_IO>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_SUBSYS_DATA:
		   Meow::Registry::GetRegistry()->iDatabaseTime+=(end-start);
		  
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_SUBSYS_DATA && Meow::Registry::GetRegistry()->iPERFORMANCE_SUBSYS_DATA>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }		break;
	  case PERFORMANCE_OVERALL:
		   if ( ((end-start)/1000) > Meow::Registry::GetRegistry()->iPERFORMANCE_OVERALL && Meow::Registry::GetRegistry()->iPERFORMANCE_OVERALL>0 )
		   {
			  PerformanceEntry_ptr entry=this;
			  Meow::Registry::GetRegistry()->RegisterPerformance(entry);
			  
		   }
		break;
	}
}
std::string Meow::PerformanceEntry::ToString()
{	

	if ( !parameters.IsNull() )
	{
		eyecatcher.append("(");
		bool found=false;
		Core::Types::Stack_ptr parmKeys=parameters->GetKeys();
		while ( !parmKeys->IsEmpty() )
		{
		   
			if ( found )
				eyecatcher.append(",");
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)parmKeys->Pop();
			Core::ptr value=parameters->Get(key->GetValueAsString());
			if  ( value.IsNull() )
			{
				eyecatcher.append("NULL");
			}
			else if ( value->IsInstanceOf("ValueType") )
			{
				eyecatcher.append(((Core::Types::ValueType_ptr)value)->GetValueAsString());
			}
			else if ( value->IsInstanceOf("Object") )
			{
			    eyecatcher.append(((Object_ptr)value)->ToString());
			}
			else 
			{
				eyecatcher.append(value->GetMeowClassName());
			}
			found=true;
		}
	}
	double asSeconds=((double)end-(double)start)/1000000.0;
	eyecatcher.append(") took " + Core::Types::ValueType::FloatToString(asSeconds) + " seconds.");
	return eyecatcher;
}

Meow::EventManifestLoader::EventManifestLoader()
{
    SetMeowClassName("EventManifestLoader");
    iEvents=new Core::Types::Array();
}
Meow::EventManifestLoader::~EventManifestLoader()
{
}
void Meow::EventManifestLoader::StartElement( string element, Core::Types::Array_ptr attributes )
{
    if ( !element.compare("Subscribe") )
    {
        Core::Types::ValueType_ptr event=(Core::Types::ValueType_ptr)attributes->Get("event");
        Core::Types::ValueType_ptr factory=(Core::Types::ValueType_ptr)attributes->Get("class");
        if ( !event.IsNull() && !factory.IsNull() )
        {
            Core::Types::Array_ptr subscribers;

            if ( !iEvents->IsSet(event->GetValueAsString()))
            {
                subscribers=new Core::Types::Array();
                iEvents->Set(event->GetValueAsString(),(Core::ptr)subscribers);
            }
            else
                subscribers=(Core::Types::Array_ptr)iEvents->Get(event->GetValueAsString());
            subscribers->Set(subscribers->Count(),(Core::ptr)factory);
        }
    }
}
void Meow::EventManifestLoader::EndElement( string element )
{
}
void Meow::EventManifestLoader::CharacterData( string data )
{
}
void Meow::EventManifestLoader::OnEvent( std::string eventName )
{
    if ( iEvents->IsSet(eventName) )
    {
        std::string message="Manifest listener found for [" + eventName + "]";
        Core::Types::Array_ptr factories=(Core::Types::Array_ptr)iEvents->Get(eventName);
        for ( int i=0; i<factories->Count(); i++ )
        {
        	Core::Types::String_ptr factory=(Core::Types::String_ptr)factories->Get(i);
            try
            {
                std::string message="Lazy loading [" + factory->GetValue() + "]";
                Meow::Factory_ptr fac=Meow::Registry::GetRegistry()->FindFactory(factory->GetValue()+"Factory");
            } catch ( ... )
            {
                std::string message="Could not find factory:" + factory->GetValue() + "Factory";
            }
        }
    }
    else
    {
        std::string message="Could not find a manifest listener for [" + eventName + "]";
    }
}


Meow::RegistryEntry::RegistryEntry()
{
    CollectionID="Guests";
    Accesses=0;
    ObjectsHeld=0;
}

Meow::RegistryEntry::~RegistryEntry()
{
}

Meow::LibraryEntry::LibraryEntry()
{
    References=0;
}

Meow::LibraryEntry::~LibraryEntry()
{

}

Meow::EventEntry::EventEntry( )
{
}

Meow::EventEntry::~EventEntry( )
{
}


void Meow::Registry::RegisterPerformance( Meow::PerformanceEntry_ptr perf )
{
	PerformanceMetrics->Push((Core::ptr)perf);
}

Meow::Registry::Registry()
{
    intransaction=false;
    TransactionalObjects=new Core::Types::Array();
    ServiceIndex=new Core::Types::Array();
    LookupService=new Core::Types::Array();
    NotificationRegistry=new Core::Types::Array();
    LookupService=new Core::Types::Array();
    PermittedEvents=new Core::Types::Array();
    DeniedEvents=new Core::Types::Array();
    Libraries=new Core::Types::Array();
    Constraints=new Core::Types::Array();
    iNameValueStore=new Core::NameValueStore();
    iClassSubscriptions=new Core::Types::Array();
    forbidsessions=true;

    DelayedEvents=new Core::Types::Stack();
    sessionready=false;
    iSession=NULL;
    batchmode=true;
    userChanged=true;
    Manifests=new Core::Types::Array();
    Controller=NULL;
	PerformanceMetrics=new Core::Types::Queue();
	ReadonlyEvents=new Core::Types::Array();
	iDatabaseTime=0;
	iIO=0;
	iCompiler=0;
	readonlymode=false;
	
    SetMeowClassName("Registry");
	
    RegisterMe(new Meow::Security::UserFactory());
    RegisterMe(new Meow::Security::GroupFactory());

	
	iPERFORMANCE_REGISTRY_FINDFACTORY=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.registry.findfactory"));
	iPERFORMANCE_REGISTRY_FINDOBJECT=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.registry.findobject"));
	iPERFORMANCE_REGISTRY_DELETEOBJECT=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.registry.deleteobject"));
	iPERFORMANCE_REGISTRY_CREATEOBJECT=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.registry.createobject"));
	iPERFORMANCE_OBJECT_CALL=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.call"));
	iPERFORMANCE_OBJECT_ENUMERATE=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.enumerate"));
	iPERFORMANCE_OBJECT_COUNT=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.count"));
	iPERFORMANCE_OBJECT_METHOD=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.method"));
	iPERFORMANCE_OBJECT_RESTORE=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.restore"));
	iPERFORMANCE_OBJECT_INSERT=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.insert"));
	iPERFORMANCE_OBJECT_UPDATE=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.update"));
	iPERFORMANCE_OBJECT_DELETE=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.object.delete"));
	iPERFORMANCE_SUBSYS_IO=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.subsys.io"));
	iPERFORMANCE_SUBSYS_DATA=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.subsys.data"));
	iPERFORMANCE_OVERALL=Core::Types::ValueType::StringToInteger(Configuration::GetConfiguration()->Get("performance.overall"));
}

Meow::Registry::~Registry()
{

}

void Meow::Registry::__Destroy( )
{

}



Meow::Core::NameValueStore_ptr Meow::Registry::GetHeap()
{
    return iNameValueStore;
}

Meow::Core::Types::Array_ptr Meow::Registry::InterpretKey( std::string data )
{
	return DeCompileKey(data);
}
std::string Meow::Registry::CompileKey( Meow::Core::Types::Array_ptr key )
{
	// TODO: Extend this to encode / that appear in the key
    std::string retval="";
    int i=0;
    for ( i=0; i<key->Count(); i++ )
    {
    	Core::ptr keyMember=key->Get(i);
		if ( i>0 )
    	    retval.append("/");
		std::string subText="";
		
		if ( keyMember->IsInstanceOf("ValueType") )
		     subText=((Core::Types::ValueType_ptr)keyMember)->GetValueAsString();
		else
		     subText=((LifecycleObject_ptr)keyMember)->ToString();
		std::size_t found = subText.find("/");
		bool specialCharsFound=false;
		if (found!=std::string::npos)
		{
			Core::Types::String_ptr plainText=new Core::Types::String(subText);
			Core::Types::ValueType_ptr encoded64=plainText->Base64Encode();
			subText="~" + encoded64->GetValueAsString();
		}
		retval.append(subText);
    }
    return retval;
}

Meow::Core::Types::Array_ptr Meow::Registry::DeCompileKey(std::string key )
{
	// TODO: Extend this to decode / that appear in the key
    Core::Types::Array_ptr retval = new Core::Types::Array();
    Meow::Core::Types::String_ptr key_as_string = new Core::Types::String(key);
    retval = key_as_string->Explode("/");
	for ( int i=0; i<retval->Count(); i++ )
	{
		Core::Types::String_ptr keyComponent=(Core::Types::String_ptr)retval->Get(i);
		if ( keyComponent->FirstChar()=='~' )
		{
			Core::Types::ValueType_ptr decoded=keyComponent->Base64Decode();
			retval->Set(i,new Core::Types::String(decoded->GetValueAsString()));
		}
	}
    return retval;
}

void Meow::Registry::SetController( Meow::Object_ptr value )
{
    if ( Controller.IsNull() )
        Controller=value;
}

void Meow::Registry::RegisterMe( Meow::Factory_ptr factory )
{
    std::string factoryname;

    factory->iRegistry=this;
    factoryname=factory->GetMeowClassName();

    if ( LookupService->IsSet(factoryname) )
    {
        std::string message="Already registered:" + factoryname;
        return;
    }

    RegistryEntry_ptr entry=new RegistryEntry();
    entry->Instance=factory;
    entry->Accesses=0;
    entry->CollectionID="Guests";

    LookupService->Set(factoryname,(Core::ptr)entry);

    Core::Types::Stack_ptr Services=factory->GetServices()->GetKeys();
    while ( ! Services->IsEmpty() )
    {
    	Core::Types::String_ptr servicename=(Core::Types::String_ptr)Services->Pop();

        if ( ! ServiceIndex->IsSet(servicename->GetValue()) )
        {
        	Core::Types::Array_ptr service=new Core::Types::Array();
            service->Set(0,(Core::ptr)factory);
            ServiceIndex->Set(servicename->GetValue(),(Core::ptr)service);
        }
        else
        {
            Core::Types::Array_ptr service;
            service=(Core::Types::Array_ptr)ServiceIndex->Get(servicename->GetValue());
            service->Set(service->Count(),(Core::ptr)servicename);
        }
    }
    factory->RegisterEvents();
}

void Meow::Registry::InstallExtensions( )
{
/*    Core::Types::Array_ptr extensions=Configuration::GetConfiguration()->GetExtensions();
    Core::Types::Stack_ptr keys=extensions->GetKeys();
    while ( !keys->IsEmpty() )
    {
    	Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
        Core::System::LinkLibrary_ptr lib=(Core::System::LinkLibrary_ptr)extensions->Get(key->GetValue());
        void * fptr=lib->GetFunction("InstallFactories");
        void   (*InstallFactories)(Meow::Registry_ptr)=(void (*)(Meow::Registry_ptr))fptr;
        InstallFactories(this);
    } */
}

Meow::Core::Types::Stack_ptr Meow::Registry::EnumerateRegistered( )
{
    return LookupService->GetKeys();
}

Meow::RegistryEntry_ptr Meow::Registry::Details( std::string factoryname )
{
    Meow::RegistryEntry_ptr details;
    if ( !LookupService->IsSet(factoryname) )
    {
        throw Exceptions::NotFound();
    }
    details=(Meow::RegistryEntry_ptr)LookupService->Get(factoryname);
    details->ObjectsHeld=details->Instance->CountObjectsHeld();
    return details;
}

void Meow::Registry::DisposeOf( std::string factoryname )
{
    Meow::RegistryEntry_ptr details;
    if ( ! LookupService->IsSet(factoryname) )
    {
        throw Exceptions::NotFound();
    }
    details=(Meow::RegistryEntry_ptr)LookupService->Get(factoryname);
    details->Instance->Purge();
    details->Instance->__Destroy();
    if ( !details->BackwardsReference.IsNull() )
    {
        details->BackwardsReference->References--;
    }

    LookupService->Unset(factoryname);
    Core::Types::Stack_ptr Entries=ServiceIndex->GetValues();
    while ( ! Entries->IsEmpty() )
    {
        Core::Types::Array_ptr Entry=(Core::Types::Array_ptr)Entries->Pop();
        Core::Types::Stack_ptr Keys=(Core::Types::Stack_ptr)Entry->GetKeys();
        while ( ! Keys->IsEmpty() )
        {
        	Core::Types::String_ptr Key=(Core::Types::String_ptr)Keys->Pop();
            Factory_ptr factory=(Factory_ptr)Entry->Get(Key->GetValue());
            if ( factory->GetMeowClassName()==factoryname )
            {
                Entry->Unset(Key->GetValue());
            }
        }
    }
}

void Meow::Registry::Purge()
{
    Core::Types::Stack_ptr Instances=LookupService->GetValues();
    while ( !Instances->IsEmpty() )
    {
        RegistryEntry_ptr Entry=(RegistryEntry_ptr)Instances->Pop();
        try
        {
            Entry->Instance->Purge();
        } catch ( Exception & excep )
        {
            std::string message=excep.Diagnostic();
        }
    }
}

void Meow::Registry::GarbageCollect( )
{
    Core::Types::Stack_ptr Instances=LookupService->GetValues();
    while ( !Instances->IsEmpty() )
    {
        RegistryEntry_ptr Entry=(RegistryEntry_ptr)Instances->Pop();
        try
        {
            Entry->Instance->GarbageCollect();
        } catch ( Exception & excep )
        {
            std::string message=excep.Diagnostic();
        }
    }
}

void Meow::Registry::Passivate()
{
    Core::Types::Stack_ptr Instances=LookupService->GetValues();
    while ( !Instances->IsEmpty() )
    {
        RegistryEntry_ptr Entry=(RegistryEntry_ptr)Instances->Pop();
        if ( ! Entry->Instance->SecondPhasePurge() )
        {
            try
            {
//		Core::Log::GetLog()->Information("Meow::Registry::Passivate","ss","Phase 1 Passivating:",Entry->Instance->GetMeowClassName().c_str());	
                Entry->Instance->Passivate();
            } catch ( Exception & excep )
            {
                std::string message=excep.Diagnostic();
                Core::Log::GetLog()->Error("Meow::Registry::Passivate","s",message.c_str());

            }
        }
//	else
//		Core::Log::GetLog()->Information("Meow::Registry::Passivate","ss","Phase 1 Passivation blocked:",Entry->Instance->GetMeowClassName().c_str());
    }
    Instances=LookupService->GetValues();
    while ( !Instances->IsEmpty() )
    {
        RegistryEntry_ptr Entry=(RegistryEntry_ptr)Instances->Pop();
        if ( Entry->Instance->SecondPhasePurge() )
        {
            try
            {
//		Core::Log::GetLog()->Information("Meow::Registry::Passivate","ss","Phase 2 Passivating:",Entry->Instance->GetMeowClassName().c_str());	
                Entry->Instance->Passivate();
            } catch ( Exception & excep )
            {
                std::string message=excep.Diagnostic();
                Core::Log::GetLog()->Error("Meow::Registry::Passivate","s",message.c_str());
            }
        }
//	else
//		Core::Log::GetLog()->Information("Meow::Registry::Passivate","ss","Phase 2 Passivation blocked:",Entry->Instance->GetMeowClassName().c_str());

    }
}
Meow::Core::Types::Stack_ptr Meow::Registry::GetFactoryList()
{
	return LookupService->GetKeys();
}

void Meow::Registry::Cleanup( )
{
    try
    {
        Passivate();
        Purge();
        Core::Types::Stack_ptr Instances=LookupService->GetValues();
        while ( !Instances->IsEmpty() )
        {
            RegistryEntry_ptr Entry=(RegistryEntry_ptr)Instances->Pop();
            std::string factoryname=Entry->Instance->GetMeowClassName();

            Entry->Instance->__Destroy();
        }
    } catch ( ... )
    {
    }
}

void Meow::Registry::Flush( std::string factoryname )
{
    try
    {
        Factory_ptr factory=FindFactory(factoryname);
        factory->Flush();
    } catch ( Exceptions::NotFound &nf )
    {
    	std::string message=nf.Diagnostic();
        Core::Log::GetLog()->Error("Meow::Registry::Passivate","s",message.c_str());
    }
}

Meow::Factory_ptr Meow::Registry::FindFactory( std::string factoryname )
{
#ifdef MEOW_PERFORMANCE_ON
	Core::Types::Array_ptr parms=new Core::Array();
	parms->Set("factoryname",new String(factoryname));
	MEOW_PERFORMANCE_ENTRY(findFactoryTimer,"FindFactory",PERFORMANCE_REGISTRY_FINDFACTORY,parms);
#endif
    Factory_ptr retval;
    if ( LookupService->IsSet(factoryname) )
    {
        Meow::RegistryEntry_ptr entry=(Meow::RegistryEntry_ptr)LookupService->Get(factoryname);
        retval=entry->Instance;
    }
    else
	{
    	// TODO - Add lazy loader for other language environments.
#ifdef MEOW_PERFORMANCE_ON
		MEOW_PERFORMANCE_EXIT(findFactoryTimer);
#endif
		Exceptions::NotFound e;
		e.SetMessage("Could not find factory " + factoryname);
        throw e;
	}
#ifdef MEOW_PERFORMANCE_ON
    MEOW_PERFORMANCE_EXIT(findFactoryTimer);
#endif
    return retval;
}

void Meow::Registry::InternalRegisterWithLibrary( Meow::Factory_ptr factory, Meow::LibraryEntry_ptr library )
{
    std::string factoryname;

    factoryname=factory->GetMeowClassName();
    if ( LookupService->IsSet(factoryname) )
        throw Exceptions::Duplicate();


    RegistryEntry_ptr entry=new RegistryEntry();
    entry->Instance=factory;
    entry->Accesses=0;
    entry->CollectionID="Guests";
    entry->BackwardsReference=library;
    entry->BackwardsReference->References++;

    LookupService->Set(factoryname,(Core::ptr)entry);

    Core::Types::Stack_ptr Services=factory->GetServices()->GetKeys();
    while ( ! Services->IsEmpty()==0 )
    {
    	Core::Types::String_ptr servicename=(Core::Types::String_ptr)Services->Pop();
        if ( ! ServiceIndex->IsSet(servicename->GetValue()) )
        {
            Core::Types::Array_ptr service=new Core::Types::Array();
            service->Set(0,(Core::ptr)factory);
            ServiceIndex->Set(servicename->GetValue(),(Core::ptr)service);
        }
        else
        {
            Core::Types::Array_ptr service;
            service=(Core::Types::Array_ptr)ServiceIndex->Get(servicename->GetValue());
            service->Set(service->Count(),(Core::ptr)servicename);
        }
    }
}

Meow::Registry_ptr Meow::Registry::GetRegistry()
{
    if ( gRegistry.IsNull() )
        gRegistry=new Meow::Registry();
    return gRegistry;
}


Meow::Registry_ptr Meow::Registry::gRegistry;

Meow::LifecycleObject_ptr Meow::Registry::FindMeByObjectReference( std::string reference )
{
    Meow::LifecycleObject_ptr retval;
    Core::Types::String_ptr ref=new Core::Types::String(reference);
    Meow::Core::Types::Array_ptr objectReferenceComponents=ref->Explode("@");
    if ( objectReferenceComponents->Count() == 2 )
    {
    	Core::Types::String_ptr className=(Core::Types::String_ptr)objectReferenceComponents->Get(0);
		// DS, kludgy hack, because we really don't want people using / in key components but seems
		// they can't help themselves. So find out how big the key is for a given class.	
		Factory_ptr factory=FindFactory(className->GetValueAsString() + "Factory");
		Core::Types::Array_ptr schemeKeys=factory->GetSchemeKeys();
		if ( schemeKeys->Count() > 1 )
		{		
			// Why a kludge? Because this won't help you with multiple key members whose
			// been unfortunate enough to use a / in member.
	    	Core::Types::String_ptr key=(Core::Types::String_ptr)objectReferenceComponents->Get(1);
	        Core::Types::Array_ptr keyComponents=DeCompileKey(key->GetValueAsString());
			// Another kludge, if someone hasn't specified the key properly, pad with empty strings
			// even if thats BAD, - we should really check the type and pump that in correctly.
			while ( keyComponents->Count() < schemeKeys->Count() )
			{
				keyComponents->Set(keyComponents->Count(),new Core::Types::String(""));
			}
    	    retval=FindMe(className->GetValueAsString(),keyComponents);
		}
		else
		{
	    	Core::Types::String_ptr key=(Core::Types::String_ptr)objectReferenceComponents->Get(1);
			Core::Types::Array_ptr keyComponents=DeCompileKey(key->GetValueAsString());
    	    retval=FindMe(className->GetValueAsString(),keyComponents);
		}
    }
    else
    {
        Meow::Exceptions::NotFound notfound;
        notfound.SetMessage("Invalid key:" + reference);
        throw notfound;
    }
    return retval;
}

Meow::LifecycleObject_ptr Meow::Registry::FindMe( std::string classname, Core::Types::Array_ptr key )
{
#ifdef MEOW_PERFORMANCE_ON
	Core::Types::Array_ptr parms=new Core::Array();
	parms->Set("classname",new String(classname));
	parms->Set("key",(ptr)key);
	MEOW_PERFORMANCE_ENTRY(findMeTimer,"FindMe",PERFORMANCE_REGISTRY_FINDOBJECT,parms);
#endif  
    std::string realclassname=classname;
    realclassname.append("Factory");
    Meow::Factory_ptr factory=FindFactory(realclassname);
    Meow::LifecycleObject_ptr retval=factory->FindByPrimaryKey(key);
#ifdef MEOW_PERFORMANCE_ON
	MEOW_PERFORMANCE_EXIT(findMeTimer);
#endif
    return retval;
}

Meow::LifecycleObject_ptr Meow::Registry::FindMe( std::string classname, std::string keyvalue )
{
    return FindMe(classname,Registry::InterpretKey(keyvalue));
}

Meow::LifecycleObject_ptr Meow::Registry::FindMe( std::string classname, const char * keyvalues[] )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    const char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    return FindMe(classname,key);
}

Meow::LifecycleObject_ptr Meow::Registry::FindMe( std::string classname, char * keyvalues[] )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    return FindMe(classname,key);
}

Meow::LifecycleObject_ptr Meow::Registry::FindMe( std::string classname,
        int argcount,
        ... )
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
    return FindMe(classname,key);
}

Meow::LifecycleObject_ptr Meow::Registry::FindMe( std::string classname )
{
    Factory_ptr factory=FindFactory(classname + "Factory");
    return FindMe(classname,factory->GetAnonymousKey());
}

Meow::LifecycleObject_ptr Meow::Registry::CreateMe( std::string classname, std::string keyvalue )
{
    return CreateMe(classname,Registry::InterpretKey(keyvalue));
}


Meow::LifecycleObject_ptr Meow::Registry::CreateMe( std::string classname, Core::Types::Array_ptr key )
{
#ifdef MEOW_PERFORMANCE_ON
  	Core::Types::Array_ptr parms=new Core::Types::Array();
	parms->Set("classname",new String(classname));
	parms->Set("key",(ptr)key);
	MEOW_PERFORMANCE_ENTRY(createMeTimer,"CreateMe",PERFORMANCE_REGISTRY_CREATEOBJECT,parms);
#endif
    Meow::LifecycleObject_ptr object;
    std::string realclassname=classname;
    realclassname.append("Factory");
    Meow::Factory_ptr factory=FindFactory(realclassname);
    object=factory->CreateByPrimaryKey(key,classname);
#ifdef MEOW_PERFORMANCE_ON
	MEOW_PERFORMANCE_EXIT(createMeTimer);
#endif
    return object;

}

Meow::LifecycleObject_ptr Meow::Registry::CreateMe( std::string classname, const char * keyvalues[] ) 
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    const char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    return CreateMe(classname,key);
}


Meow::LifecycleObject_ptr Meow::Registry::CreateMe( std::string classname, char * keyvalues[] ) 
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    return CreateMe(classname,key);
}


Meow::LifecycleObject_ptr Meow::Registry::CreateMe( std::string classname,
        int argcount,
        ... ) 
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
    return CreateMe(classname,key);
}

Meow::LifecycleObject_ptr Meow::Registry::CreateMe( std::string classname ) 
{
    Factory_ptr factory=FindFactory(classname + "Factory");
    return CreateMe(classname,factory->GetAnonymousKey());
}


Meow::LifecycleObject_ptr Meow::Registry::GetMe( std::string classname, std::string keyvalue ) 
{
    return GetMe(classname,Registry::InterpretKey(keyvalue));
}


Meow::LifecycleObject_ptr Meow::Registry::GetMe( std::string classname, Core::Types::Array_ptr key ) 
{
    Meow::LifecycleObject_ptr retval;
    try
    {
        retval=FindMe(classname,key);
    } catch ( Meow::Exceptions::NotFound &e )
    {
        retval=CreateMe(classname,key);
    }
    return retval;
}

Meow::LifecycleObject_ptr Meow::Registry::GetMe( std::string classname, const char * keyvalues[] ) 
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    const char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    return GetMe(classname,key);
}


Meow::LifecycleObject_ptr Meow::Registry::GetMe( std::string classname, char * keyvalues[] )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    return GetMe(classname,key);
}


Meow::LifecycleObject_ptr Meow::Registry::GetMe( std::string classname,
        int argcount,
        ... )
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
    return GetMe(classname,key);
}

Meow::LifecycleObject_ptr Meow::Registry::GetMe( std::string classname ) 
{
    Factory_ptr factory=FindFactory(classname + "Factory");
    LifecycleObject_ptr retval=GetMe(classname,factory->GetAnonymousKey());
    return retval;
}


void Meow::Registry::DestroyMe( std::string classname, std::string keyvalue )
{
    return DestroyMe(classname,Registry::InterpretKey(keyvalue));
}



void Meow::Registry::DestroyMe( std::string classname, Core::Types::Array_ptr key )
{
#ifdef MEOW_PERFORMANCE_ON
    Core::Types::Array_ptr parms=new Core::Array();
	parms->Set("classname",new String(classname));
	parms->Set("key",(ptr)key);
	MEOW_PERFORMANCE_ENTRY(destroyMeTimer,"DestroyMe",PERFORMANCE_REGISTRY_DELETEOBJECT,parms);
#endif
    std::string realclassname=classname;
    realclassname.append("Factory");
    Meow::Factory_ptr factory=FindFactory(realclassname);
    factory->DestroyByPrimaryKey(key);
#ifdef MEOW_PERFORMANCE_ON
	MEOW_PERFORMANCE_EXIT(destroyMeTimer);
#endif
}

void Meow::Registry::DestroyMe( std::string classname, const char *  keyvalues[] )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    const char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    DestroyMe(classname,key);
}


void Meow::Registry::DestroyMe( std::string classname, char *  keyvalues[] )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    char * keyptr=NULL;
    int index=0;
    keyptr=keyvalues[index];
    while (keyptr!=NULL)
    {
    	Core::Types::String_ptr str=new Core::Types::String();
        str->SetValue(keyptr);
        key->Set(index,(Core::ptr)str);
        index++;
        keyptr=keyvalues[index];
    }
    DestroyMe(classname,key);
}


void Meow::Registry::DestroyMe( std::string classname,
                                  int argcount,
                                  ... ) 
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
    DestroyMe(classname,key);
}


bool Meow::Registry::IsEventPermitted( std::string eventname )
{
    bool found=false;
    for ( int i=0; i<DeniedEvents->Count(); i++ )
    {
    	Meow::Core::Types::String_ptr eventid=(Meow::Core::Types::String_ptr)DeniedEvents->Get(i);
        if ( !eventid->GetValue().compare(eventname) )
        {
            found=true;
            break;
        }
    }
    if ( found )
        return false;
    for ( int i=0; i<PermittedEvents->Count(); i++ )
    {
    	Meow::Core::Types::String_ptr eventid=(Meow::Core::Types::String_ptr)PermittedEvents->Get(i);
        if ( !eventid->GetValue().compare(eventname) )
        {
            found=true;
            break;
        }
    }
    return found;
}
void Meow::Registry::PostEvent( std::string eventname, Core::Types::Array_ptr arguments )
{
    // If its a delayed event it appears as "delay.event.[eventname]" in conf file
    std::string delay_check="delay.event." + eventname;
    if ( Configuration::GetConfiguration()->IsSet(delay_check) )
    {
        QueueDelayedEvent(eventname,arguments);
        return;
    }
    PostEventDirect(eventname,arguments);
}
void Meow::Registry::PostEvent( std::string eventname, int argcount, ... )
{
    Meow::Core::Types::Array_ptr key=new Meow::Core::Types::Array();
    va_list ap;
    int i;
    va_start(ap, argcount);
    for (i = 0; i<argcount; i+=2)
    {
        char * param=va_arg(ap,char*);
        std::string real_param=param;
        Meow::Core::Types::String_ptr data=new Meow::Core::Types::String(va_arg(ap, char *));
        key->Set(real_param,(Core::ptr)data);
    }
    va_end(ap);
    return PostEvent(eventname,key);
}
void Meow::Registry::Subscribe( std::string eventname, Factory_ptr classref, Core::Types::Array_ptr predicates )
{
    Core::Types::Stack_ptr as;

    if ( NotificationRegistry->IsSet(eventname) )
        as=(Core::Types::Stack_ptr)NotificationRegistry->Get(eventname);
    else
    {
        as=new Core::Types::Stack();
        NotificationRegistry->Set(eventname,(Core::ptr)as);
    }
    Meow::EventEntry_ptr event=new Meow::EventEntry();
    event->Class=classref;
    event->Predicates=predicates;
    as->Push((Core::ptr)event);
}
Meow::Core::Types::Constraint_ptr Meow::Registry::GetConstraint( std::string regexp )
{
	Meow::Core::Types::Constraint_ptr constraint;
    if ( Constraints->IsSet(regexp) )
        constraint=(Meow::Core::Types::Constraint_ptr)Constraints->Get(regexp);
    else
    {
        constraint=new Meow::Core::Types::Constraint(regexp);
	constraint->SetExpression(regexp);
        Constraints->Set(regexp,(Core::ptr)constraint);
    }
    return constraint;
}
Meow::Object_ptr Meow::Registry::GetCurrentSession()
{
    
    if ( forbidsessions )
    	return (Meow::Object_ptr)Object::Null();

	if ( iSession.IsNull() )
	{
		SessionFactory_ptr factory=(SessionFactory_ptr)FindFactory("SessionFactory");
		iSession=factory->GetCurrentSession();
	}
    return iSession;
}
void Meow::Registry::SetCurrentSession( Object_ptr session )
{
    iSession=session;
}
bool Meow::Registry::GetUserChanged()
{
    return userChanged;
}
void Meow::Registry::SetUserChanged(bool val)
{
    userChanged=val;
}
bool Meow::Registry::IsSessionReady()
{
    return sessionready;
}
void Meow::Registry::SessionReady()
{
    sessionready=true;
}

bool Meow::Registry::IsInBatch( )
{
    return batchmode;
}
void Meow::Registry::CalledByBrowser( )
{
    batchmode=false;
}
void Meow::Registry::BlockSessions()
{
    forbidsessions=true;
}
void Meow::Registry::ReleaseSessions()
{
    forbidsessions=false;
}
Meow::DelayedEventEntry::DelayedEventEntry()
{
    SetMeowClassName("DelayedEventEntry");
}
Meow::DelayedEventEntry::~DelayedEventEntry()
{
}
void Meow::Registry::QueueDelayedEvent( std::string event, Core::Types::Array_ptr parameters )
{
    DelayedEventEntry_ptr eventToStore=new DelayedEventEntry();
    eventToStore->event=event;
    eventToStore->parameters=parameters;
    DelayedEvents->Push((Core::ptr)eventToStore);
}
void Meow::Registry::FireDelayedEvents( )
{
    while ( !DelayedEvents->IsEmpty() )
    {
        DelayedEventEntry_ptr eventToFire=(DelayedEventEntry_ptr)DelayedEvents->Pop();
        PostEventDirect(eventToFire->event,eventToFire->parameters);
    }
}
void Meow::Registry::PostEventDirect( std::string eventname, Core::Types::Array_ptr arguments )
{
	// DS - Modification to check whether we are set readonly, if we are the only
	// events to be passed through are on the whitelist as previously loaded
	// during marking us as readonly in MarkReadonly()

	SyncPoint();
	
	bool readonlymemory=readonlymode;
	if ( readonlymode && !ReadonlyEvents->IsSet(eventname) )
	{
		Core::Log::GetLog()->Security("Meow::Registry::PostEventDirect","sss","Intercepted attempt to call an event [",eventname.c_str(),"] that was not on the whitelist for a readonly session and terminated it early.");
		return;
	}
	else if ( readonlymode )
	{
		// So we get past the previous, all children are bubble down - they do not know
		// we were readonly at the top
		readonlymode=false;
	}
    // Lazy load required classes
	if ( Manifests->Count() == 0 )
    	    Core::Log::GetLog()->Information("Meow::Registry::PostEventDirect","sss","No manifest entries for [",eventname.c_str(),"]");

	    for ( int i=0; i<Manifests->Count(); i++ )
	    {
		EventManifestLoader_ptr handler=(EventManifestLoader_ptr)Manifests->Get(i);
		handler->OnEvent(eventname);
	    }
	    std::string uname="Unknown-User";
	    if ( !GetCurrentSession().IsNull() )
	    {
		Security::User_ptr user=(Security::User_ptr)((Session_ptr)GetRegistry()->GetCurrentSession())->GetCurrentUser();
		if ( !user.IsNull() )
		    uname=user->GetUsername();
	    }
	    if ( arguments.IsNull() )
		arguments=new Core::Types::Array();
	    Core::Log::GetLog()->Events(eventname,uname,arguments);
	
	    if ( NotificationRegistry->IsSet(eventname) )
	    {
		// Check is secure
		try
		{
		    LifecycleObject_ptr object;
		    const char * key[]= { eventname.c_str(), NULL };
		    bool permit=false;
		    bool runsecurity=true;

		    try
		    {
		        if ( !Configuration::GetConfiguration()->Get("event.security").compare("off") )
		        {
		            runsecurity=false;
		            permit=true;
		        }

		    } catch ( ... )
		    {
		    	// TODO : Decide what to do in this instance.
		    }
		    runsecurity=false;
		    if ( runsecurity )
		    {
		        try
		        {
		            object=FindMe("Security/Event",key);
		        } catch ( Exceptions::NotFound &nf )
		        {
		            // Try to get the security collection
		            if ( Configuration::GetConfiguration()->IsSet("default.event.security") )
		            {
		                std::string collectionid=Configuration::GetConfiguration()->Get("default.event.security");
		                Security::Group_ptr collection=(Security::Group_ptr)FindMe("Security/Group",1,collectionid.c_str());
		                object=CreateMe("Security/Event",key);
		                object->SetVariable("Group",(Core::ptr)collection);
		            }
		            else
		            {
		                throw nf;
		            }
		        }
		        Core::Types::Array_ptr params=new Core::Types::Array();
		        Security::Group_ptr group=(Security::Group_ptr)object->GetVariable("Group");
		        if ( !group.IsNull() )
		        {
		        	Security::User_ptr user=(Security::User_ptr)((Session_ptr)GetRegistry()->GetCurrentSession())->GetCurrentUser();

		            if ( !user.IsNull( ) )
		            {
		            	permit=group->IsAuthorised(user);
		            	Security::GroupFactory_ptr groupFac=(Security::GroupFactory_ptr)GetRegistry()->FindFactory("Security/GroupFactory");
		            }
		        }
		    }
  		    Core::Log::GetLog()->Security("Meow::Registry::PostEvent","sss","I'm about to find out if I can post... [",eventname.c_str(),"]");
		    if ( permit )
		    {
		        Core::Types::Stack_ptr classlist=(Core::Types::Stack_ptr)NotificationRegistry->Get(eventname);
		        Core::Types::StackIterater_ptr as=new Core::Types::StackIterater();
		        as->SetStack(classlist);
		        as->First();
		        while (as->HasMoreElements() )
		        {
		            EventEntry_ptr entry=(EventEntry_ptr)as->GetNextElement();
		            // Check the redicates
		            try
		            {
				Core::Log::GetLog()->Security("Meow::Registry::PostEvent","sss","Posting Event To [",entry->Class->GetMeowClassName().c_str(),"]");
		                entry->Class->OnEvent(eventname,arguments);
				Passivate();
		            } catch ( SystemException &e )
		            {
		            	Core::Log::GetLog()->Error("Meow::Registry::PostEvent","ss","OnEvent cycle caught an exception",e.Diagnostic().c_str());
		            }
		        }
		    }
		    else
		    {
		    	Core::Log::GetLog()->Security("Meow::Registry::PostEvent","sss","Permission refused to post event [",eventname.c_str(),"]");
		    }
		} catch ( Exceptions::NotFound &nf )
		{
			Core::Log::GetLog()->Security("Meow::Registry::PostEvent","sss","Could not find permissions for event [",eventname.c_str(),"]");
		}
	    }
	    else
	    	Core::Log::GetLog()->Security("Meow::Registry::PostEvent","sss","No listeners for event [",eventname.c_str(),"]");

		readonlymode=readonlymemory;
	SyncPoint();
}
void Meow::Registry::RecordPerformanceProfile()
{
	while ( !PerformanceMetrics->IsEmpty() )
	{
		PerformanceEntry_ptr perf=(PerformanceEntry_ptr)PerformanceMetrics->Pull();
		std::string message=perf->ToString();
		// Convert 
		switch (perf->recordType)
		{
		  case PERFORMANCE_REGISTRY_FINDFACTORY:
				Core::Log::GetLog()->Performance("performance.registry.findfactory","s",message.c_str());
				break;
		  case PERFORMANCE_REGISTRY_FINDOBJECT:
			  	Core::Log::GetLog()->Performance("performance.registry.findobject","s",message.c_str());
				break;
		  case PERFORMANCE_REGISTRY_DELETEOBJECT:
			  	Core::Log::GetLog()->Performance("performance.registry.deleteobject","s",message.c_str());
				break;
		  case PERFORMANCE_REGISTRY_CREATEOBJECT:
			    Core::Log::GetLog()->Performance("performance.registry.createobject","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_CALL:
			    Core::Log::GetLog()->Performance("performance.object.call","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_ENUMERATE:
			    Core::Log::GetLog()->Performance("performance.object.enumerate","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_COUNT:
			    Core::Log::GetLog()->Performance("performance.object.count","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_METHOD:
			    Core::Log::GetLog()->Performance("performance.object.method","s",message.c_str());
			    break;
		  case PERFORMANCE_OBJECT_RESTORE:
			    Core::Log::GetLog()->Performance("performance.object.restore","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_INSERT:
			    Core::Log::GetLog()->Performance("performance.object.insert","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_UPDATE:
			    Core::Log::GetLog()->Performance("performance.object.update","s",message.c_str());
				break;
		  case PERFORMANCE_OBJECT_DELETE:
			    Core::Log::GetLog()->Performance("performance.object.delete","s",message.c_str());
				break;
		  case PERFORMANCE_SUBSYS_IO:
			    Core::Log::GetLog()->Performance("performance.subsys.io","s",message.c_str());
				break;
		  case PERFORMANCE_SUBSYS_DATA:
			    Core::Log::GetLog()->Performance("performance.subsys.data","s",message.c_str());
				break;
		  case PERFORMANCE_OVERALL:
		  	    {
					char psfBuf[256];
					Core::Log::GetLog()->Performance("performance.overall","s",message.c_str());
					sprintf(psfBuf,"Of which I/O %f",((double)iIO)/1000000.0);
					Core::Log::GetLog()->Performance("performance.overall.totalio","s",psfBuf);
					sprintf(psfBuf,"Of which database %f",((double)iDatabaseTime)/1000000.0);
					Core::Log::GetLog()->Performance("performance.overall.database","s",psfBuf);
					sprintf(psfBuf,"Of which compiler %f",((double)iCompiler)/1000000.0);
					Core::Log::GetLog()->Performance("performance.overall.compiler","s",psfBuf);
		        }
				break;
		}
	}
}
void Meow::Registry::MarkReadonly()
{
	// meow.cnf readonly_whitelist
	if ( !Configuration::GetConfiguration()->IsSet("readonly.whitelist" ) )
	{	
		ReadonlyEvents=new Core::Types::Array();
		ReadonlyEvents->Set("Logout",new Core::Types::Boolean(true));
	}
	else
	{
		Core::Types::String_ptr confLine=new Core::Types::String(Configuration::GetConfiguration()->Get("readonly.whitelist"));
		Core::Types::Array_ptr explodedConfLine=confLine->Explode(",");
		for ( int i=0; i<explodedConfLine->Count(); i++ )
		{
			Core::Types::String_ptr entry=(Core::Types::String_ptr)explodedConfLine->Get(i);
			ReadonlyEvents->Set(entry->GetValueAsString(),new Core::Types::Boolean(true));
		}
	}
	readonlymode=true;
}
bool Meow::Registry::IsReadonly()
{
	return readonlymode;
}
std::string  Meow::Registry::GetNextUniqueId()
{
	// TODO - Implement GetNextUniqueId
	return "";
}

Meow::XML::ObjectParser_ptr  Meow::Registry::GetPattern( std::string classname )
{
	Meow::XML::ObjectParser_ptr definition;
	if ( iModelDirectory.IsNull() )
	{
		Core::Log::GetLog()->Error("Meow::Registry::GetPattern","ssss","Cannot find pattern for ",classname.c_str()," because the model directory is not found");
		Exceptions::NotFound nf;
		nf.SetMessage("Cannot find pattern for " + classname);
		throw nf;
	}
	if ( iModelDirectory->ExistsFile(classname + ".xml") )

	return definition;

}
void Meow::Registry::SetModelDirectory( Core::Storage::Directory_ptr directory )
{
	iModelDirectory=directory;
}

Meow::Core::Storage::Directory_ptr Meow::Registry::GetModelDirectory()
{
	return iModelDirectory;
}
void Meow::Registry::FrameworkMessage( std::string component, 
	std::string event, 
	std::string message )
{
	Session_ptr session=(Session_ptr)GetCurrentSession();
	if ( !session.IsNull() )
	{
		session->PushFrameworkMessage(component,event,message);
	}
	else
	{
		Core::Log::GetLog()->Error("Meow::Registry::FrameworkMessage","ssssss","Could not add framework message to null session. Component:",component.c_str()," Event:",event.c_str()," Message:",message.c_str());
	}
}
Meow::Core::Types::Queue_ptr Meow::Registry::GetFrameworkMessages()
{
    Core::Types::Queue_ptr messages=new Core::Types::Queue();
    Session_ptr session=(Session_ptr)GetCurrentSession();
    if ( !session.IsNull( ) )
    {
        Core::Types::Queue_ptr sessionMessages=session->GetFrameworkMessages();
        if ( !sessionMessages.IsNull() )
            messages=sessionMessages;
    }
        
    return messages;
}
void Meow::Registry::AddClassSubscription( std::string otherclass,  Meow::Factory_ptr myclass, std::string fieldName )
{
	if ( !iClassSubscriptions->IsSet(otherclass) )
		iClassSubscriptions->Set(otherclass,new Core::Types::Array());
	Core::Types::Array_ptr lClassSubscriptions=(Core::Types::Array_ptr)iClassSubscriptions->Get(otherclass);
	FieldSubscription_ptr fieldRecord;
	if ( !lClassSubscriptions->IsSet(myclass->GetMeowClassName()) )
	{	
		fieldRecord=new FieldSubscription();
		fieldRecord->Class=myclass;
	}
	else
	{
		fieldRecord=(FieldSubscription_ptr)lClassSubscriptions->Get(myclass->GetMeowClassName());
	}
	fieldRecord->fields->Set(fieldName,new Core::Types::String(fieldName));
	lClassSubscriptions->Set(myclass->GetMeowClassName(),(Core::ptr)fieldRecord);
	iClassSubscriptions->Set(otherclass,(Core::ptr)lClassSubscriptions);
}
Meow::Core::Types::Array_ptr Meow::Registry::GetClassSubscriptions( std::string myclass )
{
	Core::Types::Array_ptr retval;
	if ( iClassSubscriptions->IsSet(myclass) )
	{
		retval=(Core::Types::Array_ptr)iClassSubscriptions->Get(myclass);
	}
	else
	{
		retval=new Core::Types::Array();
	}
	return retval;
}
void Meow::Registry::RemoveClassSubscription( std::string otherclass, Factory_ptr myclass, std::string fieldname )
{
	if ( iClassSubscriptions->IsSet(otherclass) )
	{
		// Now lets delve into the field subscriptions
		Core::Types::Array_ptr subscriptions=(Core::Types::Array_ptr)iClassSubscriptions->Get(otherclass);
		if ( subscriptions->IsSet(myclass->GetMeowClassName()) )
		{
			FieldSubscription_ptr fieldrecord=(FieldSubscription_ptr)subscriptions->Get(myclass->GetMeowClassName());
			fieldrecord->fields->Unset(fieldname);
		}
	}
}
Meow::FieldSubscription::FieldSubscription()
{
	SetMeowClassName("FieldSubscription");
	fields=new Core::Types::Array();
}
Meow::FieldSubscription::~FieldSubscription()
{
}
void Meow::Registry::RegisterForTransactionLifecycle( LifecycleObject_ptr object )
{
	Core::Log::GetLog()->Transaction("Meow::Registry::RegisterForTransactionLifecycle","sss","Registering ",object->GetMeowClassName().c_str()," for transactions");
	TransactionalObjects->Set(TransactionalObjects->Count(),(Core::ptr)object);
	if ( intransaction )
	{
		Core::Log::GetLog()->Transaction("Meow::Registry::RegisterForTransactionLifecycle","s","In a transaction, so auto-begin.");
		object->BeginTransaction();
	}
}
void Meow::Registry::SyncPoint( )
{
	Core::Log::GetLog()->Transaction("Meow::Registry::SyncPoint","s","SyncPoint - Passivate");
	Passivate();

	Core::Log::GetLog()->Transaction("Meow::Registry::SyncPoint","s","SyncPoint");
	if ( TransactionalObjects->Count() > 0 )
	{
		if ( intransaction )
		{
			Core::Log::GetLog()->Transaction("Meow::Registry::SyncPoint","s","SyncPoint");
			for ( int i=0; i<TransactionalObjects->Count(); i++ )
			{
				LifecycleObject_ptr object=(LifecycleObject_ptr)TransactionalObjects->Get(i);
				Core::Log::GetLog()->Transaction("Meow::Registry::SyncPoint","ss","Committing:",object->GetMeowClassName().c_str());
				object->CommitTransaction();
			}
		}
		for ( int i=0; i<TransactionalObjects->Count(); i++ )
		{
			LifecycleObject_ptr object=(LifecycleObject_ptr)TransactionalObjects->Get(i);
			Core::Log::GetLog()->Transaction("Meow::Registry::SyncPoint","ss","Begin Transaction:",object->GetMeowClassName().c_str());
			object->BeginTransaction();
		}
	}
}
void Meow::Registry::Commit( )
{
	Core::Log::GetLog()->Transaction("Meow::Registry::Commit","s","Commit - Passivate");
	Passivate();
	Core::Log::GetLog()->Transaction("Meow::Registry::Commit","s","Commit");
	if ( TransactionalObjects->Count() > 0 )
	{
		if ( intransaction )
		{			
			for ( int i=0; i<TransactionalObjects->Count(); i++ )
			{
				LifecycleObject_ptr object=(LifecycleObject_ptr)TransactionalObjects->Get(i);
				Core::Log::GetLog()->Transaction("Meow::Registry::Commit","ss","Committing:",object->GetMeowClassName().c_str());
				object->CommitTransaction();
			}
		}
		else
			Core::Log::GetLog()->Transaction("Meow::Registry::Commit","s","Nothing to commit");
	}
	intransaction=false;
}
void Meow::Registry::Rollback( )
{
	Core::Log::GetLog()->Transaction("Meow::Registry::Commit","s","Rollback");
	if ( TransactionalObjects->Count() > 0 )
	{
		if ( intransaction )
		{
			for ( int i=0; i<TransactionalObjects->Count(); i++ )
			{
				LifecycleObject_ptr object=(LifecycleObject_ptr)TransactionalObjects->Get(i);
				Core::Log::GetLog()->Transaction("Meow::Registry::Rollback","ss","Rolling Back:",object->GetMeowClassName().c_str());
				object->RollbackTransaction();
			}
		}
	}
	intransaction=false;
}
void Meow::Registry::Begin( )
{	
	Core::Log::GetLog()->Transaction("Meow::Registry::Commit","s","Begin");
	if ( !intransaction )
	{
		if ( TransactionalObjects->Count() > 0 )
		{
			for ( int i=0; i<TransactionalObjects->Count(); i++ )
			{
				LifecycleObject_ptr object=(LifecycleObject_ptr)TransactionalObjects->Get(i);
				Core::Log::GetLog()->Transaction("Meow::Registry::Begin","ss","Begin Transaction:",object->GetMeowClassName().c_str());
				object->BeginTransaction();
			}
		}
		intransaction=true;
	}
}

