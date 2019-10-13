/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _REGISTRY_H_
#define _REGISTRY_H_

#include <LifecycleObject.h>
#include <Core/Types/Array.h>
#include <Core/Types/Stack.h>
#include <Core/Types/Queue.h>
#include <Core/System/LinkLibrary.h>
#include <Factory.h>
#include <Core/NameValueStore.h>
#include <Core/Types/Constraint.h>
#include <Core/Tools/XMLParser.h>
#include <XML/ObjectParser.h>
#include <Core/Storage/Directory.h>

#define PERFORMANCE_REGISTRY_FINDFACTORY  8     // performance.registry.findfactory
#define PERFORMANCE_REGISTRY_FINDOBJECT   9     // performance.registry.findobject
#define PERFORMANCE_REGISTRY_DELETEOBJECT 10    // performance.registry.deleteobject
#define PERFORMANCE_REGISTRY_CREATEOBJECT 11    // performance.registry.createobject
#define PERFORMANCE_OBJECT_CALL           12    // performance.object.call
#define PERFORMANCE_OBJECT_ENUMERATE      13    // performance.object.enumerate
#define PERFORMANCE_OBJECT_COUNT          14    // performance.object.count
#define PERFORMANCE_OBJECT_METHOD         15    // performance.object.method
#define PERFORMANCE_OBJECT_RESTORE        16    // performance.object.restore
#define PERFORMANCE_OBJECT_INSERT         17    // performance.object.insert
#define PERFORMANCE_OBJECT_UPDATE         18    // performance.object.update
#define PERFORMANCE_OBJECT_DELETE         19    // performance.object.delete
#define PERFORMANCE_SUBSYS_IO             20    // performance.subsys.io
#define PERFORMANCE_SUBSYS_DATA           21    // performance.subsys.data
#define PERFORMANCE_OVERALL               22    // performance.overall

namespace Meow {

	namespace Security {
		class User;
		typedef Reference<User> User_ptr;
	}

#define MEOW_PERFORMANCE_ENTRY(timervarT,eyecatcherT,codeT,parametersT) \
\
        Meow::PerformanceEntry_ptr timervarT=new Meow::PerformanceEntry(); \
        timervarT->StartTimer(); \
        timervarT->recordType=codeT; \
        timervarT->eyecatcher=eyecatcherT; \
        timervarT->parameters=parametersT;


#define MEOW_PERFORMANCE_EXIT(timervarT) \
\
        timervarT->EndTimer(); \
        timervarT->Test();


	class PerformanceEntry;
	typedef Reference<PerformanceEntry> PerformanceEntry_ptr;
	class PerformanceEntry : public Core::Object
	{
	public:
		PerformanceEntry();
		virtual ~PerformanceEntry();

		short recordType;
		std::string eyecatcher;
		Core::Types::Array_ptr parameters;
		unsigned long long start;
		unsigned long long end;

		void StartTimer();
		void EndTimer();
		void Test();
		std::string ToString();
	};

	class EventManifestLoader;
	typedef Reference<EventManifestLoader> EventManifestLoader_ptr;
	class EventManifestLoader : public Core::Tools::XMLParser
	{
	public:
		EventManifestLoader();
		~EventManifestLoader();
		virtual void StartElement( string element, Core::Types::Array_ptr attributes );
		virtual void EndElement( string element );
		virtual void CharacterData( string data );
		void OnEvent( std::string eventName );
	private:
		Core::Types::Array_ptr iEvents;
	};

	class RegistryEntry;
	typedef Reference<RegistryEntry> RegistryEntry_ptr;

	class LibraryEntry;
	typedef Reference<LibraryEntry> LibraryEntry_ptr;

	class EventEntry;
	typedef Reference<EventEntry> EventEntry_ptr;

	class RegistryEntry : public Object
	{
	public:
		RegistryEntry();
		~RegistryEntry();

		Factory_ptr Instance;
		unsigned long Accesses;
		std::string CollectionID;
		unsigned long ObjectsHeld;
		LibraryEntry_ptr BackwardsReference;
	};

	class LibraryEntry : public Object
	{
	public:
		LibraryEntry();
		~LibraryEntry();

		Meow::Core::System::LinkLibrary_ptr Library;
		unsigned long References;
	};

