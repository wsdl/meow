/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer 
 */
#ifndef _PERSISTENCE_LAZYENUMERATOR_H_
#define _PERSISTENCE_LAZYENUMERATOR_H_
#include <Core/Types/Array.h>
#include <Registry.h>
#include <LifecycleObject.h>
#include <Persistence/SQLResult.h>

namespace Meow {
	namespace Persistence {
		class LazyEnumerator : public Core::Types::Array
		{
		public:
			LazyEnumerator();
			~LazyEnumerator();
			virtual Core::ptr  Get( unsigned long key );
			void SetLazyClassName( std::string classname );
			void SetLazyKeyFields( Core::Types::Array_ptr keyFields );
			void SetLazyAttributeFields( Core::Types::Array_ptr attributeFields );
			void SetResultObject( SQLResult_ptr result );
			virtual unsigned long Count();
			void OverrideCount( unsigned long counter );
			virtual std::string GetValueAsString();
		private:
			SQLResult_ptr iResult;
			std::string iLazyClassName;
			Core::Types::Array_ptr iLazyKeyFields;
			Core::Types::Array_ptr iLazyAttributeFields;
			unsigned long iCachedItemCount;
		};
		typedef Reference<LazyEnumerator> LazyEnumerator_ptr;
	} // End of namespace
}
#endif
