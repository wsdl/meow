#include <PersistenceFactory.h>
#include <Persistence/Select.h>
#include <Persistence/LazyEnumerator.h>
#include <Persistence/Delete.h>
#include <Persistence/Update.h>
#include <Persistence/Insert.h>
#include <Persistence/Where.h>
#include <Persistence/Database.h>
#include <inc/configuration.h>

Meow::PersistenceFactory::PersistenceFactory()
{
	SetMeowClassName("PersistenceFactory");
	iPersistenceDescriptor=NULL;

}
Meow::PersistenceFactory::~PersistenceFactory()
{

}

bool Meow::PersistenceFactory::Upgrade( int currentVersion )
{
	std::string myClass=GetMeowClassName().substr(0,GetMeowClassName().length()-7);
	Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(myClass);
	bool retval=true;

	Core::Types::Queue_ptr changeList=GetChanges(currentVersion);

	Core::Types::Array_ptr creationSequence=new Core::Types::Array();
	while ( !changeList->IsEmpty() )
	{
		SchemaOperation_ptr operation=(SchemaOperation_ptr)changeList->Pull();
		if ( operation->iVersion == 0 )
		{
			creationSequence->Set(creationSequence->Count(),(Core::ptr)operation);
		}
		else
		{
			changeList->Push((Core::ptr)operation);
			break;
		}
	}
	if ( creationSequence->Count() > 0 )
		database->Create(myClass,creationSequence);
	while ( !changeList->IsEmpty() )
	{
		SchemaOperation_ptr operation=(SchemaOperation_ptr)changeList->Pull();
		database->Action(myClass,operation);
	}
	return retval;
}