	class EventEntry : public Object
	{
	public:
		EventEntry();
		~EventEntry();
		Factory_ptr Class;
		Core::Types::Array_ptr Predicates;
	};

	class DelayedEventEntry;
	typedef Reference<DelayedEventEntry> DelayedEventEntry_ptr;

	class DelayedEventEntry : public Object
	{
	public:
		DelayedEventEntry();
		~DelayedEventEntry();
		std::string event;
		Core::Types::Array_ptr parameters;
	};

	class FieldSubscription;
	typedef Reference<FieldSubscription> FieldSubscription_ptr;

	class FieldSubscription : public Object
	{
	public:
		FieldSubscription();
		~FieldSubscription();
		Factory_ptr Class;
		Core::Types::Array_ptr fields;
	};

	class Registry;
	typedef Reference<Registry> Registry_ptr;


	class Registry : public Object
	{
	private:
		Registry();

	public:
		virtual ~Registry();

		Core::NameValueStore_ptr  GetNameValueStore();
		static Registry_ptr GetRegistry();
		static Core::Types::Array_ptr InterpretKey( std::string key );
		static std::string  CompileKey( Core::Types::Array_ptr key );
		static Core::Types::Array_ptr  DeCompileKey( std::string key );
		void   __Destroy( );
		void   SetController( Object_ptr value );
		void   RegisterMe( Factory_ptr factory );
		void   InstallExtensions( );
		Core::Types::Stack_ptr     EnumerateRegistered();
		RegistryEntry_ptr   Details( std::string factoryname );
		Factory_ptr         FindFactory( std::string factoryname );

		LifecycleObject_ptr FindMeByObjectReference( std::string reference );
		LifecycleObject_ptr FindMe( std::string classname, Core::Types::Array_ptr key );
		LifecycleObject_ptr FindMe( std::string classname, char * key[] );
		LifecycleObject_ptr FindMe( std::string classname, const char * key[] );
		LifecycleObject_ptr FindMe( std::string classname, std::string key );
		LifecycleObject_ptr FindMe( std::string classname, int argcount, ... );
		LifecycleObject_ptr FindMe( std::string classname );
		LifecycleObject_ptr CreateMe( std::string classname, Core::Types::Array_ptr key );
		LifecycleObject_ptr CreateMe( std::string classname, char * key[] );
		LifecycleObject_ptr CreateMe( std::string classname, const char * key[] );
		LifecycleObject_ptr CreateMe( std::string classname, std::string key );
		LifecycleObject_ptr CreateMe( std::string classname, int argcount, ... );
		LifecycleObject_ptr CreateMe( std::string classname );
		LifecycleObject_ptr GetMe( std::string classname, Core::Types::Array_ptr key );
		LifecycleObject_ptr GetMe( std::string classname, char * key[] );
		LifecycleObject_ptr GetMe( std::string classname, const char * key[] );
		LifecycleObject_ptr GetMe( std::string classname, std::string key );
		LifecycleObject_ptr GetMe( std::string classname, int argcount, ... );
		LifecycleObject_ptr GetMe( std::string classname );

