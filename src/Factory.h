/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _FACTORY_H_
#define _FACTORY_H_
#include <LifecycleObject.h>
#include <Core/Types/Array.h>
#include <Core/Types/Queue.h>
#include <Exceptions/NotFound.h>
#include <Exceptions/Duplicate.h>
#include <Exceptions/BadParameter.h>
#include <stdarg.h>
#include <Core/Tools/XMLParser.h>
#include <Core/Types/Constraint.h>

namespace Meow {
	class Registry;
	typedef Reference<Registry> Registry_ptr;

	class Factory;
	typedef Reference<Factory> Factory_ptr;

	class UncollectedFactory;
	typedef Reference<UncollectedFactory> UncollectedFactory_ptr;

	class CollectionParser;
	typedef Reference<CollectionParser> CollectionParser_ptr;

	class SchemaField;
	typedef Reference<SchemaField> SchemaField_ptr;

	class JoinSpec;
	typedef Reference<JoinSpec> JoinSpec_ptr;

	class Linkage;
	typedef Reference<Linkage> Linkage_ptr;
	
	class JoinSpec : public Object
	{
	public:
		JoinSpec();
		~JoinSpec();
		std::string iOtherClass;
		Core::Types::Array_ptr iLinkList;
	};

	class Linkage : public Object
	{
	public:
		Linkage();
		~Linkage();

		SchemaField_ptr iLeftField;
		typedef enum eOperation { LessThan, MoreThan, Equals, NotEqualTo, LessThanOrEqualTo, MoreThanOrEqualTo } eOperation;
		typedef enum eTargetType { Field, Value } eTargetType;
		eOperation Operation;
		eTargetType TargetType;
		Core::ptr iRightField; 
	};


	class CollectionParser : public Meow::Core::Tools::XMLParser
	{
	public:
		CollectionParser();
		~CollectionParser();

		Core::Types::Array_ptr GetCollection();

		virtual void StartElement( string element, Core::Types::Array_ptr attributes );
		virtual void EndElement( string element );
		virtual void CharacterData( string data );
		std::string GetMeowClassName();
	private:
		Core::Types::Array_ptr collection;
		int depth;
		std::string classname;
		std::string attributedata;
		std::string characterdata;
	};

	class InstanceParser : public Core::Tools::XMLParser
	{
	public:
		InstanceParser();
		~InstanceParser();

		Core::Types::Array_ptr GetProperties();

		virtual void StartElement( string element, Core::Types::Array_ptr attributes );
		virtual void EndElement( string element );
		virtual void CharacterData( string data );

	private:
		Core::Types::Array_ptr properties;
		int depth;
		std::string attributedata;
		std::string characterdata;
	};



	class Factory : public Object
	{
	public:
		Factory();
		~Factory();

		bool GetSerial();
		bool GetKeySet();
		Core::Types::Array_ptr GetSchemeKeys();
		Core::Types::Array_ptr GetSchemeAttributes();
		virtual std::string GetAliasFormat();

		typedef enum SchemaFieldType {
				Serial,
				Key,
				Attribute
		} SchemaFieldType;

		typedef enum SchemaColumnType {
				Object,
				String,
				Integer,
				Float,
				Boolean,
				Date,
				Array,
				Queue,
				Stack,
				Void,
				ValueType,
				Enumeration,
				Password,
				Text,
				HTML,
				XML,
				Image,
				File
		} SchemaColumnType;
		
		typedef enum SchemeSecurityType {
				Plain,
				UserPublicKey,
				ApplicationPublicKey
		} SchemaSecurityType;

		Core::ptr AttributeConversion( std::string fieldName,  Core::Types::ValueType_ptr value ) ;
	    bool AttributeValidation( std::string fieldName, Core::ptr value ); 
	    std::string AttributeFullValidation(std::string fieldName, Core::ptr value);
		// Schema operations
		// If you wish to use the generalised storage scheme then you can just
		// set the methods on the factory during the constructor

		// ShowInLists
		// --------------------------------------
		// By default nothing is shown in a list view, unless you set the attribute to be shown in lists
		// with this override.
		void ShowInLists( std::string fieldname );

		// LoadShowInListsDefault
		// --------------------------------------
		// By default we do nothing with these, unless this method is called. In
		// PersistentFactory this is loaded up before an Enumeration. This
		// first loads from Service/Props factory.[GetMeowClassName()].ShowInLists
		// and iterates around the array.
		// Then, loads from Service/UserProps factory [GetMeowClassName()].ShowInLists for
		// the current user, if logged in. But it does this once per session
		void LoadShowInListsDefault();
		
