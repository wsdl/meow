// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetClassName and GetClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#ifndef _CORE_SCOPINGARRAY_H_
#define _CORE_SCOPINGARRAY_H_
#include <Core/Object.h>
#include <Core/Types/Stack.h>
#include <Core/Types/String.h>
#include <Core/Types/ValueType.h>

/* The scoping array serves to wrap kernel array to provide better separation between scopes when objects are used to enter and leave scopes. The idea
   is that typically when an ObjectCall is made - or any other delegating call - we PushScope, and when that delegated call returns it PopScopes meaning
   that clutter that is set in the callee does not get reflected in the caller, with the following exceptions:

   1. If the variable is set in a the scope above, it gets updated in that scope, unless the Push was opaque (in which case you can only set or get to the level of
      the closest opaque scope).
   2. If the variable is DeepSet then it goes straight through opaque scopes, mostly used for object attributes
   3. With objects you should use PushScopeOpaqueWithCopy specifying Survivors, because this will allow object attributes to be copied forward and be visible to
      "Get" calls, and "Set" calls already go down with DeepSet. (perhaps a better implementation would have been DeepGet).
   4. PushScopeOpaque creates a new scope that can't see the scopes below it.
   5. PushScopeOpaqueWithCopy creates a new scope that can't modify the scopes below it, but it contains an exact copy of the keys and object references, unless you specify
      a survivor list which would form the only items that could get copied if they are set.

 */
namespace Meow {
	namespace Core {
		namespace Types {
			class Scope;
			typedef Reference<Scope> Scope_ptr;
			class Scope : public Core::Object
			{
			public:
				Scope();
				virtual ~Scope();
				bool isOpaque;
				Scope_ptr parent;
				Array_ptr properties;

				void DeepSet( string index, ptr item );
				void DeepSet( string index, unsigned long id, ptr item );
				void DeepSet( unsigned long index, ptr item );

				void Set( string index, ptr item );
				void Set( string index, unsigned long id, ptr item );
				void Set( unsigned long index, ptr item );

				ptr  Get( string index );
				ptr  Get( unsigned long key );

				ptr  ParentGet( string index );
				ptr  ParentGet( unsigned long key );

				bool IsSet( std::string key );
				bool IsSet( unsigned long key );

				unsigned long Count();

				void Unset( std::string key );
				void Unset( unsigned long key );

				Stack_ptr GetKeys( );
				Stack_ptr GetValues( );
			};

			class ScopingArray;
			typedef Reference<ScopingArray> ScopingArray_ptr;

			class ScopingArray : public Object
			{
			public:
				ScopingArray();
				virtual ~ScopingArray();
				void DeepSet( string index, ptr item );
				void DeepSet( string index, unsigned long id, ptr item );
				void DeepSet( unsigned long index, ptr item );
				void Set( string index, ptr item );
				void Set( string index, unsigned long id, ptr item );
				void Set( unsigned long index, ptr item );
				ptr  Get( string index );
				ptr  Get( unsigned long key );
				ptr  ParentGet( string index );
				ptr  ParentGet( unsigned long key );
				bool IsSet( std::string key );
				bool IsSet( unsigned long key );
				virtual unsigned long Count();
				void Unset( std::string key );
				void Unset( unsigned long key );
				Stack_ptr GetKeys( );
				Stack_ptr GetValues( );
				void PushScope();
				void PushScopeOpaque();
				void PushScopeOpaqueWithCopy( Array_ptr survivors );
				void PopScope();
			private:
				Stack_ptr iScopes;
			};
		}
	}
} // End of namespace

#endif
