/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _LIFECYCLEOBJECT_H_
#define _LIFECYCLEOBJECT_H_
#include <Object.h>
#include <Exceptions/NotFound.h>
#include <Exceptions/Duplicate.h>
#include <Exceptions/Rollback.h>
#include <Exceptions/Terminate.h>
#include <Object.h>
#include <Core/System/Lock.h>
#include <Core/Tools/XMLParser.h>

namespace Meow
{
	class LifecycleObject;
	typedef Reference<LifecycleObject> LifecycleObject_ptr;

	class Factory;
	typedef Reference<Factory> Factory_ptr;

	class LifecycleObject : public Object
	{
	public:
		LifecycleObject();
		~LifecycleObject();

		virtual void    		__Destroy( );
		virtual bool    		IsPurgeable( );
		virtual void    		Restore( );
		virtual void    		RestoreFromAttributes( Core::Types::Array_ptr attributes );
		virtual void    		Passivate( );
		virtual void    		_Delete( );
		virtual void    		Create( );
		virtual bool    		Getter( ) ;
		virtual bool    		Setter( );
		virtual void    		ReadRequired( );
		virtual bool    		IsReadRequired( );
		virtual void    		ClearReadRequired( );
		virtual void    		MarkDirty( );
		virtual void    		ClearDirty( );
		virtual bool    		IsDirty( );
		Factory_ptr     		GetFactory();
		virtual void    		MakeBeforeImage();
		virtual void    		PostUpdated();
		virtual void 			SetVariable( std::string varname, Core::ptr value );
		virtual Core::ptr 		GetVariable( std::string varname );
		virtual bool 			ExistsVariable( std::string varname );
		virtual Core::Types::Stack_ptr 
								GetVariableKeys();
		virtual Core::Types::Array_ptr 
								GetAttributeImage();
		virtual std::string 	ToXML();
		virtual std::string 	GetPublicAlias();
		virtual std::string 	GetURL();
		virtual std::string 	Validate( );
		virtual bool 			QuickValidate( );
		virtual Core::Types::Array_ptr 
								GetCallableMethods( );
		virtual void    		BeginTransaction();
		virtual void    		CommitTransaction();
		virtual void    		RollbackTransaction();

	protected:
		/* If your object uses DataObject persistence it will
   	   	   need to return valid XML from this method */
		virtual std::string     GetPersistenceSpecification();
		bool iIsDirty;
		bool iReadRequired;
		Factory_ptr iFactory;
		friend class Factory;
		Core::Types::Array_ptr iBeforeImage;
	};
} // End of NameSpace Meow

#endif