		// GetShowInListSettings
		// --------------------------------------
		// Returns a name/value pair sequence of what's currently set, for use with a user
		// interface.
		Core::Types::Array_ptr GetShowInListsSettings();

		// SaveShowInListSettingsForAll
		// --------------------------------------
		// Updates the settings
		void SaveShowInListSettingsForAll( Core::Types::Array_ptr settings );
		
		// SaveShowInListSettingsForCurrentUser
		// --------------------------------------
		// Updates the settings
		void SaveShowInListSettingsForCurrentUser( Core::Types::Array_ptr settings );

		// AddSerialKeyField
		// ------------------------------
		// Add a single attribute that is set on construction with an empty
		// "Create" operation. If there are any
		// other key fields it will throw a BadParameter exception, there can only
		// be one key serial field. By definition it is of type Integer
		void AddSerialKeyField( std::string fieldname, 
								std::string publicName, 
								std::string title, 
								bool publicSearch=false,
								std::string description="" );

		// AddCompoundKeyField
		// ------------------------------
		// Add a compound key field, if there is already a serial key variable then
		// throw an exception. Specify the type, and whether the field is indexible.
		void AddCompoundKeyField( std::string fieldname, 
								SchemaColumnType type, 
								bool indexible, 
								std::string publicName, 
								std::string title, 
								bool publicSearch=false,
								std::string additionalConstraints="", // If the type is an Object, specify the classname here i.e. Security/User
								std::string description="" 
								) ;

		// AddAttributeField
		// ------------------------------
		// Add an attribute field. These are not keys and are not indexible [quick searchable]
		void AddAttributeField( std::string fieldname,	// Private name of field
						SchemaColumnType type, // Type of field
						Core::ptr defaultValue, // Default value, should match type, if Factory::Enumeration use a Core::Types::String 
						int version, // Increment this if you modify an object and add an attribute field
						std::string publicName, // Public attribute name - case sensitive
						std::string securityGroupRead, // Public security group for being able read/see
						std::string securityGroupWrite, // Public security group for being able to modify
						std::string title, // The title of the attribute to appear at the head of columns
						bool publicSearch=false, // Whether it responds to a general keyword search
						Core::Types::Constraint_ptr constraint=(Core::Types::Constraint*)NULL, // Additional constraints or NULL
						std::string additionalContraints="",   // If its Factory::Enumeration, the values as options like this "a|b|c",
															  // note empty values should be like this "|b|c|d", yes, its a cheap regular 
															  // expression but note we "explode" based on | so get it right ;). 
															  // If type is an Object specify the classname here i.e. Security/User
															  //
						Factory::SchemaSecurityType securityType=Factory::Plain,
						std::string description="",
						std::string section=""
// ,
//						securityConfiguration=""
						);

		// DeleteAttributeField
		// ------------------------------
		// When you modify a class you should make sure that if you are dropping an attribute that it is
		// declared here, the final attribute list will be the sum of the attributes at the current version
		// but this allows the automatic storage engine to know what to do to adhere to final version if
		// it needs to upgrade.
		void DeleteAttributeField( std::string fieldname, int version );

		
		// MakePrivateField
		// ------------------------------
		// If you make a field private it indicates to a visual component it shouldn't be rendered, but it can still be 
		// used by business logic.
		void MakePrivateField( std::string fieldname );
		
		// On[Create|Update|Delete] methods
		// --------------------------------
		// Allows you to auto get a new event posted if one of these happens to your class [if you inherit from an object pattern that supports it]
		// OnCreate(eventname) -> Posts an event AFTER creation (last thing it does) with the current name value pairs from the object. 
		//				Current : Core/Types/Array
		// OnUpdate(eventname) -> Posts an event AFTER update (last thing it does) with the previous name value pairs from the object and the current name
		//				value pairs from the object
		//				Previous: Core/Types/Array
		//				Current : Core/Types/Array
		// OnDelete(eventname) -> Posts an event AFTER delete (last thing it does) with the previous name value pairs from the object.
		//				Previous: Core/Types/Array
		void OnCreate( std::string eventName );
		void OnUpdate( std::string eventName );
		void OnDelete( std::string eventName );
		
		// Because we can't intercept updates from inside the factory, the implementor of passivate must call PostUpdateEvent
		// meh.
		void PostUpdateEvents( Core::Types::Array_ptr before, Core::Types::Array_ptr current );
		
