#include <LifecycleObject.h>
#include <Factory.h>
#include <Core/System/Thread.h>
#include <Core/Log.h>
#include <iostream>
#include <Core/Types/String.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>
#include <Core/Time/Date.h>
#include <Core/Types/Queue.h>
#include <Core/Types/Stack.h>
#include <Core/Types/Boolean.h>
#include <inc/configuration.h>

Meow::LifecycleObject::LifecycleObject() :
    iIsDirty(false),
    iReadRequired(false)
{
    SetMeowClassName("LifecycleObject");
}

Meow::LifecycleObject::~LifecycleObject()
{
    if ( iIsDirty )
        Passivate( );
}

void Meow::LifecycleObject::__Destroy( )
{
}
bool Meow::LifecycleObject::IsPurgeable( )
{
		return false;
}
bool Meow::LifecycleObject::Getter( )
{
	bool retval=true;
    try
    {
        if ( IsReadRequired() )
            Restore();
    } catch ( Meow::Exception &e )
    {
    	Core::Log::GetLog()->Error(GetURL(),"sss","Error in Getter(), exception caught and absorbed [",e.Diagnostic().c_str(),"]");
    	retval=false;
    }
    return retval;
}
bool Meow::LifecycleObject::Setter( )
{
	bool retval=true;
	try
	{
	   if ( IsReadRequired() )
	       Restore();
	} catch ( Meow::Exception &e )
	{
	   Core::Log::GetLog()->Error(GetURL(),"sss","Error in Setter(), exception caught and absorbed [",e.Diagnostic().c_str(),"]");
	   retval=false;
	}
	return retval;
}
void Meow::LifecycleObject::_Delete( )
{

}
void Meow::LifecycleObject::Create( )
{
	// If its a serial type then we set our own unique key internally.
	// If its a key type, then we get our values from the key and throw Terminate if the key is not set up in a type-safe fashion
	// and has as many values as key components.
	// If its neither, throw terminate, this class does not deal with non key based object storage.
}
void Meow::LifecycleObject::Restore( )
{
	// Set up the where clause as per delete, and then retrieve the components from the data.
}
void Meow::LifecycleObject::RestoreFromAttributes( Core::Types::Array_ptr attributes )
{
	// Just restore from attributes.
}
void Meow::LifecycleObject::Passivate( )
{
	PostUpdated();
	MakeBeforeImage();
	// Update using the generated where clause.
}

void Meow::LifecycleObject::ReadRequired( )
{
    iReadRequired=true;
}

bool Meow::LifecycleObject::IsReadRequired( )
{
    return iReadRequired;
}

void Meow::LifecycleObject::ClearReadRequired( )
{
    iReadRequired=false;
}

void Meow::LifecycleObject::MarkDirty( )
{
    iIsDirty=true;
}
bool Meow::LifecycleObject::IsDirty( )
{
    bool retval=false;
    retval=iIsDirty;
    return retval;
}

void Meow::LifecycleObject::ClearDirty( )
{
    iIsDirty=false;
}


