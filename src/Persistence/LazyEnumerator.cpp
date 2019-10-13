#include <Persistence/LazyEnumerator.h>
#include <Core/Types/Integer.h>

Meow::Persistence::LazyEnumerator::LazyEnumerator()
{
    iLazyClassName="";
    iLazyKeyFields=new Core::Types::Array();
    iLazyAttributeFields=new Core::Types::Array();
    iCachedItemCount=0;
    SetMeowClassName("Persistence/LazyEnumerator");
}
Meow::Persistence::LazyEnumerator::~LazyEnumerator()
{
    if ( !iResult.IsNull() )
        iResult->Release();
}
Meow::Core::ptr  Meow::Persistence::LazyEnumerator::Get( unsigned long key )
{
	Core::Types::ValueType_ptr keyAsString=new Core::Types::Integer(key);
    Meow::Core::ptr retval;

    if ( Core::Types::Array::IsSet(key) )
        return Core::Types::Array::Get(key);
    

// Use the details to locate the result
// but we need to convert 
    Core::Types::Array_ptr keyPtr=new Core::Types::Array();
    for ( int i=0; i<iLazyKeyFields->Count(); i++ )
    {
    	SchemaField_ptr val=(SchemaField_ptr)iLazyKeyFields->Get(i);
    	Core::ptr keyComponent=(Core::ptr)iResult->FetchCellDataAsValue(key,val->iFieldName);
    	// ARRRGH we need to do this differently
	if ( val->iColumnType == Factory::Object )
	{
		// Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","s","Its an object!");
		Core::Types::ValueType_ptr keyComponentAsString=(Core::Types::ValueType_ptr)keyComponent;
		// Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Reference:",keyComponentAsString->GetValueAsString().c_str());
			
		if ( keyComponentAsString->GetValueAsString().length()>0 )
		{
			try
			{
				// Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","s","Calling find");
			
				keyComponent=(Core::ptr)Registry::GetRegistry()->FindMeByObjectReference(keyComponentAsString->GetValueAsString());
				// Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Found:",((LifecycleObject_ptr)keyComponent)->ToString().c_str());
	
			} catch ( Meow::Exceptions::NotFound &nf )
			{
				Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Exception:",nf.Diagnostic().c_str());
			}
		}
		else
		{
			// Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","s","Empty reference");
			keyComponent=Core::Object::Null();
		}
	}	
	// else
	// 	Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","s","Not an object");
	// Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","sd","Set key reference:",i);			
	keyPtr->Set((unsigned long)i,(Core::ptr)keyComponent);
    }
    // Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Key is:",keyPtr->GetValueAsString().c_str());
    if ( iLazyAttributeFields->Count() == 0 )
    {
    	try
	{
	    retval=Meow::Registry::GetRegistry()->FindMe(iLazyClassName,keyPtr);
	} catch ( Exception &e )
        {
        	Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Exception:",e.Diagnostic().c_str());
        }
    }
    else
    {
    	 /* virtual LifecycleObject_ptr FindByPrimaryKeyOrRestore( Core::Types::Array_ptr key, Core::Types::Array_ptr attributes )
			throw(Exceptions::NotFound,
					Exceptions::Rollback,
					Exceptions::Terminate,
					Exceptions::Security);
		 */
	Core::Types::Array_ptr attributes=new Core::Types::Array();
	for ( int i=0; i<iLazyAttributeFields->Count(); i++ )
	{
		SchemaField_ptr val=(SchemaField_ptr)iLazyAttributeFields->Get(i);
		attributes->Set(val->iPublicName,(Core::ptr)iResult->FetchCellDataAsValue(key,val->iFieldName));
	}
        try
        {
        	Factory_ptr factory=Meow::Registry::GetRegistry()->FindFactory(iLazyClassName + "Factory");
        // 	Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ssss","Calling FindByPrimaryKeyOrRestore on:",iLazyClassName.c_str()," with key ",keyPtr->GetValueAsString().c_str()); 
        	retval=factory->FindByPrimaryKeyOrRestore(keyPtr,attributes);
       //  	if ( retval.IsNull() )
        // 		Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","s","Returned NULL");
        // 	else
        // 		Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Returned:",((LifecycleObject_ptr)retval)->ToString().c_str());
        } catch ( Exception &e )
        {        	
        	Core::Log::GetLog()->Error("Meow::Persistence::LazyEnumerator::Get","ss","Exception:",e.Diagnostic().c_str());
        }
    }
    return retval;
}
void Meow::Persistence::LazyEnumerator::SetLazyClassName( std::string classname )
{
    iLazyClassName=classname;
}
void Meow::Persistence::LazyEnumerator::SetLazyKeyFields( Core::Types::Array_ptr keyFields )
{
    iLazyKeyFields=keyFields;
}
void Meow::Persistence::LazyEnumerator::SetLazyAttributeFields( Core::Types::Array_ptr attributeFields )
{
    iLazyAttributeFields=attributeFields;
}
			