		SchemaField_ptr GetAttributeByAlias( std::string );
		SchemaField_ptr GetKeyAttributeByAlias( std::string );

		// Upgrade
		virtual bool Upgrade( int currentVersion );

		// GetChanges
		// ------------------------------
		// Work out the change list to make to the storage area.
		Core::Types::Queue_ptr GetChanges( int currentVersion );

		// GetAttributes
		// ------------------------------
		// For the current security user, work out the attribute list.
		Core::Types::Array_ptr GetAttributes( );

		// QuietCan*
		// ------------------------------
		// For the current security user, work out the ability to interact with
		// this class.
		bool QuietCanWrite();
		bool QuietCanRead();
		bool QuietCanCreate();
		bool QuietCanDelete();


		void InitialiseSecurity();


		Registry_ptr GetRegistry( );
		virtual void OnEvent( std::string eventname, Core::Types::Array_ptr parameters );
		virtual void __Destroy( );
		virtual Core::Types::Array_ptr GetServices( );
		virtual void Purge( );
		virtual void Passivate( );
		virtual void Flush( );
		virtual void GarbageCollect( );
		virtual unsigned long CountObjectsHeld( );
		virtual bool KeyCheck( Core::Types::Array_ptr key );
		virtual void RegisterEvents( );

		virtual LifecycleObject_ptr GetInstance( std::string service ) ;
		virtual LifecycleObject_ptr FindByPrimaryKey( int argcount, ... );
		virtual LifecycleObject_ptr FindByPrimaryKey( std::string );
		virtual LifecycleObject_ptr FindByPrimaryKey( Core::Types::Array_ptr key );
		virtual LifecycleObject_ptr FindByPrimaryKeyOrRestore( Core::Types::Array_ptr key, Core::Types::Array_ptr attributes );
		virtual LifecycleObject_ptr FindByPrimaryKey( char * key[] );
		virtual LifecycleObject_ptr CreateByPrimaryKey( std::string service, int argcount, ... );
		virtual LifecycleObject_ptr CreateByPrimaryKey( std::string key, std::string service );
		virtual LifecycleObject_ptr CreateByPrimaryKey( Core::Types::Array_ptr key, std::string service );
		virtual LifecycleObject_ptr CreateByPrimaryKey( char * key[], std::string service );
		virtual void DestroyByPrimaryKey( int argcount, ... );
		virtual void DestroyByPrimaryKey( std::string key );
		virtual void DestroyByPrimaryKey( Core::Types::Array_ptr key );
		virtual void DestroyByPrimaryKey( char * key[] );
		virtual void MarkChangedExternally( Core::Types::Array_ptr key );
		Core::Types::Array_ptr GetAnonymousKey( );
		std::string CompoundKeyHashing( Core::Types::Array_ptr key );
		Meow::Core::Types::Array_ptr DeCompileKey( std::string compiled_key );
		std::string GetClassNameFromKey( std::string compiled_key );


		// This returns an instance of Security/Group, but because we need to limit forward declarations
		// due to Visual Studio this is now an Object_ptr
		Object_ptr GetMinimumSecurity();
		Object_ptr GetFindSecurity( );
		Object_ptr GetCreateSecurity( );
		Object_ptr GetDeleteSecurity( );
		Object_ptr GetUpdateSecurity( );

		virtual bool CanJoin( std::string relationshipname );
		virtual JoinSpec_ptr GetJoinSpec( std::string relationshipname );
		void AddJoin( std::string relationshipname, JoinSpec_ptr joins );
		virtual Core::Types::Stack_ptr GetJoinList( );
		virtual void FromXML( std::string xmldocument );
		virtual Meow::LifecycleObject_ptr GetFromXML( std::string service, std::string xmldocument );
		virtual void SetVariable( std::string varname, Core::ptr value );
		virtual Core::ptr GetVariable( std::string varname );
		virtual bool ExistsVariable( std::string varname );
		Core::Types::Stack_ptr GetVariableKeys();
		virtual bool SecondPhasePurge();

		Core::Types::Array_ptr GetSearchableParts( );
		virtual Core::Types::Array_ptr Search( std::string, unsigned long offset, unsigned long limit, std::string service, Core::Types::Array_ptr constraints=(Core::Types::Array*)NULL ); 
		virtual unsigned long CountSearch( std::string, std::string,Core::Types::Array_ptr constraints=(Core::Types::Array*)NULL );
		int  GetVersion();

		virtual void AddCallableMethod( 
				std::string method, 
				Core::Types::Array_ptr parameters,
				std::string label=""
		);
		Core::Types::Array_ptr GetCallableMethodList();

