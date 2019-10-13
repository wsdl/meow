/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <Core/Threadable.h>
#include <Core/Types/Array.h>
#include <Core/Types/String.h>
#include <Core/Log.h>
#include <Exceptions/NotFound.h>
#include <Exceptions/Terminate.h>
#include <Exceptions/Rollback.h>
#include <Exceptions/Security.h>

namespace Meow {
	class Object;
	typedef Reference<Object> Object_ptr;
	class Object : public Core::Threadable
	{
	public:
		Object();
		virtual ~Object();
		virtual void __Destroy( );

		Core::Types::Array_ptr GetUniqueKey( );
		void SetUniqueKey( Core::Types::Array_ptr );
		unsigned long GetHashCode();

		virtual std::string ToString();
		virtual bool Equals( Object_ptr other );

		virtual void SetVariable( std::string varname, Core::ptr value );
		virtual Core::ptr GetVariable( std::string varname );
		virtual bool ExistsVariable( std::string varname );
		virtual Core::Types::Stack_ptr GetVariableKeys();

		Core::Log_ptr GetLog();

		// For dynamic binding
		virtual Core::ptr ExecuteMethod( std::string methodname, Meow::Core::Types::Array_ptr parameters );

		// Framework method
		bool GetReady();
		void IsReady();
	protected:
		Core::Types::Array_ptr __Data;
	private:
		Core::Types::Array_ptr PrivateKey;
		unsigned long iHash;
		bool __Ready;
	}; // End of Class CoreObject
} // End of NameSpace Meow
#endif