Meow::Object_ptr Meow::PersistenceFactory::GetPersistenceDescriptor( std::string service ) throw (Exceptions::Rollback)
{
	if ( iPersistenceDescriptor.IsNull() )
	{
		try
		{
			// Find out through precedence which datastore we should be using
			std::string dataStoreName="meow";
			if ( Configuration::GetConfiguration()->IsSet("datastore") )
				dataStoreName=Configuration::GetConfiguration()->Get("datastore");
			if ( Configuration::GetConfiguration()->IsSet("datastore." + service ))
				dataStoreName=Configuration::GetConfiguration()->Get("datastore." + service );
			// Now try and get the database
			Persistence::Database_ptr descriptor=(Persistence::Database_ptr)GetRegistry()->GetMe("Persistence/Database",1,dataStoreName.c_str());
			descriptor->Initialise();
			iPersistenceDescriptor=descriptor;
		} catch ( Exceptions::NotFound &nf )
		{
			Exceptions::Rollback rb;
			rb.SetMessage(nf.Diagnostic());
			rb.SetMessage("Could not get a persistence descriptor for this class [" + service + "] therefore Rollback will be thrown.");
			throw rb;
		}
	}
	return iPersistenceDescriptor;
}
unsigned long Meow::PersistenceFactory::CountMatches( std::string service, std::string fieldName, Core::ptr value )
{
	unsigned long retval=0;
	// Convert the field name to one we can use with a filter
	SchemaField_ptr field=GetAttributeByAlias(fieldName);
	if ( field.IsNull() )
		field=GetKeyAttributeByAlias(fieldName);
	if ( !field.IsNull() )
	{	
		Persistence::Where_ptr where=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where",new Core::Types::Array());
		std::string stringifiedValue="";
		if ( !value.IsNull() )
		{
			if ( value->IsInstanceOf("Object") )
				stringifiedValue=((Object_ptr)value)->ToString();
			else if ( value->IsInstanceOf("ValueType") )
				stringifiedValue=((Core::Types::ValueType_ptr)value)->GetValueAsString();
		}
		where->Expression(field->iFieldName,"=",stringifiedValue);
		where->LogicalAnd();
		where->Expression("__available","=","1");
		retval=CountEnumerateWithFilter(service,(Filter_ptr)where);
	}
	return retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::Matches( std::string service, std::string fieldName, Core::ptr value )
{
	Core::Types::Array_ptr retval=new Core::Types::Array();
	// Convert the field name to one we can use with a filter
	SchemaField_ptr field=GetAttributeByAlias(fieldName);
	if ( field.IsNull() )
		field=GetKeyAttributeByAlias(fieldName);
	if ( !field.IsNull() )
	{	
		Persistence::Where_ptr where=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where",new Core::Types::Array());
		std::string stringifiedValue="";
		if ( !value.IsNull() )
		{
			if ( value->IsInstanceOf("Object") )
				stringifiedValue=((Object_ptr)value)->ToString();
			else if ( value->IsInstanceOf("ValueType") )
				stringifiedValue=((Core::Types::ValueType_ptr)value)->GetValueAsString();
		}
		where->Expression(field->iFieldName,"=",stringifiedValue);
		where->LogicalAnd();
		where->Expression("__available","=","1");
		retval=EnumerateWithFilter(service,(Filter_ptr)where);
	}
	return retval;
}

unsigned long Meow::PersistenceFactory::CountEnumerate( std::string service ) throw
				(Meow::Exceptions::Security,Exceptions::Rollback)
{
	unsigned long retval=0;
	CanRead();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		Persistence::Where_ptr where=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where");
	    
	//	where->Expression("deleted","=","0");
	//	where->LogicalAnd();
		where->Expression("__available","=","1");
		where->LogicalAnd();
		where->Expression("__available","!=","0");
		select->SetWhere(where);
//		select->SetFrom(database->SanitizeTableName(service));
		select->SetFrom(service);
		select->AddColumn("COUNT(*)","hits");
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		Core::Types::ValueType_ptr count=result->FetchCellDataAsValue(0,"hits");
		retval=count->GetValueAsInteger();
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}

	return retval;
}
unsigned long Meow::PersistenceFactory::CountEnumerateWithFilter( std::string service, Filter_ptr filter ) throw
				(Meow::Exceptions::Security,Exceptions::Rollback)
{
	Core::Log::GetLog()->Warning("Meow::PersistenceFactory::CountEnumerateWithFilter","s",service.c_str());
	unsigned long retval=0;
	CanRead();
	if ( ((Persistence::Where_ptr)filter)->GetClauses()->Count()>0 )
	{
		((Persistence::Where_ptr)filter)->Ket();	
		((Persistence::Where_ptr)filter)->LogicalAnd();
	}
	// ((Persistence::Where_ptr)filter)->Expression("deleted","=","0");
	// ((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","=","1");


	((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","!=","0");
		
	Core::Log::GetLog()->Warning("Meow::PersistenceFactory::CountEnumerateWithFilter","ss",service.c_str()," finished logical and.");
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		select->SetFrom(database->SanitizeTableName(service));
		select->AddColumn("COUNT(*)","hits");
		select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		Core::Types::ValueType_ptr count=result->FetchCellDataAsValue(0,"hits");
		retval=count->GetValueAsInteger();
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::Enumerate( std::string service ) throw (Exceptions::Security,Exceptions::Rollback)
{
	CanRead();
	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		Persistence::Where_ptr where=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where");
		// where->Expression("deleted","=","0");
		// where->LogicalAnd();
		where->Expression("__available","=","1");
		where->LogicalAnd();
		where->Expression("__available","!=","0");
	

		select->SetWhere(where);
		select->SetFrom(database->SanitizeTableName(service));
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		// select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::EnumerateWithLimit( std::string service, int offset, int limit ) throw (Exceptions::Security,Exceptions::Rollback)
{
	CanRead();
	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		Persistence::Where_ptr where=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where");
		// where->Expression("deleted","=","0");
		// where->LogicalAnd();
		where->Expression("__available","=","1");
		where->LogicalAnd();
		where->Expression("__available","!=","0");
	
		select->SetWhere(where);
		select->SetFrom(database->SanitizeTableName(service));
		select->SetLimit(offset,limit);
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		// select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::EnumerateWithLimitAndDirection( std::string service, int offset, int limit, std::string orderby, std::string attribute ) throw
				 (Exceptions::Security,Exceptions::Rollback)
{
	Core::Log::GetLog()->Error("Meow::PersistenceFactory::EnumerateWithLimitAndDirection","s","EYE!");
	CanRead();
	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		Persistence::Where_ptr where=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where");
		// where->Expression("deleted","=","0");
		// where->LogicalAnd();
		where->Expression("__available","=","1");
		where->LogicalAnd();
		where->Expression("__available","!=","0");
	
		select->SetWhere(where);
		select->SetFrom(database->SanitizeTableName(service));
		select->SetLimit(offset,limit);
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		if ( attribute.compare("ASC") )
			select->SetDescending();	
		if ( orderby.length() > 0 )
		{
			if ( attribute.compare("ASC") )
				select->SetDescending();	
			select->SetOrderBy(orderby);
		}
		// select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::EnumerateWithFilter( std::string service, Filter_ptr filter ) throw
				 (Exceptions::Security,Exceptions::Rollback)
{
	CanRead();
	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	if ( ((Persistence::Where_ptr)filter)->GetClauses()->Count()>0 )
	{
		((Persistence::Where_ptr)filter)->Ket();	
		((Persistence::Where_ptr)filter)->LogicalAnd();
	}
	// ((Persistence::Where_ptr)filter)->Expression("deleted","=","0");
	// ((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","=","1");

	((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","!=","0");
	
//	((Persistence::Where_ptr)filter)->Ket();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");

		select->SetFrom(database->SanitizeTableName(service));
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		select->SetWhere((Persistence::Where_ptr)filter);
		// select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::EnumerateWithFilterAndLimit( std::string service, Filter_ptr filter, int offset, int limit ) throw
				 (Exceptions::Security,Exceptions::Rollback)
{
	CanRead();
	if ( ((Persistence::Where_ptr)filter)->GetClauses()->Count()>0 )
	{
		((Persistence::Where_ptr)filter)->Ket();	
		((Persistence::Where_ptr)filter)->LogicalAnd();
	}
	// ((Persistence::Where_ptr)filter)->Expression("deleted","=","0");
	// ((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","=","1");
	((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","!=","0");
	
	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		select->SetFrom(database->SanitizeTableName(service));
		select->SetLimit(offset,limit);
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		select->SetWhere((Persistence::Where_ptr)filter);
		// select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::EnumerateWithFilterAndDirection( std::string service, Filter_ptr filter, std::string orderby, std::string attribute ) throw
				 (Exceptions::Security,Exceptions::Rollback)
{
	CanRead();
	if ( ((Persistence::Where_ptr)filter)->GetClauses()->Count()>0 )
	{
		((Persistence::Where_ptr)filter)->Ket();	
		((Persistence::Where_ptr)filter)->LogicalAnd();
	}
	// ((Persistence::Where_ptr)filter)->Expression("deleted","=","0");
	// ((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","=","1");

	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		select->SetFrom(database->SanitizeTableName(service));
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		select->SetWhere((Persistence::Where_ptr)filter);
		if ( orderby.length() > 0 )
		{
			select->SetOrderBy(orderby);
			if ( attribute!="ASC" )
				select->SetDescending();	

		}
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}

Meow::Core::Types::Array_ptr Meow::PersistenceFactory::EnumerateWithFilterAndLimitAndDirection( std::string service, Filter_ptr filter, int offset, int limit, std::string orderby, std::string attribute ) throw
				 (Exceptions::Security,Exceptions::Rollback)
{
	CanRead();
	if ( ((Persistence::Where_ptr)filter)->GetClauses()->Count()>0 )
	{
		((Persistence::Where_ptr)filter)->Ket();	
		((Persistence::Where_ptr)filter)->LogicalAnd();
	}
	// ((Persistence::Where_ptr)filter)->Expression("deleted","=","0");
	// ((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","=","1");

	((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","!=","0");
	
	Persistence::LazyEnumerator_ptr retval=new Persistence::LazyEnumerator();
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Select_ptr select=(Persistence::Select_ptr)GetRegistry()->GetMe("Persistence/Select");
		select->SetFrom(database->SanitizeTableName(service));
		select->SetLimit(offset,limit);
		Core::Types::Array_ptr attributes=GetSchemeKeys();
		Core::Types::Stack_ptr keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		/* DS - Support restore from attributes */
		attributes=GetSchemeAttributes();
		keys=attributes->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
			SchemaField_ptr field=(SchemaField_ptr)attributes->Get(key->GetValueAsString());
			select->AddColumn(field->iFieldName);
		}
		retval->SetLazyClassName(service);
		retval->SetLazyKeyFields(GetSchemeKeys());
		retval->SetLazyAttributeFields(GetSchemeAttributes());
		select->SetWhere((Persistence::Where_ptr)filter);
		if ( orderby.length() > 0 )
		{
			select->SetOrderBy(orderby);
			if ( attribute!="ASC" )
				select->SetDescending();	

		}
				// select->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)select);
		retval->SetResultObject(result);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
	return (Meow::Core::Types::Array_ptr)retval;
}
void Meow::PersistenceFactory::DestroyWithFilter( std::string service, Filter_ptr filter ) throw (Exceptions::Security,Exceptions::Rollback)
{
	CanDelete();
	if ( ((Persistence::Where_ptr)filter)->GetClauses()->Count()>0 )
	{
		((Persistence::Where_ptr)filter)->Ket();	
		((Persistence::Where_ptr)filter)->LogicalAnd();
	}
	// ((Persistence::Where_ptr)filter)->Expression("deleted","=","0");
	// ((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","=","1");

	((Persistence::Where_ptr)filter)->LogicalAnd();
	((Persistence::Where_ptr)filter)->Expression("__available","!=","0");
	
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor(service);
		Persistence::Delete_ptr operation=(Persistence::Delete_ptr)GetRegistry()->GetMe("Persistence/Delete");
		operation->SetFrom(database->SanitizeTableName(service));
		operation->SetWhere((Persistence::Where_ptr)filter);
		Persistence::SQLResult_ptr result=(Persistence::SQLResult_ptr)database->ObjectQuery((Persistence::Query_ptr)operation);
	} catch ( Exception &e )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(e.Diagnostic());
		throw rb;
	}
}
Meow::Core::Types::Array_ptr Meow::PersistenceFactory::Search( std::string humanstring, unsigned long offset, unsigned long limit, std::string service, Core::Types::Array_ptr constraints )
{
	Core::Types::Array_ptr retval=new Core::Types::Array();
	if ( GetSearchableParts()->Count() > 0 )
	{
		Persistence::Where_ptr iFilter=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where");
		bool first=true;
		if ( !constraints.IsNull() && constraints->Count() > 0 )
		{
			iFilter->Bra();
			Core::Types::Stack_ptr constraintKeys=constraints->GetKeys();
			while ( !constraintKeys->IsEmpty() )
			{
				Core::Types::ValueType_ptr constraintKey=(Core::Types::ValueType_ptr)constraintKeys->Pop();
				Core::Types::ValueType_ptr safeExpression=(Core::Types::ValueType_ptr)constraints->Get(constraintKey->GetValueAsString());
				if ( !first )
					iFilter->LogicalAnd();
				first=false;
				iFilter->Expression(constraintKey->GetValueAsString(),"=",safeExpression->GetValueAsString());
				
			}	
			iFilter->Ket();
			if ( humanstring.length() > 0 )
				iFilter->LogicalAnd();
		}
		first=true;
		if ( humanstring.length() > 0 )
		{
			iFilter->Bra();	
			Core::Types::Array_ptr searchList=GetSearchableParts();
			Core::Types::Stack_ptr keys=searchList->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
				if ( first )
					first=false;
				else
					iFilter->LogicalOr();
				SchemaField_ptr field=(SchemaField_ptr)searchList->Get(key->GetValueAsString());
				iFilter->Expression(field->iFieldName,"LIKE","%" + humanstring + "%");
			}
			iFilter->Ket();
		}
		std::string direction="ASC";
		if ( GetDescending() )
			direction="DESC";
		Core::Log::GetLog()->Error("Meow::PersistenceFactory::Search","ssss","(",humanstring.c_str(),")=",direction.c_str());
		// We should really find out what we are making ....!
		retval=EnumerateWithFilterAndLimitAndDirection(service,(Filter_ptr)iFilter,(int)offset,(int)limit,GetOrderByColumn(),direction);
	}
	return retval;
}
unsigned long Meow::PersistenceFactory::CountSearch( std::string humanstring,std::string service, Core::Types::Array_ptr constraints )
{
	unsigned long retval=0;
	if ( GetSearchableParts()->Count() > 0 )
	{
		Persistence::Where_ptr iFilter=(Persistence::Where_ptr)GetRegistry()->GetMe("Persistence/Where");
		bool first=true;
		if ( !constraints.IsNull() && constraints->Count() > 0 )
		{
			iFilter->Bra();
			Core::Types::Stack_ptr constraintKeys=constraints->GetKeys();
			while ( !constraintKeys->IsEmpty() )
			{
				Core::Types::ValueType_ptr constraintKey=(Core::Types::ValueType_ptr)constraintKeys->Pop();
				Core::Types::ValueType_ptr safeExpression=(Core::Types::ValueType_ptr)constraints->Get(constraintKey->GetValueAsString());
							
				if ( !first )
					iFilter->LogicalAnd();
				first=false;
				iFilter->Expression(constraintKey->GetValueAsString(),"=",safeExpression->GetValueAsString());
				
			}	
			iFilter->Ket();
			if ( humanstring.length() > 0 )
				iFilter->LogicalAnd();
		}
		first=true;
		if ( humanstring.length() > 0 )
		{
			iFilter->Bra();	
			Core::Types::Array_ptr searchList=GetSearchableParts();
			Core::Types::Stack_ptr keys=searchList->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
				if ( first )
					first=false;
				else
					iFilter->LogicalOr();
				SchemaField_ptr field=(SchemaField_ptr)searchList->Get(key->GetValueAsString());
				iFilter->Expression(field->iFieldName,"LIKE","%" + humanstring + "%");
			}
			iFilter->Ket();
		}
		// We should really find out what we are making ....!
		retval=CountEnumerateWithFilter(service,(Filter_ptr)iFilter);
	}
	return retval;	
}
