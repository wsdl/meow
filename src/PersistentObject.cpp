#include <PersistentObject.h>
#include <Persistence/Database.h>
#include <Persistence/Where.h>
#include <Persistence/Select.h>
#include <Persistence/Update.h>
#include <Persistence/Delete.h>
#include <Persistence/Insert.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>
#include <Core/Types/Boolean.h>
#include <Core/Time/Date.h>
#include <Core/Types/Array.h>
#include <Registry.h>
#include <Session.h>
#include <Security/User.h>
#include <Security/Group.h>
#include <inc/configuration.h>

Meow::PersistentObject::PersistentObject() : iNotifiable(true)
{
	SetMeowClassName("PersistentObject");
	iCachedJoinSpecs=new Core::Types::Array();
}
Meow::PersistentObject::~PersistentObject()
{

}
bool  Meow::PersistentObject::HasPublicVariable( std::string alias )
{
	SchemaField_ptr field=GetFactory()->GetAttributeByAlias(alias);
	return !field.IsNull();			
}
Meow::Core::ptr Meow::PersistentObject::GetPublicVariable( std::string alias )
{
	Meow::Core::ptr retval;
	SchemaField_ptr field=GetFactory()->GetAttributeByAlias(alias);
	if ( !field.IsNull() )		
    {
        retval=LifecycleObject::GetVariable(field->iFieldName);   
        if ( !retval.IsNull() )
        {
            if ( retval->IsInstanceOf("ValueType") )
            {
                if ( field->iColumnType==Factory::Object )
                {
                    // Its an object so if we get something from here that's not null
                    // and its a string, resolve it. If not a string, return it. If we have
                    // to resolve it and we can't, its NULL (related object reference destroyed).
                    try
                    { 
                        Core::Types::ValueType_ptr retvalAsVT=(Core::Types::ValueType_ptr)retval;
                        retval=GetFactory()->GetRegistry()->FindMeByObjectReference(retvalAsVT->GetValueAsString());
                        LifecycleObject::SetVariable(field->iFieldName,retval);
                    } catch ( Exceptions::NotFound &nf )
                    {
                        retval=NULL;
                    }
                }
            }
        }
    }
	return retval;
}
void Meow::PersistentObject::SetPublicVariable( std::string alias, Core::ptr value )
{
	// Lets see, has it changed?
	SchemaField_ptr field=GetFactory()->GetAttributeByAlias(alias);
	if ( field.IsNull() )
		field=(SchemaField_ptr)GetFactory()->GetAttributes()->Get(alias);
	Core::ptr currentVariable=LifecycleObject::GetVariable(field->iFieldName);
	bool altered=true;
	if ( currentVariable.getPointer()!=value.getPointer() )
	{
		if ( value.IsNull() && currentVariable.IsNull() )
			altered=false;
		else if ( value.IsNull() || currentVariable.IsNull() )
			altered=true;	
		else 
		{
			// TODO:
			// We should speed up these comparisons by using a luid system
			if ( value->GetMeowClassName() != currentVariable->GetMeowClassName() )
				altered=true;
			else
			{
				// So now we need to see what its become
				if ( value->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsVType=(Core::Types::ValueType_ptr)value;
					Core::Types::ValueType_ptr currentVariableAsVType=(Core::Types::ValueType_ptr)currentVariable;
					altered=valueAsVType->GetValueAsString()!=currentVariableAsVType->GetValueAsString();
					Core::Log::GetLog()->Error("Meow::PersistentObject::SetPublicVariable","sss",valueAsVType->GetValueAsString().c_str(),"=",currentVariableAsVType->GetValueAsString().c_str());
				}
				else
				{
					Object_ptr valueAsObject=(Object_ptr)value;
					Object_ptr currentAsObject=(Object_ptr)currentVariable;
					altered=valueAsObject->ToString() != currentAsObject->ToString();
					Core::Log::GetLog()->Error("Meow::PersistentObject::SetPublicVariable","sss",valueAsObject->ToString().c_str(),"=",currentAsObject->ToString().c_str());

				}
			}
		}
	}
	/* if ( altered && iNotifiable )
	{
		Core::Types::Array_ptr evParms=new Core::Types::Array();
		Core::ptr thisObject=this;
		evParms->Set("this",thisObject);
		evParms->Set("field",new Core::Types::String(alias));
		evParms->Set("become",value);
		Registry::GetRegistry()->PostEvent("PersistentObjectAltered",evParms);
	} */
	if ( altered )
	{
		field->iChanged=true;		
		LifecycleObject::SetVariable(field->iFieldName,value);
		MarkDirty();
	}	

}
bool Meow::PersistentObject::IsPurgeable( )
{
	return true;
}
Meow::Object_ptr Meow::PersistentObject::GetPersistenceDescriptor()
{
	return ((PersistenceFactory_ptr)GetFactory())->GetPersistenceDescriptor(GetMeowClassName());
}
Meow::Object_ptr Meow::PersistentObject::GetFilter(  )
{
	if ( iFilterDescriptor.IsNull() )
	{
		try
		{
			Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Where");
		//	filter->Expression("","=","0");
		//	filter->LogicalAnd();
			filter->Bra();
			Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
			if ( GetFactory()->GetSerial() )
			{
				// Its only got one key
				Core::Types::Array_ptr keys=((Factory_ptr)GetFactory())->GetSchemeKeys();
				SchemaField_ptr field=(SchemaField_ptr)keys->Get(0);

				std::string colName=field->iFieldName;

				if ( GetUniqueKey()->Count() == 0 )
				{
					filter->Expression(colName,"=",0);
				}
				else
				{
					Core::Types::Array_ptr key=GetUniqueKey();
					Core::Types::ValueType_ptr keyComponent=(Core::Types::ValueType_ptr)key->Get(0);
					filter->Expression(colName,"=",keyComponent->GetValueAsInteger());
				}
			}
			else if ( GetFactory()->GetKeySet() )
			{
				// Each key definition in the scheme keys has an equivalent entry in the unique key
				Core::Types::Stack_ptr keys=GetFactory()->GetSchemeKeys()->GetKeys();
				int primaryKeyIndex=0;
				while ( !keys->IsEmpty() )
				{
					Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
					SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeKeys()->Get(key->GetValueAsString());
					// What we want to know here is the column type, this allows us to build the query properly
					// everything else we can ignore
					Core::ptr keyValue=GetUniqueKey()->Get(primaryKeyIndex);
					if ( primaryKeyIndex > 0 )
						filter->LogicalAnd();
					switch ( field->iColumnType )
					{
					case Factory::Object:
						{
							std::string keyAsString="";
							if ( !keyValue.IsNull() )
							{
								if ( keyValue->GetMeowClassName() == "String" )
									keyAsString=((Core::Types::String_ptr)keyValue)->GetValueAsString();
								else
									keyAsString=((Object_ptr)keyValue)->GetMeowClassName() + "@" + GetFactory()->GetRegistry()->CompileKey(((Object_ptr)keyValue)->GetUniqueKey());
							}
							filter->Expression(key->GetValueAsString(),"=",keyAsString);
						}
						break;
					case Factory::String:
					case Factory::Text:
					case Factory::HTML:
					case Factory::XML:
					case Factory::Password:
					case Factory::Enumeration:
					case Factory::Image:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
						break;
					case Factory::Integer:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsInteger());
						break;
					case Factory::Float:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsFloat());
						break;
					case Factory::Boolean:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsBoolean());
						break;
					case Factory::Date:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsInteger());
						break;
					case Factory::Array:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
						break;
					case Factory::Queue:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
						break;
					case Factory::Stack:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
						break;
					case Factory::Void:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::Void_ptr)keyValue)->ToString());
						break;
					case Factory::ValueType:
						filter->Expression(key->GetValueAsString(),"=",((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
						break;
					default:
						// OK so the max it can be is a ptr, we can't store/retrieve those by default so
						// lets store a ""
						std::string keyAsString="";
						filter->Expression(key->GetValueAsString(),"=","");
					}
					
					primaryKeyIndex++;
				}
			}
			else
			{
				Exceptions::BadParameter bp;
				bp.SetMessage("Class schema " + GetMeowClassName() + " does not include a compound or a serial key, so normal persistence methods will not work.");
				throw bp;
			}
			filter->Ket();
			iFilterDescriptor=filter;
		} catch ( Exceptions::NotFound &nf )
		{
			Exceptions::Rollback rb;
			rb.SetMessage(nf.Diagnostic());
			rb.SetMessage("Could not get a filter descriptor for this class [" + GetMeowClassName() + "] therefore Rollback will be thrown.");
			throw rb;
		}
	}
	return iFilterDescriptor;
}
void Meow::PersistentObject::Restore( )
{
	// If its a serial, then we use the first value from GetUniqueKey
	bool success=false;
	if ( GetFactory()->GetSchemeAttributes()->Count() > 0 )
	{
		if ( GetFactory()->GetSerial() )
		{
			if ( GetUniqueKey()->Count() == 0 )
			{
				Exceptions::NotFound nf;
				nf.SetMessage("Serial key (autoincrement) and empty key - behaviour is to throw.");
				throw nf;
			}
			else if ( GetUniqueKey()->Count() == 1 )
			{
				Core::ptr key=GetUniqueKey()->Get(0);
				if ( key.IsNull() )
				{
					Exceptions::NotFound nf;
					nf.SetMessage("Serial key (autoincrement) and null key member 0- behaviour is to throw.");
					throw nf;
				}
				else
				{
					Core::Types::ValueType_ptr keyAsVT=(Core::Types::ValueType_ptr)key;
					if ( keyAsVT->Count() == 0 )
					{
						Exceptions::NotFound nf;
						nf.SetMessage("Serial key (autoincrement) and zero length key member 0 - behaviour is to throw.");
						throw nf;
					}
				}
			}
		}
		else
		{
			// Its not a serial, we need to cash/convert everything.
			unsigned long keyptr=0;
			Core::Types::Stack_ptr keys=GetFactory()->GetSchemeKeys()->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
				SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeKeys()->Get(key->GetValueAsString());
				Core::ptr value=GetUniqueKey()->Get(keyptr);
				if ( !value.IsNull() && field->iColumnType == Factory::Object )
				{
					try
					{
						Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","ds",keyptr," Test restore from object ref and override content.");

						// TO RESUME HERE - DS
						// ADD DEBUGGING OUTPUT
						if ( !value->IsInstanceOf("LifecycleObject") )
						{
							GetUniqueKey()->Set(keyptr,(Core::ptr)GetFactory()->GetRegistry()->FindMeByObjectReference(((Core::Types::ValueType_ptr)value)->GetValueAsString()));
							Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","ds",keyptr," overridden key component");
						}
						else
							Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","sss",value->GetMeowClassName().c_str(),"=","LifecycleObject");
					} catch ( ... )
					{
						Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","sss","Catch [",((Core::Types::ValueType_ptr)value)->GetValueAsString().c_str(),"]");
				
					}
				}
				else
					Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","ds",keyptr," is NULL or not an Object");
				keyptr++;
			}
		}
		std::string diagnosticMessage="";
		try
		{
			Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
			// We got a database, so lets try and generate the query.
			Persistence::Select_ptr operation=(Persistence::Select_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Select");
			operation->SetFrom(database->SanitizeTableName(GetMeowClassName()));

			Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
			operation->SetWhere(filter);
			Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
				operation->AddColumn(key->GetValueAsString());
			}
			Persistence::SQLResult_ptr result=database->ObjectQuery((Persistence::Query_ptr)operation);

			// Build up the property assigns, we need the definition from the factory.
			keys=GetFactory()->GetSchemeAttributes()->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
				Core::Types::ValueType_ptr storeResult=result->FetchCellDataAsValue(0,key->GetValueAsString());
				Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","sss","storeResult=[",storeResult->GetValueAsString().c_str(),"]");
				SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeAttributes()->Get(key->GetValueAsString());
				Core::ptr value;
				// Core::ptr value=GetVariable(key->GetValueAsString());
				switch ( field->iColumnType )
				{
				case Factory::Object:
					try
					{
						if ( storeResult->GetValueAsString().length() > 0 )
						{
							
                            // DS - 30/4/18 - only get the object if we need to, otherwise store it. 
                            // so store result should be a string.
                            // Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","sss","find[",storeResult->GetValueAsString().c_str(),"]");
                            //	value=GetFactory()->GetRegistry()->FindMeByObjectReference(storeResult->GetValueAsString());	
                            value=(Core::ptr)storeResult;
						}
					
					} catch ( ... )
					{
						Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","s","Catch");
				
					}
					break;
				case Factory::String:
				case Factory::Text:
				case Factory::HTML:
				case Factory::XML:
				case Factory::Password:
					case Factory::Enumeration:
					case Factory::Image:
					value=new Core::Types::String(storeResult->GetValueAsString());

					break;
				case Factory::Integer:
					value=new Core::Types::Integer(storeResult->GetValueAsInteger());
					break;
				case Factory::Float:
					value=new Core::Types::Float(storeResult->GetValueAsFloat());
					break;
				case Factory::Boolean:
					value=new Core::Types::Boolean(storeResult->GetValueAsBoolean());
					break;
				case Factory::Date:
					value=new Core::Time::Date(storeResult->GetValueAsInteger());
					break;
				case Factory::Array:
					value=new Core::Types::Array();
					((Core::Types::Array_ptr)value)->Import(storeResult->GetValueAsString());
					break;
				case Factory::Queue:
					// TODO: Implement Queue XML
					// value=new Core::Types::Queue(storeResult->GetValueAsString());
					break;
				case Factory::Stack:
					// TODO: Implement Stack XML
					// value=new Core::Types::Stack(storeResult->GetValueAsString());
					break;
				case Factory::Void:
					value=new Core::Types::Void(storeResult->GetValueAsString());
					break;
				case Factory::ValueType:
				default:
					value=storeResult;
				}
//				Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","sss","done[",storeResult->GetValueAsString().c_str(),"]");
				//			Core::Log::GetLog()->Information("Meow::PersistentObject::Restore","ss","Setting variable:",value->GetMeowClassName().c_str());
				Object::SetVariable(key->GetValueAsString(),(Core::ptr)value);
			}
			success=true;
			ClearDirty();
		} catch ( Exception &e )
		{
			diagnosticMessage=e.Diagnostic();
		}

		if ( !success )
		{
			Exceptions::NotFound rb;
			rb.SetMessage(rb.Diagnostic());
			throw rb;
		}
	}
}
void Meow::PersistentObject::RestoreFromAttributes( Core::Types::Array_ptr attributes )
{
	// If its a serial, then we use the first value from GetUniqueKey
	bool success=false;
	if ( GetFactory()->GetSchemeAttributes()->Count() > 0 )
	{
		std::string diagnosticMessage="";
		try
		{
			// Build up the property assigns, we need the definition from the factory.
			Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
				SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeAttributes()->Get(key->GetValueAsString());
				Core::Types::ValueType_ptr storeResult=(Core::Types::ValueType_ptr)attributes->Get(field->iPublicName);
				Core::ptr value;
				// Core::ptr value=GetVariable(key->GetValueAsString());
				switch ( field->iColumnType )
				{
				case Factory::Object:
					if ( storeResult->Count() > 0 )
					{
                        /* DS - 30/04/18 - do not store the object, just its reference */
						// try
						// {
                        // value=GetFactory()->GetRegistry()->FindMeByObjectReference(storeResult->GetValueAsString());
						// } catch ( Exceptions::NotFound &nf )
						// {
						//	Core::Log::GetLog()->Error("Meow::PersistentObject::RestoreFromAttributes","ss","Could not restore object:",storeResult->GetValueAsString().c_str());
						// }
						value=(Core::ptr)storeResult;
					}
					break;
				case Factory::String:
				case Factory::Text:
					case Factory::HTML:
					case Factory::XML:
				case Factory::Password:
					case Factory::Enumeration:
					case Factory::Image:
					value=new Core::Types::String(storeResult->GetValueAsString());
					break;
				case Factory::Integer:
					value=new Core::Types::Integer(storeResult->GetValueAsInteger());
					break;
				case Factory::Float:
					value=new Core::Types::Float(storeResult->GetValueAsFloat());
					break;
				case Factory::Boolean:
					value=new Core::Types::Boolean(storeResult->GetValueAsBoolean());
					break;
				case Factory::Date:
					value=new Core::Time::Date(storeResult->GetValueAsInteger());
					break;
				case Factory::Array:
					value=new Core::Types::Array();
					((Core::Types::Array_ptr)value)->Import(storeResult->GetValueAsString());
					break;
				case Factory::Queue:
				// TODO: XML Queue
				//	value=new Core::Types::Queue(storeResult->GetValueAsString());
					break;
				case Factory::Stack:
				// TODO: XML Stack
				//	value=new Core::Types::Stack(storeResult->GetValueAsString());
					break;
				case Factory::Void:
					value=new Core::Types::Void(storeResult->GetValueAsString());
					break;
				case Factory::ValueType:
				default:
					value=storeResult;
				}
				Object::SetVariable(field->iFieldName,(Core::ptr)value);

			}
			ClearDirty();
			success=true;
		} catch ( Exception &e )
		{
			diagnosticMessage=e.Diagnostic();
		}
		// If we failed for any reason to execute the deletion, throw a rollback exception.
		if ( !success )
		{
			Exceptions::Rollback rb;
			rb.SetMessage(rb.Diagnostic());
			throw rb;
		}
	}
}
void Meow::PersistentObject::Passivate( )
{
	// If its a serial, then we use the first value from GetUniqueKey
	bool success=false;
	bool changesPickedUp=false;
	if ( GetFactory()->GetSchemeAttributes()->Count() > 0 )
	{
		
		std::string diagnosticMessage="";
		try
		{
			Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
			// We got a database, so lets try and generate the query.
			Persistence::Update_ptr operation=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
			operation->SetWhat(database->SanitizeTableName(GetMeowClassName()));
			Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
			operation->SetWhere(filter);
			// Build up the property assigns, we need the definition from the factory.
			Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
			while ( !keys->IsEmpty() )
			{
				Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
				SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeAttributes()->Get(key->GetValueAsString());
				if ( field->iChanged )
				{
					Core::ptr value=GetVariable(key->GetValueAsString());
					switch ( field->iColumnType )
					{
					case Factory::Object:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),std::string(""));
						else
						{
                            // If its a string, its not changed, therefore we can ignore it.
                            if ( !value->IsInstanceOf("ValueType") )
                                operation->AddAssign(key->GetValueAsString(),((Object_ptr)value)->GetMeowClassName() + "@" + GetFactory()->GetRegistry()->CompileKey(((Object_ptr)value)->GetUniqueKey()));
                            else
                            {
                                operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsString());                                    
                            }
						}
						break;
					case Factory::String:
					case Factory::Text:
					case Factory::HTML:
					case Factory::XML:
					case Factory::Password:
						case Factory::Enumeration:
						case Factory::Image:
						case Factory::File:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),std::string(""));
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsString());
						break;
					case Factory::Integer:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),0);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsInteger());
						break;
					case Factory::Float:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),0.0);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsFloat());
						break;
					case Factory::Boolean:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),false);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsInteger());
						break;
					case Factory::Date:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),0);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsInteger());
						break;
					case Factory::Array:
					case Factory::Queue:
					case Factory::Stack:
					case Factory::ValueType:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),std::string(""));
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsString());
						break;
					case Factory::Void:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),std::string(""));
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::Void_ptr)value)->ToString());
						break;
					default:
						operation->AddAssign(key->GetValueAsString(),std::string(""));
					}
					changesPickedUp=true;
					field->iChanged=false;
				}
			}
			operation->AddAssign("__available",new Core::Types::Integer(1));
				
			// Be strict about handling returns from functions.
			Persistence::SQLResult_ptr result;
			Core::Log::GetLog()->Error("operation->AddAssign","ss","__available=","1");
			operation->AddAssign("__available","1");

			if (changesPickedUp)
			{
				Core::Time::Date_ptr now=new Core::Time::Date();
				operation->AddAssign("___changed",now->GetValueAsString());
				// DS - Add the security user detection into ___user
				Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
				if ( !session.IsNull() )
				{
					Security::User_ptr user=(Security::User_ptr)session->GetCurrentUser();
					if ( !user.IsNull() )
					{
						operation->AddAssign("___user",user->ToString());
					}
				}
				result=database->ObjectQuery((Persistence::Query_ptr)operation);	
				PostUpdated();
				MakeBeforeImage();
			}
			ClearDirty();
			success=true;
		} catch ( Exception &e )
		{
			diagnosticMessage=e.Diagnostic();
		}
		// If we failed for any reason to execute the deletion, throw a rollback exception.
		if ( !success )
		{
			Exceptions::Rollback rb;
			rb.SetMessage(rb.Diagnostic());
			throw rb;
		}
	}

}
void Meow::PersistentObject::_Delete( )
{
	// If its a serial, then we use the first value from GetUniqueKey
	bool success=false;
	std::string diagnosticMessage="";
	try
	{
		Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
		// First we update the user to who deleted it, if its available
		// DS - Add the security user detection into ___user
		Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
		
		Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
		Persistence::SQLResult_ptr result;
		if ( !session.IsNull() )
		{
			Security::User_ptr user=(Security::User_ptr)session->GetCurrentUser();
			if ( !user.IsNull() )
			{
				Persistence::Update_ptr update=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
				update->SetWhat(database->SanitizeTableName(GetMeowClassName()));
				update->AddAssign("___user",user->ToString());
				update->SetWhere(filter);
				result=database->ObjectQuery((Persistence::Query_ptr)update);
			}
		}
		
		
		// We got a database, so lets try and generate the query.
		Persistence::Delete_ptr operation=(Persistence::Delete_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Delete");		
		operation->SetFrom(database->SanitizeTableName(GetMeowClassName()));
		operation->SetWhere(filter);
		Core::Time::Date_ptr now=new Core::Time::Date();
		// operation->AddAssign("",(Core::Types::ValueType_ptr)now);
		// Be strict about handling returns from functions.
		result=database->ObjectQuery((Persistence::Query_ptr)operation);
		success=true;
	/*	if ( iNotifiable )
		{
			Core::Types::Array_ptr evParms=new Core::Types::Array();
			Core::ptr thisObject=this;
			evParms->Set("this",thisObject);
			Registry::GetRegistry()->PostEvent("PersistentObjectDeleted",evParms);
		} */
		ClearDirty();
	} catch ( Exception &e )
	{
		diagnosticMessage=e.Diagnostic();
	}
	// If we failed for any reason to execute the deletion, throw a rollback exception.
	if ( !success )
	{
		Exceptions::Rollback rb;
		rb.SetMessage(rb.Diagnostic());
		throw rb;
	}
}
void Meow::PersistentObject::Create( )
{
	// If its a serial, then we use the first value from GetUniqueKey
		bool success=false;
		if ( GetFactory()->GetSchemeAttributes()->Count() > 0 )
		{
			std::string diagnosticMessage="";
			try
			{
				Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
				// We got a database, so lets try and generate the query.
				Persistence::Insert_ptr operation=(Persistence::Insert_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Insert");
				operation->SetInto(database->SanitizeTableName(GetMeowClassName()));
				// Build up the assigns from key values, if its not a serial key
				if ( !GetFactory()->GetSerial() && GetFactory()->GetKeySet() )
				{
					Core::Types::Array_ptr keyArray=GetFactory()->GetSchemeKeys();
					Core::Types::Stack_ptr keys=keyArray->GetKeys();
					int primaryKeyIndex=0;
					while ( !keys->IsEmpty() )
					{
						Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
						SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeKeys()->Get(key->GetValueAsString());
						// What we want to know here is the column type, this allows us to build the query properly
						// everything else we can ignore
						Core::ptr keyValue=GetUniqueKey()->Get(primaryKeyIndex);
						switch ( field->iColumnType )
						{
						case Factory::Object:
							{
																
								std::string keyAsString="";
								// Now we need to check the keyvalue, if its Current/User or Current/User reference we have something specific to set.
								
								if ( !keyValue.IsNull() )
								{	
									
									operation->AddAssign(key->GetValueAsString(),((Object_ptr)keyValue)->GetMeowClassName() + "@" + GetFactory()->GetRegistry()->CompileKey(((Object_ptr)keyValue)->GetUniqueKey()));
									
								}
								else
									operation->AddAssign(key->GetValueAsString(),new Core::Types::String(""));
							}
							break;
						case Factory::String:
						case Factory::Text:
						case Factory::HTML:
						case Factory::XML:
						case Factory::Password:
						case Factory::Enumeration:
						case Factory::Image:
						case Factory::File:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
							break;
						case Factory::Integer:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)keyValue)->GetValueAsInteger());
							break;
						case Factory::Float:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)keyValue)->GetValueAsFloat());
							break;
						case Factory::Boolean:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)keyValue)->GetValueAsInteger());
							break;
						case Factory::Date:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)keyValue)->GetValueAsInteger());
							break;
						case Factory::Array:
						case Factory::Queue:
						case Factory::ValueType:
						case Factory::Stack:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)keyValue)->GetValueAsString());
							break;
						case Factory::Void:
							operation->AddAssign(key->GetValueAsString(),((Core::Types::Void_ptr)keyValue)->ToString());
							break;
						default:
							operation->AddAssign(key->GetValueAsString(),new Core::Types::String(""));
						}
						primaryKeyIndex++;
					}
				}

				// Build up the property assigns, we need the definition from the factory.
				Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
				while ( !keys->IsEmpty() )
				{
					Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
					SchemaField_ptr field=(SchemaField_ptr)GetFactory()->GetSchemeAttributes()->Get(key->GetValueAsString());
					Object::SetVariable(key->GetValueAsString(),field->iDefaultValue);
					Core::ptr value=Object::GetVariable(key->GetValueAsString());
					switch ( field->iColumnType )
					{
					case Factory::Object:
						{
							if ( value.IsNull() )
							{
								operation->AddAssign(key->GetValueAsString(),std::string(""));
							}
							else 
							{
								if ( value->GetMeowClassName() == "Current/User" )
								{
									LifecycleObject_ptr currentUser=Registry::GetRegistry()->GetMe("Current/User");
									operation->AddAssign(key->GetValueAsString(),currentUser->ToString());
								}
								else if (  value->GetMeowClassName() == "Current/UserReference" )
								{
									LifecycleObject_ptr currentUser=Registry::GetRegistry()->GetMe("Current/UserReference");
									operation->AddAssign(key->GetValueAsString(),currentUser->ToString());
								}
								else
									operation->AddAssign(key->GetValueAsString(),((Object_ptr)value)->GetMeowClassName() + "@" + GetFactory()->GetRegistry()->CompileKey(((Object_ptr)value)->GetUniqueKey()));
							}
							break;
						}	
					case Factory::String:
					case Factory::Text:
					case Factory::HTML:
					case Factory::XML:
					case Factory::Password:
					case Factory::Enumeration:
					case Factory::Image:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),new Core::Types::String(""));
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsString());
						break;
					case Factory::Integer:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),0);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsInteger());
						break;
					case Factory::Float:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),0.0);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsFloat());
						break;
					case Factory::Boolean:
	
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),false);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsInteger());
						break;
					case Factory::Date:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),0);
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsInteger());
						break;
					case Factory::Array:
					case Factory::Queue:
					case Factory::Stack:
					case Factory::ValueType:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),std::string(""));
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::ValueType_ptr)value)->GetValueAsString());
						break;
					case Factory::Void:
						if ( value.IsNull() )
							operation->AddAssign(key->GetValueAsString(),std::string(""));
						else
							operation->AddAssign(key->GetValueAsString(),((Core::Types::Void_ptr)value)->ToString());
						break;
					default:
						operation->AddAssign(key->GetValueAsString(),"");
					}
				}
				
				operation->AddAssign("__available","0");
				// Be strict about handling returns from functions.
				Core::Log::GetLog()->Warning("Meow::PersistentObject::Create","s","About to call Object Query");
				// DS - Add the security user detection into ___user
				Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
				if ( !session.IsNull() )
				{
					Security::User_ptr user=(Security::User_ptr)session->GetCurrentUser();
					if ( !user.IsNull() )
					{
						operation->AddAssign("___user",user->ToString());
					}
				}
				Persistence::SQLResult_ptr result=database->ObjectQuery((Persistence::Query_ptr)operation);
				Core::Log::GetLog()->Warning("Meow::PersistentObject::Create","s","Done.");
				success=true;
				if ( GetFactory()->GetSerial() )
				{
					Core::Types::Array_ptr newKeyData=new Core::Types::Array();
					newKeyData->Set(0,new Core::Types::String(result->GetLastInsertID()));
					SetUniqueKey(newKeyData);
				}
				/* if ( iNotifiable )
				{
					Core::Types::Array_ptr evParms=new Core::Types::Array();
					Core::ptr thisObject=this;
					evParms->Set("this",thisObject);
					Registry::GetRegistry()->PostEvent("PersistentObjectAdded",evParms);
				} */ 
				ClearDirty();
			} catch ( Exception &e )
			{
				diagnosticMessage=e.Diagnostic();
			}
			// If we failed for any reason to execute the deletion, throw a rollback exception.
			if ( !success )
			{
				Exceptions::Rollback rb;
				rb.SetMessage(rb.Diagnostic());
				throw rb;
			}
		}
		Core::Log::GetLog()->Warning("Meow::PersistentObject::Create","s","Returning.");
}
Meow::Core::ptr Meow::PersistentObject::ExecuteMethod( std::string method, Core::Types::Array_ptr parameters)
{
	Meow::Core::ptr retval;
	// So we need to intercept methods that begin with Set
	// or Get.
	Core::Types::String_ptr methodAsString=new Core::Types::String(method);
	if ( methodAsString->Count()==3 )
	{
		Core::Types::String_ptr prefix=methodAsString->SubString(0,3);
		if ( prefix->GetValueAsString() == "Get" )
		{
			Core::Types::String_ptr attribute=methodAsString->SubString(3,methodAsString->Count());
			SchemaField_ptr field=GetFactory()->GetAttributeByAlias(field->iPublicName);
			if ( field.IsNull() )
			{
				// Try and get it from the key
				field=GetFactory()->GetKeyAttributeByAlias(field->iPublicName);
				retval=GetUniqueKey()->Get(field->iOffset);
			} else
			{
				Session_ptr currentSession=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
				if ( currentSession.IsNull() )
				{
					Security::User_ptr currentUser=(Security::User_ptr)currentSession->GetCurrentUser();
					if ( !currentUser.IsNull() )
					{
						bool canSeeAttribute=false;
						try
						{
							Security::Group_ptr group=(Security::Group_ptr)Registry::GetRegistry()->FindMe("Security/Group",1,field->iSecurityGroupRead.c_str());
							if ( group->IsAuthorised(currentUser) )
								retval=Object::GetVariable(attribute->GetValueAsString());
							else
							{
								Core::Log::GetLog()->Error("Meow::Factory::ExecuteMethod","ssss","Not a member of ",field->iSecurityGroupRead.c_str()," when trying to satisfy method call ",method.c_str());
							}
						}
						catch ( Exceptions::NotFound &nf )
						{
							Core::Log::GetLog()->Error("Meow::Factory::ExecuteMethod","ssss","Can't find ",field->iSecurityGroupRead.c_str()," when trying to satisfy method call ",method.c_str());
						}

					}
				}
			}
		}
		else if ( prefix->GetValueAsString() == "Set" )
		{
			Core::Types::String_ptr attribute=methodAsString->SubString(3,methodAsString->Count());
			SchemaField_ptr field=GetFactory()->GetAttributeByAlias(field->iPublicName);
			if ( !field.IsNull() )
			{
				Session_ptr currentSession=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
				if ( currentSession.IsNull() )
				{
					Security::User_ptr currentUser=(Security::User_ptr)currentSession->GetCurrentUser();
					if ( !currentUser.IsNull() )
					{
						bool canSeeAttribute=false;
						try
						{
							Security::Group_ptr group=(Security::Group_ptr)Registry::GetRegistry()->FindMe("Security/Group",1,field->iSecurityGroupWrite.c_str());
							if ( group->IsAuthorised(currentUser) )
								retval=Object::GetVariable(attribute->GetValueAsString());
							else
							{
								Core::Log::GetLog()->Error("Meow::Factory::ExecuteMethod","ssss","Not a member of ",field->iSecurityGroupWrite.c_str()," when trying to satisfy method call ",method.c_str());
							}
						}
						catch ( Exceptions::NotFound &nf )
						{
							Core::Log::GetLog()->Error("Meow::Factory::ExecuteMethod","ssss","Can't find ",field->iSecurityGroupWrite.c_str()," when trying to satisfy method call ",method.c_str());
						}
					}
				}
			}
		}
		else if ( prefix->GetValueAsString() == "CountJoin" )
		{
			Core::Types::ValueType_ptr relationship=(Core::Types::ValueType_ptr)parameters->Get(0);
			retval=new Core::Types::Integer(CountJoin(relationship->GetValueAsString()));
		}
		else if ( prefix->GetValueAsString() == "GetJoin" )
		{
			Core::Types::ValueType_ptr relationship=(Core::Types::ValueType_ptr)parameters->Get(0);
			retval=GetJoin(relationship->GetValueAsString());
		}
	}
	return retval;
}
unsigned long Meow::PersistentObject::CountJoin( std::string relationship )
{
	unsigned long retval=0;
	Filter_ptr filter=JoinSpecToFilter( relationship);
	if ( filter.IsNull( ) )
	{
		Core::Log::GetLog()->Error("Meow::PersistenctObject::CountJoin","ss","Null Join Filter for ",relationship.c_str());
	}
	else
	{
		JoinSpec_ptr spec=GetFactory()->GetJoinSpec(relationship);
		try
		{	
			PersistenceFactory_ptr otherFactory=(PersistenceFactory_ptr)Registry::GetRegistry()->FindFactory(spec->iOtherClass+"Factory");
			retval=otherFactory->CountEnumerateWithFilter(spec->iOtherClass,filter);
		} catch ( Exceptions::NotFound &nf )
		{
			Core::Log::GetLog()->Error("Meow::PersistenctObject::CountJoin","ss","Cannot find factory for ",relationship.c_str());	
		}
	}
	return retval;
}
Meow::Core::Types::Array_ptr Meow::PersistentObject::GetJoin( std::string relationship )
{
	Core::Types::Array_ptr retval;
	Filter_ptr filter=JoinSpecToFilter( relationship);
	if ( filter.IsNull( ) )
	{
		Core::Log::GetLog()->Error("Meow::PersistenctObject::GetJoin","ss","Null Join Filter for ",relationship.c_str());	
	}
	else
	{
		JoinSpec_ptr spec=GetFactory()->GetJoinSpec(relationship);
		try
		{	
			PersistenceFactory_ptr otherFactory=(PersistenceFactory_ptr)Registry::GetRegistry()->FindFactory(spec->iOtherClass+"Factory");
			retval=otherFactory->EnumerateWithFilter(spec->iOtherClass,filter);
		} catch ( Exceptions::NotFound &nf )
		{
			Core::Log::GetLog()->Error("Meow::PersistenctObject::GetJoin","ss","Cannot find factory for ",relationship.c_str());	
		}
	}
	return retval;
}
Meow::Filter_ptr Meow::PersistentObject::JoinSpecToFilter( std::string relationshipname )
{
	
	Meow::Filter_ptr retval;
	try
	{
		if ( !iCachedJoinSpecs->IsSet(relationshipname) )
		{
			
			retval=(Meow::Filter_ptr)Registry::GetRegistry()->GetMe("Persistence/Where");
			iCachedJoinSpecs->Set(relationshipname,(Core::ptr)retval);
			Meow::Persistence::Where_ptr where=(Meow::Persistence::Where_ptr)retval;
			JoinSpec_ptr joinspec=GetFactory()->GetJoinSpec(relationshipname);
			Core::Types::Stack_ptr keys=joinspec->iLinkList->GetKeys();
			bool iFirst=true;
			while ( !keys->IsEmpty() )
			{
				Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
				Linkage_ptr link=(Linkage_ptr)joinspec->iLinkList->Get(key->GetValueAsString());
				
				std::string op="";
				switch ( link->Operation )
				{
				case Linkage::LessThan: op="<"; break;
				case Linkage::LessThanOrEqualTo: op="<="; break;
				case Linkage::MoreThan: op=">"; break;
				case Linkage::MoreThanOrEqualTo: op=">="; break;
				case Linkage::Equals: op="="; break;
				case Linkage::NotEqualTo: op="!="; break;
				}
				
				if ( iFirst )
					iFirst=false;
				else
					where->LogicalAnd();

				std::string stringifiedLeftValue="";
				Core::ptr leftValue=GetVariable(link->iLeftField->iFieldName);
				if ( leftValue->IsInstanceOf("ValueType") )
				{
					Core::Types::ValueType_ptr valueAsValueType=(Core::Types::ValueType_ptr)leftValue;
					stringifiedLeftValue=valueAsValueType->GetValueAsString();
				}
				else if ( link->iRightField->IsInstanceOf("Object") )
				{
					Object_ptr valueAsObject=(Object_ptr)leftValue;
					stringifiedLeftValue=valueAsObject->ToString();
				}
				else
				{
					stringifiedLeftValue=leftValue->GetMeowClassName();
				}

				if ( link->TargetType == Linkage::Field )
					where->LinkColumn(stringifiedLeftValue,op,((SchemaField_ptr)link->iRightField)->iFieldName);
				else
				{
					std::string stringifiedValue="";

					if ( !link->iRightField.IsNull() )
					{
						if ( link->iRightField->IsInstanceOf("ValueType") )
						{
							Core::Types::ValueType_ptr valueAsValueType=(Core::Types::ValueType_ptr)link->iRightField;
							stringifiedValue=valueAsValueType->GetValueAsString();
						}
						else if ( link->iRightField->IsInstanceOf("Object") )
						{
							Object_ptr valueAsObject=(Object_ptr)link->iRightField;
							stringifiedValue=valueAsObject->ToString();
						}
						else
						{
							stringifiedValue=link->iRightField->GetMeowClassName();
						}
					}
					where->LinkColumn(stringifiedLeftValue,op,stringifiedValue);
				}	
				
			}
			retval=(Meow::Filter_ptr)where;
			iCachedJoinSpecs->Set(relationshipname,(Core::ptr)retval);
		}
		else
			retval=(Meow::Filter_ptr)iCachedJoinSpecs->Get(relationshipname);

		
	} catch ( Exceptions::NotFound &nf )
	{
		Core::Log::GetLog()->Error("Meow::PersistentObject::JoinSpecToFilter","sss","Cannot return join filter for relationship ",relationshipname.c_str()," due to an exception caught for Persistence/Where");
	}
	return retval;	
}
unsigned long Meow::PersistentObject::CountAutoJoin( std::string otherclass, std::string publicattribute )
{
	unsigned long count=0;
	// So this is a standard filter we can work out already. We know that we need to conver the public attribute to a private fieldname
	// and that the value to search for on that fieldname (=) is our ToString reference. So we just need
	// to cast the other factory to a persistent object.
	PersistenceFactory_ptr otherFactory=(PersistenceFactory_ptr)Registry::GetRegistry()->FindFactory(otherclass + "Factory");
	SchemaField_ptr declaration=otherFactory->GetAttributeByAlias(publicattribute);
	if ( declaration.IsNull() )
		declaration=otherFactory->GetKeyAttributeByAlias(publicattribute);
	if ( !declaration.IsNull() )
	{
		std::string iReference=ToString();
		Persistence::Where_ptr filter=(Persistence::Where_ptr)Registry::GetRegistry()->GetMe("Persistence/Where");
		filter->Expression(declaration->iFieldName,"=",iReference);
		count=otherFactory->CountEnumerateWithFilter(otherclass,(Filter_ptr)filter);
	}
	return count;
}
Meow::Core::Types::Array_ptr Meow::PersistentObject::GetAutoJoin( std::string otherclass, std::string publicattribute )
{
	Core::Types::Array_ptr retval=new Core::Types::Array();
	PersistenceFactory_ptr otherFactory=(PersistenceFactory_ptr)Registry::GetRegistry()->FindFactory(otherclass + "Factory");
	SchemaField_ptr declaration=otherFactory->GetAttributeByAlias(publicattribute);
	if ( declaration.IsNull() )
		declaration=otherFactory->GetKeyAttributeByAlias(publicattribute);
	if ( !declaration.IsNull() )
	{
		std::string iReference=ToString();
		Persistence::Where_ptr filter=(Persistence::Where_ptr)Registry::GetRegistry()->GetMe("Persistence/Where");
		filter->Expression(declaration->iFieldName,"=",iReference);
		retval=otherFactory->EnumerateWithFilter(otherclass,(Filter_ptr)filter);
	}
	return retval;
}
Meow::Core::Types::Array_ptr Meow::PersistentObject::GetAutoJoinWithLimit( std::string otherclass, std::string publicattribute, int offset, int limit )
{
	Core::Types::Array_ptr retval=new Core::Types::Array();
	PersistenceFactory_ptr otherFactory=(PersistenceFactory_ptr)Registry::GetRegistry()->FindFactory(otherclass + "Factory");
	SchemaField_ptr declaration=otherFactory->GetAttributeByAlias(publicattribute);
	if ( declaration.IsNull() )
		declaration=otherFactory->GetKeyAttributeByAlias(publicattribute);
	if ( !declaration.IsNull() )
	{
		std::string iReference=ToString();
		Persistence::Where_ptr filter=(Persistence::Where_ptr)Registry::GetRegistry()->GetMe("Persistence/Where");
		filter->Expression(declaration->iFieldName,"=",iReference);
		retval=otherFactory->EnumerateWithFilterAndLimit(otherclass,(Filter_ptr)filter,offset,limit);
	}
	return retval;
}