Meow::Factory_ptr Meow::LifecycleObject::GetFactory( )
{
    return iFactory;
}
std::string  Meow::LifecycleObject::GetPersistenceSpecification()
{
    return "";
}
void        Meow::LifecycleObject::SetVariable( std::string varname, Core::ptr data ) 
{
	if ( !GetFactory().IsNull() )
		GetFactory()->CanWrite();
	Setter();
    Object::SetVariable(varname,data);
}
Meow::Core::ptr Meow::LifecycleObject::GetVariable( std::string varname )
{
	if ( !GetFactory().IsNull() )
		GetFactory()->CanRead();
	return Object::GetVariable(varname);
}
bool Meow::LifecycleObject::ExistsVariable( std::string varname ) throw (Exceptions::Security)
{
    bool retval=false;
    if ( !GetFactory().IsNull() )
    	GetFactory()->CanRead();
    return Object::ExistsVariable(varname);
}
Meow::Core::Types::Stack_ptr Meow::LifecycleObject::GetVariableKeys() throw
							 (Exceptions::Security)
{
	Meow::Core::Types::Stack_ptr stack=new Core::Types::Stack();
	if ( !GetFactory().IsNull() )
		GetFactory()->CanRead();
	stack=Object::GetVariableKeys();
	return stack;
}
Meow::Core::Types::Array_ptr Meow::LifecycleObject::GetAttributeImage()
{
	Core::Types::Array_ptr image=new Core::Types::Array();
	// So we can get the normal stuff this way (the attributes) but we also need the key fields.
	// which we can only find out by querying the factory.
	Core::Types::Stack_ptr varKeys=GetVariableKeys();
	Factory_ptr myFactory=GetFactory();
	Core::Types::Array_ptr schemeAttributes=myFactory->GetSchemeAttributes();
	
	while ( !varKeys->IsEmpty() )
	{
		Core::Types::ValueType_ptr varKey=(Core::Types::ValueType_ptr)varKeys->Pop();				
		SchemaField_ptr field=(SchemaField_ptr)schemeAttributes->Get(varKey->GetValueAsString());
		if ( !field.IsNull() )
		{
			image->Set(field->iPublicName,GetVariable(varKey->GetValueAsString()));	
		}
		
	}
	if ( GetUniqueKey()->Count()>0 )
	{
		
		if ( !myFactory.IsNull() )
		{
			Core::Types::Array_ptr schemeKeys=myFactory->GetSchemeKeys();
			for ( int i=0; i<schemeKeys->Count();i++ )
			{
				SchemaField_ptr field=(SchemaField_ptr)schemeKeys->Get(i);
				image->Set(field->iPublicName,GetUniqueKey()->Get(i));
			}
		
		}
	}
	image->Set("_ObjectReference",new Core::Types::String(ToString()));
	return image;
}
void Meow::LifecycleObject::MakeBeforeImage()
{
	iBeforeImage=GetAttributeImage();
}
void Meow::LifecycleObject::PostUpdated()
{
	Factory_ptr factory=GetFactory();
	if ( !factory.IsNull() )
	{
		Core::Types::Array_ptr afterImage=GetAttributeImage();
		factory->PostUpdateEvents( iBeforeImage, afterImage);
		iBeforeImage=afterImage;
	}
}
std::string Meow::LifecycleObject::GetPublicAlias()
{
	std::string retval;
	if ( GetFactory()->GetAliasFormat().length() > 0 )
	{
		std::string alias=GetFactory()->GetAliasFormat();
		// So now we need to tokenise.
		unsigned long length=alias.length();
		std::string currentToken;
		bool intoken=false;
		for ( unsigned long i=0; i<length; i++ )
		{
			char c=alias[i];
			switch (c)
			{
			case '[':
				if ( intoken )
					retval.append("[" + currentToken);
				intoken=true;
				currentToken="";
				break;
			case ']':
				if ( intoken )
				{
					// get the current token and find the attribute by 
					Meow::SchemaField_ptr field=GetFactory()->GetAttributeByAlias(currentToken);
					if ( field.IsNull() )
					{
						field=GetFactory()->GetKeyAttributeByAlias(currentToken);
						if ( field.IsNull() )
						{
							retval.append("[" + currentToken + "]");
						}
						else
						{
							Core::ptr value;
							if ( field->iFieldType==Factory::Key || field->iFieldType==Factory::Serial )
							{
								value=GetUniqueKey()->Get(field->iOffset);	
							}
							else
							{
								value=GetVariable(field->iFieldName);
							}
							if ( value.IsNull() )
							{
								retval.append("NULL");
							}
							else if ( value->IsInstanceOf("ValueType") )
							{
								Core::Types::ValueType_ptr valueAsPtr=(Core::Types::ValueType_ptr)value;
								retval.append(valueAsPtr->GetValueAsString());
							}
							else if ( value->IsInstanceOf("LifecycleObject") )
							{
								LifecycleObject_ptr objectAsPtr=(LifecycleObject_ptr)value;
								retval.append(objectAsPtr->GetPublicAlias());
							}
							else if ( value->IsInstanceOf("Object") )
							{	
								Object_ptr objectAsPtr=(Object_ptr)value;
								retval.append(objectAsPtr->ToString());
							}
							else
							{
								retval.append(value->GetMeowClassName());
							}
						}
					}
					else
					{
						Core::ptr value;
						if ( field->iFieldType==Factory::Key )
						{
							value=GetUniqueKey()->Get(field->iOffset);	
						}
						else
						{
							value=GetVariable(field->iFieldName);
						}
						if ( value.IsNull() )
						{
							retval.append("NULL");
						}
						else if ( value->IsInstanceOf("ValueType") )
						{
							Core::Types::ValueType_ptr valueAsPtr=(Core::Types::ValueType_ptr)value;
							retval.append(valueAsPtr->GetValueAsString());
						}
						else if ( value->IsInstanceOf("LifecycleObject") )
						{
							LifecycleObject_ptr objectAsPtr=(LifecycleObject_ptr)value;
							retval.append(objectAsPtr->GetPublicAlias());
						}
						else if ( value->IsInstanceOf("Object") )
						{	
							Object_ptr objectAsPtr=(Object_ptr)value;
							retval.append(objectAsPtr->ToString());
						}
						else
						{
							retval.append(value->GetMeowClassName());
						}
					}
					currentToken="";
					intoken=false;
				}
				break;
			default:
				if ( intoken )
					currentToken+=c;
				else
					retval+=c;
			}
		}
	}	
	else
		retval=GetURL();
    return retval;
}
std::string Meow::LifecycleObject::GetURL()
{
    return ToString();
}
std::string Meow::LifecycleObject::ToXML()
{
	// return "<" + GetMeowClassName() + "/>";
	Core::Types::String_ptr classname=new Core::Types::String(GetMeowClassName());
	Core::Types::ValueType_ptr classnameSafe=classname->Replace("/","_");
	std::string retval="<" + classnameSafe->GetValueAsString() + ">";
	retval+="<PublicAlias>" + GetPublicAlias() + "</PublicAlias>";	
	retval+="<Ref>" + ToString() + "</Ref>";
	Core::Types::Array_ptr attributes=GetFactory()->GetAttributes();
	Core::Types::Stack_ptr keys=attributes->GetKeys();
	while ( !keys->IsEmpty() )	
	{
		Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keys->Pop();
		
		SchemaField_ptr iField=(SchemaField_ptr)attributes->Get(key->GetValueAsString());	
		retval+="<" + iField->iPublicName + " type=\"";
		Core::ptr value=GetVariable(iField->iFieldName);
		switch(iField->iColumnType)	
		{	
		case Factory::Object:
			retval+="Object\">";
			if ( !value.IsNull() )
				retval+=Core::Types::ValueType::Encode64(((LifecycleObject_ptr)value)->ToString());	
			break;
		case Factory::String:
			retval+="String\">";
			if ( !value.IsNull() )
				retval+=Core::Types::ValueType::Encode64(((Core::Types::String_ptr)value)->GetValueAsString());	
			break;
		case Factory::Integer:
			retval+="Integer\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::Integer_ptr)value)->GetValueAsString());	
			break;
		case Factory::Float:
			retval+="Float\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::Float_ptr)value)->GetValueAsString());	
			break;
		case Factory::Boolean:
			retval+="Boolean\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::Boolean_ptr)value)->GetValueAsString());
			break;
		case Factory::Date:
			retval+="Date\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Time::Date_ptr)value)->Format(Configuration::GetConfiguration()->Get("time.format")));
			break;
		case Factory::Array:
			retval+="Array\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::Array_ptr)value)->GetValueAsString());
			break;
		case Factory::Queue:
			retval+="Queue\">";
		//	if ( !value.IsNull() )	
		//		retval+=((Core::Types::Queue_ptr)value)->GetValueAsString();
			break;
		case Factory::Stack:
			retval+="Stack\">";
		//	if ( !value.IsNull() )	
		//		retval+=((Core::Types::Stack_ptr)value)->GetValueAsString();
			break;
		case Factory::Void:	
			retval+="Void\">";
		//	if ( !value.IsNull() )	
		//		retval+=((Core::Types::Void_ptr)value)->GetValueAsString();
			
			break;
		case Factory::ValueType:
			retval+="ValueType\">";	
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());
			
			break;
		case Factory::Enumeration:
			retval+="Enumeration\">";		
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());
			
			break;
		case Factory::Password:
			retval+="Password\">";
			break;
		case Factory::Text:
			retval+="Text\">";	
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());
			break;
		case Factory::HTML:
			retval+="HTML\">";	
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());
			break;
		case Factory::XML:	
			retval+="XML\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());			
			break;
		case Factory::Image:
			retval+="Image\">";	
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());
			
			break;
		case Factory::File:
			retval+="File\">";
			if ( !value.IsNull() )	
				retval+=Core::Types::ValueType::Encode64(((Core::Types::ValueType_ptr)value)->GetValueAsString());			
			break;
		}
		
		retval+="</" + iField->iPublicName + ">"; 
	}
	retval+="</" + classnameSafe->GetValueAsString() + ">";
	return retval;
}
std::string Meow::LifecycleObject::Validate()
{
	std::string retval;
	std::string errorxml;
	std::string outcome = "Success";
	errorxml.append("<Errors>");


	retval.append("<ObjectValidationResult ref=\"");
	retval.append(ToString());
	retval.append("\">");
	try
	{
		Core::Types::Array_ptr schemekeys=GetFactory()->GetSchemeKeys();
		Core::Types::Stack_ptr keyfields=schemekeys->GetKeys();
		while ( !keyfields->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keyfields->Pop();
			SchemaField_ptr iFieldKey=(SchemaField_ptr)schemekeys->Get(key->GetValueAsString());
			Core::Types::ValueType_ptr fieldval = (Core::Types::ValueType_ptr)GetVariable(iFieldKey->iFieldName);
			std::string result;
			if (iFieldKey->iFieldType == Factory::Serial )
				result = "Success";
			else
				result = GetFactory()->AttributeFullValidation(iFieldKey->iFieldName,(Core::ptr)fieldval);
			if(result.compare("Success"))
			{
				outcome="Failure";
				errorxml.append("<Error>");
				errorxml.append("<FieldName>");
				errorxml.append(iFieldKey->iFieldName);
				errorxml.append("</FieldName>");
				errorxml.append("<FieldType>");
				//If filling out FieldType appears to be redundant here. it can be removed
				errorxml.append("<FieldType>");
				switch ( iFieldKey->iColumnType )
				{
				case Meow::Factory::Object:
					errorxml.append("Object");
					break;
				case Meow::Factory::String:
					errorxml.append("String");
					break;
				case Meow::Factory::Text:
					errorxml.append("Text");
					break;
				case Meow::Factory::HTML:
					errorxml.append("HTML");
					break;
				case Meow::Factory::XML:
					errorxml.append("XML");
					break;
				case Meow::Factory::Password:
					errorxml.append("Password");
					break;
				case Meow::Factory::Integer:
					errorxml.append("Integer");
					break;
				case Meow::Factory::Float:
					errorxml.append("Float");
					break;
				case Meow::Factory::Boolean:
					errorxml.append("Boolean");
					break;
				case Meow::Factory::Date:
					errorxml.append("Date");
					break;
				case Meow::Factory::Array:
					errorxml.append("Array");
					break;
				case Meow::Factory::Queue:
					errorxml.append("Queue");
					break;
				case Meow::Factory::Stack:
					errorxml.append("Stack");
					break;
				case Meow::Factory::Void:
					errorxml.append("Void");
					break;
				case Meow::Factory::ValueType:
					errorxml.append("ValueType");
					break;
				case Meow::Factory::Enumeration:
					errorxml.append("Enumeration");
					break;
				case Meow::Factory::Image:
					errorxml.append("Image");
					break;
				case Meow::Factory::File:
					errorxml.append("File");
					break;
				}
				errorxml.append("</FieldType>");
				errorxml.append("<FieldValue>");
				errorxml.append(fieldval->GetValueAsString());
				errorxml.append("</FieldValue>");
				errorxml.append("<ErrorReason>");
				errorxml.append(result.replace(0,8,""));
				errorxml.append("</ErrorReason>");
				errorxml.append("</Error>");
			}
		}

		Core::Types::Array_ptr schemeattr=GetFactory()->GetSchemeAttributes();
		Core::Types::Stack_ptr attrfields=schemeattr->GetKeys();
		while ( !attrfields->IsEmpty() )
		{
			Core::Types::ValueType_ptr attrkey=(Core::Types::ValueType_ptr)attrfields->Pop();
			SchemaField_ptr iFieldAttr=(SchemaField_ptr)schemeattr->Get(attrkey->GetValueAsString());
			Core::Types::ValueType_ptr fieldval = (Core::Types::ValueType_ptr)GetVariable(iFieldAttr->iFieldName);
			std::string result = GetFactory()->AttributeFullValidation(iFieldAttr->iFieldName,(Core::ptr)fieldval);
			if(result.compare("Success"))
			{
				outcome="Failure";
				errorxml.append("<Error>");
				errorxml.append("<FieldName>");
				errorxml.append(iFieldAttr->iFieldName);
				errorxml.append("</FieldName>");
				//If filling out FieldType appears to be redundant here. it can be removed
				errorxml.append("<FieldType>");
				switch ( iFieldAttr->iColumnType )
				{
				case Meow::Factory::Object:
					errorxml.append("Object");
					break;
				case Meow::Factory::String:
					errorxml.append("String");
					break;
				case Meow::Factory::Text:
					errorxml.append("Text");
					break;
				case Meow::Factory::HTML:
					errorxml.append("HTML");
					break;
				case Meow::Factory::XML:
					errorxml.append("XML");
					break;
				case Meow::Factory::Password:
					errorxml.append("Password");
					break;
				case Meow::Factory::Integer:
					errorxml.append("Integer");
					break;
				case Meow::Factory::Float:
					errorxml.append("Float");
					break;
				case Meow::Factory::Boolean:
					errorxml.append("Boolean");
					break;
				case Meow::Factory::Date:
					errorxml.append("Date");
					break;
				case Meow::Factory::Array:
					errorxml.append("Array");
					break;
				case Meow::Factory::Queue:
					errorxml.append("Queue");
					break;
				case Meow::Factory::Stack:
					errorxml.append("Stack");
					break;
				case Meow::Factory::Void:
					errorxml.append("Void");
					break;
				case Meow::Factory::ValueType:
					errorxml.append("ValueType");
					break;
				case Meow::Factory::Enumeration:
					errorxml.append("Enumeration");
					break;
				case Meow::Factory::Image:
					errorxml.append("Image");
					break;
				case Meow::Factory::File:
					errorxml.append("File");
					break;
				}
				errorxml.append("</FieldType>");
				errorxml.append("<FieldValue>");
				errorxml.append(fieldval->GetValueAsString());
				errorxml.append("</FieldValue>");
				errorxml.append("<ErrorReason>");
				errorxml.append(result.replace(0,8,""));
				errorxml.append("</ErrorReason>");
				errorxml.append("</Error>");
			}
		}

	}
	catch(...)
	{
		Core::Log::GetLog()->Error("Meow::LifecycleObject::Validate","s","Something went wrong");
	}

	errorxml.append("</Errors>");

	retval.append("<Outcome>");
	retval.append(outcome);
	retval.append("</Outcome>");
	retval.append(errorxml);
	retval.append("</ObjectValidationResult>");

	return retval;

}
bool Meow::LifecycleObject::QuickValidate()
{
	try
	{
		Core::Types::Array_ptr schemekeys=GetFactory()->GetSchemeKeys();
		Core::Types::Stack_ptr keyfields=schemekeys->GetKeys();
		while ( !keyfields->IsEmpty() )
		{
			Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)keyfields->Pop();
			SchemaField_ptr iFieldKey=(SchemaField_ptr)schemekeys->Get(key->GetValueAsString());
			Core::Types::ValueType_ptr fieldval = (Core::Types::ValueType_ptr)GetVariable(iFieldKey->iFieldName);
			//bool result = GetFactory()->AttributeValidation(iFieldKey->iFieldName,(Core::ptr)fieldval);
			bool result;
			if (iFieldKey->iFieldType == Factory::Serial )
				result = true;
			else
				result = GetFactory()->AttributeValidation(iFieldKey->iFieldName,(Core::ptr)fieldval);
			if(!result)
				return false;
		}

		Core::Types::Array_ptr schemeattr=GetFactory()->GetSchemeAttributes();
		Core::Types::Stack_ptr attrfields=schemeattr->GetKeys();
		while ( !attrfields->IsEmpty() )
		{
			Core::Types::ValueType_ptr attrkey=(Core::Types::ValueType_ptr)attrfields->Pop();
			SchemaField_ptr iFieldAttr=(SchemaField_ptr)schemeattr->Get(attrkey->GetValueAsString());
			Core::Types::ValueType_ptr fieldval = (Core::Types::ValueType_ptr)GetVariable(iFieldAttr->iFieldName);
			bool result = GetFactory()->AttributeValidation(iFieldAttr->iFieldName,(Core::ptr)fieldval);
			if(!result)
				return false;
		}

	}
	catch(...)
	{
		Core::Log::GetLog()->Error("Meow::LifecycleObject::QuickValidate","s","Something went wrong");
		return false;
	}
	return true;

}

Meow::Core::Types::Array_ptr Meow::LifecycleObject::GetCallableMethods( )
{
	Core::Types::Array_ptr iMethodList=new Core::Types::Array();
	Core::Types::Stack_ptr keys=GetFactory()->GetCallableMethodList()->GetKeys();
	while ( !keys->IsEmpty() )
	{
		Core::Types::ValueType_ptr methodName=(Core::Types::ValueType_ptr)keys->Pop();
		
		iMethodList->Set(methodName->GetValueAsString(),GetFactory()->GetCallableMethodList()->Get(methodName->GetValueAsString()));
	}
	return iMethodList;
}
void    Meow::LifecycleObject::BeginTransaction()
{
}
void    Meow::LifecycleObject::CommitTransaction()
{
}
void    Meow::LifecycleObject::RollbackTransaction()
{
}
