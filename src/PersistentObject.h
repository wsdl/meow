/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _PERSISTENTOBJECT_H_
#define _PERSISTENTOBJECT_H_
#include <LifecycleObject.h>
#include <PersistenceFactory.h>

namespace Meow
{
	class PersistentObject;
	typedef Reference<PersistentObject> PersistentObject_ptr;

	class Factory;
	typedef Reference<Factory> Factory_ptr;

	class PersistentObject : public LifecycleObject
	{
	public:
		PersistentObject();
		~PersistentObject();

		virtual bool      HasPublicVariable( std::string alis );
		virtual Core::ptr GetPublicVariable( std::string alias );
		virtual void      SetPublicVariable( std::string alias, Core::ptr value );

		virtual bool    IsPurgeable( );
		virtual void    Restore( );
		virtual void    RestoreFromAttributes( Core::Types::Array_ptr attributes );
		virtual void    Passivate( );
		virtual void    _Delete( );
		virtual void    Create( );

		virtual Core::ptr ExecuteMethod( std::string methodname, Meow::Core::Types::Array_ptr parameters );

		virtual unsigned long CountJoin( std::string relationship );
		virtual Core::Types::Array_ptr GetJoin( std::string relationship );

		virtual unsigned long CountAutoJoin( std::string otherfactory, std::string publicattribute );
		virtual Core::Types::Array_ptr GetAutoJoin( std::string otherfactory, std::string publicattribute );
		virtual Core::Types::Array_ptr GetAutoJoinWithLimit( std::string otherfactory, std::string publicattribute, int offset, int limit );
		virtual void ImageOrFileCopy(std::string srcattribute, PersistentObject_ptr targetobj, std::string targetattribute);
		virtual bool Lock();
		virtual bool UnLock();
		virtual Core::Types::Array_ptr GetLockInfo();
		virtual void Available( );
	protected:
		Filter_ptr JoinSpecToFilter( std::string relationship );
		Object_ptr GetPersistenceDescriptor();
		Object_ptr GetFilter( );
		bool iNotifiable;

	private:
		Object_ptr iFilterDescriptor;
		Core::Types::Array_ptr iCachedJoinSpecs;
		
	};
} // End of NameSpace Meow

#endif