void Meow::PersistentObject::ImageOrFileCopy(std::string sourceattribute, PersistentObject_ptr targetobj, std::string targetattribute)
{
	bool proceed=false;
	Core::Types::ValueType_ptr sourceattr = (Core::Types::ValueType_ptr)GetPublicVariable(sourceattribute);
	Core::ptr targetattr = targetobj->GetPublicVariable(targetattribute);
	SchemaField_ptr srcfield=(SchemaField_ptr)GetFactory()->GetAttributeByAlias(sourceattribute);
	SchemaField_ptr targetfield=(SchemaField_ptr)targetobj->GetFactory()->GetAttributeByAlias(targetattribute);

	//Inspect and compare the schema of source and target fields
	Core::Types::ValueType_ptr srcconfEntry;
	Core::Types::ValueType_ptr srcmask;
	Core::Types::ValueType_ptr srctypeDef;
	Core::Types::ValueType_ptr tgtconfEntry;
	Core::Types::ValueType_ptr tgtmask;
	Core::Types::ValueType_ptr tgttypeDef;
	Core::Types::ValueType_ptr srcwidth;
	Core::Types::ValueType_ptr srcheight;
	Core::Types::ValueType_ptr tgtwidth;
	Core::Types::ValueType_ptr tgtheight;
	switch ( srcfield->iColumnType )
	{
		case Factory::Image:
			if (targetfield->iColumnType == Factory::Image)
			{
				//Proceed and check for further compatibility
				//For Image type copy - Image can be defined as 3 or 5 components
				if ( srcfield->iEnumerationValues.length() > 0 )
				{
					Core::Types::String_ptr srcexplodeMe=new Core::Types::String(srcfield->iEnumerationValues);
					Core::Types::Array_ptr srccomponents=srcexplodeMe->Explode("|");
					if ( targetfield->iEnumerationValues.length() > 0 )
					{
						Core::Types::String_ptr trgexplodeMe=new Core::Types::String(targetfield->iEnumerationValues);
						Core::Types::Array_ptr tgtcomponents=trgexplodeMe->Explode("|");
						if ( srccomponents->Count() == tgtcomponents->Count() )
						{
							srcconfEntry=(Core::Types::String_ptr)srccomponents->Get(0);
							srcmask=(Core::Types::String_ptr)srccomponents->Get(1);
							srctypeDef=(Core::Types::String_ptr)srccomponents->Get(2);
							tgtconfEntry=(Core::Types::String_ptr)tgtcomponents->Get(0);
							tgtmask=(Core::Types::String_ptr)tgtcomponents->Get(1);
							tgttypeDef=(Core::Types::String_ptr)tgtcomponents->Get(2);
							if(srctypeDef->Equals(tgttypeDef))
							{
								if(srccomponents->Count() == 5)
								{
									//copy only if dimensions also match
									Core::Types::String_ptr srcwidth=(Core::Types::String_ptr)srccomponents->Get(3);
									Core::Types::String_ptr srcheight=(Core::Types::String_ptr)srccomponents->Get(4);
									Core::Types::String_ptr tgtwidth=(Core::Types::String_ptr)tgtcomponents->Get(3);
									Core::Types::String_ptr tgtheight=(Core::Types::String_ptr)tgtcomponents->Get(4);
									if((srcwidth->GetValueAsString() == tgtwidth->GetValueAsString()) && (srcheight->GetValueAsString() == tgtheight->GetValueAsString()))
									{
										//proceed and copy from srcconfEntry to tgtconfEntry using tgtmask
										proceed=true;
									}
								}
								else
								{
									//proceed and copy from srcconfEntry to tgtconfEntry using tgtmask
									proceed=true;
								}
							}
							else
								Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Incompatible Copy Attempt","Field type definition mismatch");

						}
						else
							Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Incompatible Copy Attempt","Field definition mismatch");
					}
				}

			}
			else
			{
				Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Incompatible Copy Attempt","source field(Image) and target field(Non-Image) are different types");
			}
			break;
		case Factory::File:
			if (targetfield->iColumnType == Factory::File)
			{
				//For File type copy - File can be defined as 3 components
				if ( srcfield->iEnumerationValues.length() > 0 )
				{
					Core::Types::String_ptr srcexplodeMe=new Core::Types::String(srcfield->iEnumerationValues);
					Core::Types::Array_ptr srccomponents=srcexplodeMe->Explode("|");
					if ( targetfield->iEnumerationValues.length() > 0 )
					{
						Core::Types::String_ptr trgexplodeMe=new Core::Types::String(targetfield->iEnumerationValues);
						Core::Types::Array_ptr tgtcomponents=trgexplodeMe->Explode("|");
						if ( srccomponents->Count() == tgtcomponents->Count() )
						{
							srcconfEntry=(Core::Types::String_ptr)srccomponents->Get(0);
							srcmask=(Core::Types::String_ptr)srccomponents->Get(1);
							srctypeDef=(Core::Types::String_ptr)srccomponents->Get(2);
							tgtconfEntry=(Core::Types::String_ptr)tgtcomponents->Get(0);
							tgtmask=(Core::Types::String_ptr)tgtcomponents->Get(1);
							tgttypeDef=(Core::Types::String_ptr)tgtcomponents->Get(2);
							if(srctypeDef->Equals(tgttypeDef))
							{
								//proceed and copy from srcconfEntry to tgtconfEntry using tgtmask
								proceed=true;
							}
						}
						else
							Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Incompatible Copy Attempt","Field definition mismatch");
					}
				}
			}
			else
			{
				Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Incompatible Copy Attempt","source field(File) and target field(Non-File) are different types");
			}
			break;
		default:
			Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Invalid Copy Attempt","source field is not of type image or file");
	}
	if(proceed == true)
	{
		std::string retval="";
			std::string path=Configuration::GetConfiguration()->Get("fs." + tgtconfEntry->GetValueAsString());
			Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Path check.");
			if ( path.length() == 0 )
			{
					//outcome="Storage path not declared " + tgtconfEntry->GetValueAsString();
					Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","sss","Path ",tgtconfEntry->GetValueAsString().c_str()," not defined.");
			}
			else
			{
				// OK so we need to tokenise
				std::string alias=tgtmask->GetValueAsString();
				std::string currentToken="";
				Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Working on mask:",alias.c_str());
				bool intoken=false;
				for ( unsigned long i=0; i<tgtmask->Count(); i++ )
				{
					char c=alias[i];


					Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","sd","Working on char:",c);
					switch (c)
					{
					case '[':
						if ( intoken )
						{
							Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Already in token, so treating this as a [ that we want outside the token.");
							retval.append("[" + currentToken);
						}
						else
						{
							Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Now in token");
							intoken=true;
							currentToken="";
						}
						break;
					case ']':
						if ( intoken )
						{
							Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","End of token:",currentToken.c_str());
							// get the current token and find the attribute by
							Meow::SchemaField_ptr field=targetobj->GetFactory()->GetAttributeByAlias(currentToken);
							if ( field.IsNull() )
							{

								field=targetobj->GetFactory()->GetKeyAttributeByAlias(currentToken);
							}
							if ( field.IsNull() )
							{
								Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Field was NULL");
								retval.append("[" + currentToken + "]");
							}
							else
							{
								Core::ptr value;
								if ( field->iFieldType==Factory::Key || field->iFieldType==Factory::Serial )
								{
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","sd","Get it from key component:",field->iOffset);
									value=targetobj->GetUniqueKey()->Get(field->iOffset);
								}
								else
								{
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","ss","Get it from attribute ",field->iFieldName.c_str());
									value=targetobj->GetVariable(field->iFieldName);
								}
								if ( value.IsNull() )
								{
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Its null.");
									retval.append("NULL");
								}
								else if ( value->IsInstanceOf("ValueType") )
								{
									Core::Types::ValueType_ptr valueAsPtr=(Core::Types::ValueType_ptr)value;
									retval.append(valueAsPtr->GetValueAsString());
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Its a value type.");
								}
								else if ( value->IsInstanceOf("LifecycleObject") )
								{
									LifecycleObject_ptr objectAsPtr=(LifecycleObject_ptr)value;
									retval.append(objectAsPtr->GetPublicAlias());
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Its a lifecycle object.");
								}
								else if ( value->IsInstanceOf("Object") )
								{
									Object_ptr objectAsPtr=(Object_ptr)value;
									retval.append(objectAsPtr->ToString());
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Its an object.");
								}
								else
								{
									Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Its a class name.");
									retval.append(value->GetMeowClassName());
								}
							}
							currentToken="";
							intoken=false;
						}
						break;
					default:
						Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Add character");
						if ( intoken )
							currentToken+=c;
						else
							retval+=c;
					}
				}
					if ( retval.length() == 0 )
						Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Could not parse mask");
					else
					{
						std::string targetfile=Configuration::GetConfiguration()->Get("fs." + tgtconfEntry->GetValueAsString()) + "/" + retval;
						std::string targetfileurl=Configuration::GetConfiguration()->Get("url." + tgtconfEntry->GetValueAsString()) + "/" + retval;
						std::string sourcefile=Configuration::GetConfiguration()->Get("fs." + srcconfEntry->GetValueAsString());

						Core::Types::String_ptr sourcefilename = new Core::Types::String(sourceattr->GetValueAsString());
						Core::Types::Array_ptr filename = sourcefilename->Explode("/");
						Core::Types::String_ptr srcfilename = (Core::Types::String_ptr)filename->Get(filename->Count()-1);
						sourcefile.append("/");
						sourcefile.append(srcfilename->GetValueAsString());

						Core::Storage::File_ptr file;
						bool output = file->CopyFile(sourcefile,targetfile);
						if ( output )
							Core::Log::GetLog()->Error("Meow::PersistentObject::ImageOrFileCopy","s","Successfully copied");

						targetobj->SetPublicVariable(targetfield->iPublicName,new Core::Types::String(targetfileurl));
						targetobj->Passivate();
					}

			}
	}

}

bool Meow::PersistentObject::Lock()
{
	bool retval = false;
	//Firstly check if there is a lock already and by whom
	Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
	Persistence::Select_ptr operation=(Persistence::Select_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Select");
	operation->SetFrom(database->SanitizeTableName(GetMeowClassName()));

	Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
	operation->SetWhere(filter);

	Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
	while ( !keys->IsEmpty() )
	{
		Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
		operation->AddColumn(key->GetValueAsString());
	}
	operation->AddColumn("__lockholder");
	operation->AddColumn("__lockheldsince");
	Persistence::SQLResult_ptr result=database->ObjectQuery((Persistence::Query_ptr)operation);
	int rowCounter=0;
	try
	{
		while ( true )
		{
			Core::Types::String_ptr lockholder = new Core::Types::String(result->FetchCellData(rowCounter,"__lockholder"));
			Core::Types::String_ptr lockheldsince = new Core::Types::String(result->FetchCellData(rowCounter,"__lockheldsince"));
			Core::Time::Date_ptr now=new Core::Time::Date();
			Security::User_ptr currentuser;
			Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
			if ( !session.IsNull() )
				currentuser=(Security::User_ptr)session->GetCurrentUser();
			if(lockholder->GetValueAsString() == "")
			{
				//Scenario 1 - No one has locked it currently go ahead and set the owner to current user
				Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","s","No Lockholder set yet");
				Persistence::Update_ptr updateoperation=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
				updateoperation->SetWhat(database->SanitizeTableName(GetMeowClassName()));
				updateoperation->SetWhere(filter);
				updateoperation->AddAssign("__lockholder",currentuser->ToString());
				updateoperation->AddAssign("__lockheldsince",now->GetValueAsString());
				Persistence::SQLResult_ptr upresult=database->ObjectQuery((Persistence::Query_ptr)updateoperation);
				return true;
			}
			else
			{
				if(!((lockholder->GetValueAsString()).compare(currentuser->ToString())))
				{
					//Scenario 2 - lockholder exists but owner is current user - in which update the lockheldsinceonly
					Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","s","Lockholder same as current ownner");
					Persistence::Update_ptr updateoperation=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
					updateoperation->SetWhat(database->SanitizeTableName(GetMeowClassName()));
					updateoperation->SetWhere(filter);
					updateoperation->AddAssign("__lockheldsince",now->GetValueAsString());
					Persistence::SQLResult_ptr upresult=database->ObjectQuery((Persistence::Query_ptr)updateoperation);
					return true;
				}
				else
				{

					//Scenario 3 - lockholder exists and some one else is the owner
					Core::Time::Date_ptr lockheldsinceasdate=(Core::Time::Date_ptr)lockheldsince;
					if(lockheldsinceasdate->GetValueAsInteger() < (now->GetValueAsInteger()-43200))
					{
						//3a - Lock is so old(12 hrs past) that it can be overwritten
						Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","s","more than 12 hours old lock so override");
						Persistence::Update_ptr updateoperation=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
						updateoperation->SetWhat(database->SanitizeTableName(GetMeowClassName()));
						updateoperation->SetWhere(filter);
						updateoperation->AddAssign("__lockholder",currentuser->ToString());
						updateoperation->AddAssign("__lockheldsince",now->GetValueAsString());
						Persistence::SQLResult_ptr upresult=database->ObjectQuery((Persistence::Query_ptr)updateoperation);
						return true;
					}
					else
					{
						//3b - Lock is currently in use so current user cannot gain a lock as of now
						Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","ssss","Could not obtain lock as its locked by someone else ",lockholder->GetValueAsString().c_str()," while current user is ",currentuser->ToString().c_str());
						return false;
					}
				}
			}
			rowCounter++;
		}
	} catch ( Exceptions::NotFound &nf )
	{
	}
	return false;
}
bool Meow::PersistentObject::UnLock()
{
	bool retval = false;
	//Firstly check if there is a lock already and by whom
	Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
	Persistence::Select_ptr operation=(Persistence::Select_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Select");
	operation->SetFrom(database->SanitizeTableName(GetMeowClassName()));

	Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
	operation->SetWhere(filter);

	Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
	while ( !keys->IsEmpty() )
	{
		Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
		operation->AddColumn(key->GetValueAsString());
	}
	operation->AddColumn("__lockholder");
	operation->AddColumn("__lockheldsince");
	Persistence::SQLResult_ptr result=database->ObjectQuery((Persistence::Query_ptr)operation);
	int rowCounter=0;
	try
	{
		while ( true )
		{
			Core::Types::String_ptr lockholder = new Core::Types::String(result->FetchCellData(rowCounter,"__lockholder"));
			Core::Types::String_ptr lockheldsince = new Core::Types::String(result->FetchCellData(rowCounter,"__lockheldsince"));

			Core::Time::Date_ptr now=new Core::Time::Date();
			Security::User_ptr currentuser;
			Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
			if ( !session.IsNull() )
				currentuser=(Security::User_ptr)session->GetCurrentUser();
			if(lockholder->GetValueAsString() == "")
			{
				//Scenario 1 - No one has locked it so cannot attempt to unlock but we will return true here so that if someone was doing a safety check they know its unlocked!
				Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","s","No Lockholder set yet so cannot unlock");
				return true;
			}
			else
			{
				if(!((lockholder->GetValueAsString()).compare(currentuser->ToString())))
				{
					//Scenario 2 - lockholder exists and owner is current user - so can unlock
					Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","s","Lockholder same as current ownner");
					Persistence::Update_ptr updateoperation=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
					updateoperation->SetWhat(database->SanitizeTableName(GetMeowClassName()));
					updateoperation->SetWhere(filter);
					updateoperation->AddAssign("__lockholder","");
					updateoperation->AddAssign("__lockheldsince","0");
					Persistence::SQLResult_ptr upresult=database->ObjectQuery((Persistence::Query_ptr)updateoperation);
					return true;
				}
				else
				{
					//Scenario 3 - lockholder exists and some one else is the owner so not permitted to unlock
					Core::Log::GetLog()->Error("Meow::PersistentObject::Lock","s","Owned by someone else so cannot unlock");
					return false;

				}
			}
			rowCounter++;
		}
	} catch ( Exceptions::NotFound &nf )
	{
	}
	return false;
}
Meow::Core::Types::Array_ptr Meow::PersistentObject::GetLockInfo()
{
	Core::Types::Array_ptr retval=new Core::Types::Array();
	Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
	Persistence::Select_ptr operation=(Persistence::Select_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Select");
	operation->SetFrom(database->SanitizeTableName(GetMeowClassName()));

	Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
	operation->SetWhere(filter);

	Core::Types::Stack_ptr keys=GetFactory()->GetSchemeAttributes()->GetKeys();
	while ( !keys->IsEmpty() )
	{
		Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
		operation->AddColumn(key->GetValueAsString());
	}
	operation->AddColumn("__lockholder");
	operation->AddColumn("__lockheldsince");
	Persistence::SQLResult_ptr result=database->ObjectQuery((Persistence::Query_ptr)operation);
	int rowCounter=0;
	try
	{
		while ( true )
		{
			Core::Types::String_ptr lockholder = new Core::Types::String(result->FetchCellData(rowCounter,"__lockholder"));
			Core::Types::String_ptr lockheldsince = new Core::Types::String(result->FetchCellData(rowCounter,"__lockheldsince"));
			retval->Set("LockHolder",(Core::ptr)lockholder);
			retval->Set("LockHeldSince",(Core::ptr)lockheldsince);
			rowCounter++;
		}
	} catch ( Exceptions::NotFound &nf )
	{
	}
	return retval;
}
void Meow::PersistentObject::Available( ) 
{
	// If its a serial, then we use the first value from GetUniqueKey
	bool success=false;
	if ( GetFactory()->GetSchemeAttributes()->Count() > 0 )
	{
		
		std::string diagnosticMessage="";
		try
		{
			Persistence::Database_ptr database=(Persistence::Database_ptr)GetPersistenceDescriptor();
			// We got a database, so lets try and generate the query.
			Persistence::Update_ptr operation=(Persistence::Update_ptr)GetFactory()->GetRegistry()->GetMe("Persistence/Update");
			operation->SetWhat(database->SanitizeTableName(GetMeowClassName()));
			Persistence::Where_ptr filter=(Persistence::Where_ptr)GetFilter();
			operation->SetWhere(filter);
		
			// Be strict about handling returns from functions.
			Persistence::SQLResult_ptr result;
			Core::Log::GetLog()->Error("operation->AddAssign","ss","__available=","1");
			operation->AddAssign("__available","1");

			Core::Time::Date_ptr now=new Core::Time::Date();
			operation->AddAssign("___changed",now->GetValueAsString());
			// DS - Add the security user detection into ___user
			Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
			if ( !session.IsNull() )
			{
				Security::User_ptr user=(Security::User_ptr)session->GetCurrentUser();
				if ( !user.IsNull() )
				{
					operation->AddAssign("___user",user->ToString());
				}
			}
			result=database->ObjectQuery((Persistence::Query_ptr)operation);	
			PostUpdated();
			MakeBeforeImage();
			ClearDirty();
			success=true;
		} catch ( Exception &e )
		{
			diagnosticMessage=e.Diagnostic();
		}
		// If we failed for any reason to execute the deletion, throw a rollback exception.
		if ( !success )
		{
			Exceptions::Rollback rb;
			rb.SetMessage(rb.Diagnostic());
			throw rb;
		}
	}

}