		void DestroyMe( std::string classname, Core::Types::Array_ptr key );
		void DestroyMe( std::string classname, char * key[] );
		void DestroyMe( std::string classname, const char * key[] );
		void DestroyMe( std::string classname, std::string );
		void DestroyMe( std::string classname, int argcount, ... );
		void DisposeOf( std::string factoryname );
		void Purge();
		void Passivate();
		void Cleanup();
		void Flush( std::string factoryname );
		void GarbageCollect( );
		bool IsEventPermitted( std::string eventname );
		void PostEvent( std::string eventname, Core::Types::Array_ptr arguments );
		void PostEvent( std::string eventname, int argcount, ... );
		void Subscribe( std::string eventname, Factory_ptr classref, Core::Types::Array_ptr predicates );
		Core::Types::Constraint_ptr      GetConstraint( std::string regexp );
		Object_ptr GetCurrentSession();
		void SetCurrentSession( Object_ptr session );
		bool GetUserChanged();
		void SetUserChanged(bool val);
		Core::NameValueStore_ptr GetHeap();
		bool IsSessionReady();
		void SessionReady();
		bool IsInBatch( );
		void CalledByBrowser( );
		void QueueDelayedEvent( std::string event, Core::Types::Array_ptr parameters );
		void FireDelayedEvents( );
		void RegisterPerformance( PerformanceEntry_ptr perf );
		void RecordPerformanceProfile();
		void MarkReadonly();
		bool IsReadonly();
		static std::string  GetNextUniqueId();
		XML::ObjectParser_ptr  GetPattern( std::string classname );
		void SetModelDirectory( Core::Storage::Directory_ptr directory );
		Core::Storage::Directory_ptr GetModelDirectory();
		Core::Types::Stack_ptr GetFactoryList();
		void FrameworkMessage( std::string component, 
			std::string event, 
			std::string message );
        Core::Types::Queue_ptr GetFrameworkMessages();
		// If when adding attributes to a class we mention in "additionalconstraints" the
		// other class name, tell that other class (first parameter ) that we refer to it
		// so that it can use introspection to discover our relationships. We do this
		// in a "soft" way through the registry because that other class might not have
		// been installed at the point we define the attribute (and may never be).
		void AddClassSubscription( std::string otherclass, Factory_ptr myclass, std::string fieldName );
		// Get the subscription list for my class, note this will be an array of FieldSubscriptions
		Core::Types::Array_ptr GetClassSubscriptions( std::string myclass );
		// If we remove an attribute, we need to remove this reference.
		void RemoveClassSubscription( std::string otherclass, Factory_ptr myclass, std::string fieldName );
		
		// Improve explicit session handling semantics to prevent a session object being created before we want one
		void BlockSessions();
		void ReleaseSessions();

		void RegisterForTransactionLifecycle( LifecycleObject_ptr object );
		void SyncPoint( );
		void Commit( );
		void Rollback( );
		void Begin( );
	protected:
		void InternalRegisterWithLibrary( Factory_ptr factory, LibraryEntry_ptr library );
		void PostEventDirect( std::string eventname, Core::Types::Array_ptr arguments );

		Core::Types::Array_ptr TransactionalObjects;
		Core::Types::Array_ptr Manifests;
		Core::Types::Array_ptr Libraries;
		Core::Types::Array_ptr ServiceIndex;
		Core::Types::Array_ptr LookupService;
		Core::Types::Array_ptr PermittedEvents;
		Core::Types::Array_ptr DeniedEvents;
		Core::Types::Array_ptr NotificationRegistry;
		Core::Types::Array_ptr Constraints;
		Object_ptr Controller;
		Object_ptr iSession;
		Core::NameValueStore_ptr iNameValueStore;
		static Registry_ptr gRegistry;
		bool sessionready;
		bool batchmode;
		bool readonlymode;
		Core::Types::Array_ptr ReadonlyEvents;
		Core::Types::Stack_ptr DelayedEvents;
		bool userChanged;
		Core::Types::Queue_ptr PerformanceMetrics;
		Core::Types::Array_ptr iPatterns;
		Core::Storage::Directory_ptr iModelDirectory;
		Core::Types::Array_ptr iClassSubscriptions;


		unsigned long iPERFORMANCE_REGISTRY_FINDFACTORY;
		unsigned long iPERFORMANCE_REGISTRY_FINDOBJECT;
		unsigned long iPERFORMANCE_REGISTRY_DELETEOBJECT;
		unsigned long iPERFORMANCE_REGISTRY_CREATEOBJECT;
		unsigned long iPERFORMANCE_OBJECT_CALL;
		unsigned long iPERFORMANCE_OBJECT_ENUMERATE;
		unsigned long iPERFORMANCE_OBJECT_COUNT;
		unsigned long iPERFORMANCE_OBJECT_METHOD;
		unsigned long iPERFORMANCE_OBJECT_RESTORE;
		unsigned long iPERFORMANCE_OBJECT_INSERT;
		unsigned long iPERFORMANCE_OBJECT_UPDATE;
		unsigned long iPERFORMANCE_OBJECT_DELETE;
		unsigned long iPERFORMANCE_SUBSYS_IO;
		unsigned long iPERFORMANCE_SUBSYS_DATA;
		unsigned long iPERFORMANCE_OVERALL;
		unsigned long long iIO;
		unsigned long long iDatabaseTime;
		unsigned long long iCompiler;
		friend class PerformanceEntry;
		bool forbidsessions;
		bool intransaction;
	}; // End of Class Registry
} // End of NameSpace Meow
#endif