		virtual void AddCallableFactoryMethod( 
				std::string method, 
				Core::Types::Array_ptr parameters,
				std::string label="" 
		);
		Core::Types::Array_ptr GetCallableFactoryMethodList();
		virtual std::string GetFriendlyName();
		virtual bool IsPublicApplication();
		virtual void SetOrderByColumn ( std::string columnname );
		virtual void DescendingOrder();
		virtual void AscendingOrder();
		virtual bool GetDescending();
		virtual std::string GetOrderByColumn ( );

		virtual std::string GetInterface();
	protected:
		void SetMinimumSecurity( Object_ptr sec );
		void SetFindSecurity( Object_ptr sec );
		void SetCreateSecurity( Object_ptr sec );
		void SetDeleteSecurity( Object_ptr sec );
		void SetUpdateSecurity( Object_ptr sec );
		void AddInstanceToContainer( LifecycleObject_ptr object );
		void RemoveKeyFromContainer( Core::Types::Array_ptr key );
		bool IsKeyInContainer( Core::Types::Array_ptr key );
		LifecycleObject_ptr FindInstanceInContainer( Core::Types::Array_ptr key );
		void SetFactoryOnObject( LifecycleObject_ptr object );
		void PassivateChildren( Core::Types::Array_ptr children );
		void PurgeChildren( Core::Types::Array_ptr children );
		void FlushChildren( Core::Types::Array_ptr children );
		void GarbageCollectionChildren( Core::Types::Array_ptr children );

		virtual void CanWrite();
		virtual void CanRead();
		virtual void CanCreate();
		virtual void CanDelete();

	private:
		friend class Registry;
		friend class LifecycleObject;
		Core::Types::Array_ptr ldContainer;
		Core::Types::Array_ptr Services;
		Core::Types::Array_ptr Joins;
		Registry_ptr iRegistry;
		Object_ptr iMinimumSecurity;
		Object_ptr iFindSecurity;
		Object_ptr iCreateSecurity;
		Object_ptr iDeleteSecurity;
		Object_ptr iUpdateSecurity;
		unsigned long objectsHeld;
		Core::Types::Array_ptr iHashMap;
		Core::Types::Array_ptr iSchemeKeys;
		Core::Types::Array_ptr iSchemeAttributes;
		Core::Types::Array_ptr iSchemeAttributesByAlias;
		Core::Types::Array_ptr iSchemeKeysByAlias;
		bool iSerial;
		bool iKeySet;
		Core::Types::Queue_ptr iAttributeQueue;
		int iVersion;
		Core::Types::Array_ptr iSearchableParts;
		Core::Types::Array_ptr iCallableMethodList;
		Core::Types::Array_ptr iCallableFactoryMethodList;
		std::string iDefaultOrderBy;
		bool iDescendingOrder;
		Core::Types::Array_ptr iShowInLists;
		Core::Types::Array_ptr iOnCreateEvents;
		Core::Types::Array_ptr iOnDeleteEvents;
		Core::Types::Array_ptr iOnUpdateEvents;
	}; // End of Class Factory

	class SchemaField : public Object
	{
	public:
		SchemaField();
		~SchemaField();
		Factory::SchemaFieldType iFieldType;
		Factory::SchemaColumnType iColumnType;
		std::string iFieldName;
		bool iIndex;
		bool iSerial;
		Core::ptr iDefaultValue;
		int iVersion;
		std::string iPublicName;
		std::string iSecurityGroupRead;
		std::string iSecurityGroupWrite;
		std::string iTitle;
		int iOffset;
		Core::Types::Constraint_ptr iConstraint;
		std::string iEnumerationValues;
		bool iChanged;
		bool iShowInList;
		Factory::SchemaSecurityType iSecurityType;
//		std::string iSecurityConfiguration;
		std::string iDescription;
		std::string iSection;
		bool iPrivate;
	};

	class Method;
	typedef Reference<Method> Method_ptr;
	class Method : public Object
	{
	public:
		Method();
		~Method();
		std::string iMethodName;
		std::string iButtonLabel;
		Core::Types::Array_ptr iParameters;
	};

	class SchemaOperation;
	typedef Reference<SchemaOperation> SchemaOperation_ptr;
	class SchemaOperation : public Object
	{
	public:
		SchemaOperation();
		~SchemaOperation();
		int iVersion;
		int iOperation;
		SchemaField_ptr iDefinition;
	};
} // End of NameSpace Meow
#endif