void Meow::Persistence::LazyEnumerator::SetResultObject( Meow::Persistence::SQLResult_ptr result )
{
    iResult=result;
}
void Meow::Persistence::LazyEnumerator::OverrideCount( unsigned long counter )
{
    iCachedItemCount=counter;
}
unsigned long Meow::Persistence::LazyEnumerator::Count( )
{
    return iCachedItemCount;
}
std::string	Meow::Persistence::LazyEnumerator::GetValueAsString()
{
    // We need the CoreArray scheme here
    std::string retval;
    iCachedItemCount=0;
    retval="<Array subtype=\"lazy\">\n";
    try
    {
        int i=0;
        while (true )
        {
			iCachedItemCount++;
            std::string treatas="";
            std::string realvalue="";
            Core::ptr CurrentItem=Get(i);
            if ( CurrentItem.IsNull() )
            {
                treatas="Null";
                realvalue="0x0";
            }
            else if ( CurrentItem->IsInstanceOf("Browser/Input/HTTPFile") )
            {
                treatas="Null";
                realvalue="#PRIVATE#";
            }
            else if ( CurrentItem->IsInstanceOf("Array") )
            {
                // Fix for array of arrays
                treatas="Array";
                Core::Types::Array_ptr arrayOfData=(Core::Types::Array_ptr)CurrentItem;
                realvalue=arrayOfData->GetValueAsString();
            }
            else if ( CurrentItem->IsInstanceOf("ValueType") )
            {
                treatas="ValueType";
                Core::Types::ValueType_ptr value=(Core::Types::ValueType_ptr)CurrentItem;
                realvalue=value->GetValueAsString();
            }
            else if ( CurrentItem->IsInstanceOf("LifecycleObject"))
            {
                treatas="LifecycleObject";
                LifecycleObject_ptr value=(LifecycleObject_ptr)CurrentItem;
                Core::Types::Array_ptr keyval=value->GetUniqueKey();
                if ( !keyval.IsNull() )
                {
                    for ( int i=0; i<keyval->Count(); i++ )
                    {
                    	Core::Types::ValueType_ptr keypart=(Core::Types::ValueType_ptr)keyval->Get(i);
                        if ( i>0 )
                            realvalue.append("/");
                        realvalue.append(keypart->GetValueAsString());
                    }
                }
                else
                {
                    treatas="Null";
                    realvalue="0x0";
                }
            }
            else if ( CurrentItem->IsInstanceOf("CoreObject") )
            {
                treatas="CoreObject";
                Core::ptr value=(Core::ptr)CurrentItem;
                realvalue=value->GetMeowClassName();
            }
            else
            {
                treatas="Null";
                realvalue="#PRIVATE#";
            }
            Core::Types::Integer_ptr intval=new Core::Types::Integer(i);
            retval=retval + "<Element key=\"" + intval->GetValueAsString() +"\" treatas=\"" + treatas + "\" id=\"" + intval->GetValueAsString() + "\" class=\"";
            if ( !CurrentItem.IsNull() )
            {
                retval=retval + CurrentItem->GetMeowClassName() + "\">" + realvalue + "</Element>\n";
            }
            else
            {
                retval=retval + "invalid" + "\">" + realvalue + "</Element>\n";
            }
            i++;
        }
    } catch ( ... )
    {
        // End of array
    }
    retval=retval + "</Array>\n";
    return retval;
}
