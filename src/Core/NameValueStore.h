// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_NAMEVALUESTORE_H_
#define _CORE_NAMEVALUESTORE_H_
#include <Core/Object.h>
#include <Core/Types/ValueType.h>

namespace Meow {
	namespace Core {
		class NameValueStore;
		typedef Reference<NameValueStore> NameValueStore_ptr;

		class NameValueStore : public Object
		{
		public:
			NameValueStore();
			~NameValueStore();
			void SetValue(std::string domain, std::string name,  Meow::Core::Types::ValueType_ptr value );
			Core::Types::ValueType_ptr GetValue( std::string domain, std::string name );
			void UnsetValue( std::string domain, std::string name );
			bool ExistsValue( std::string domain, std::string name );
			Core::Types::ValueType_ptr OverrideValue( std::string domain, std::string name, Core::Types::ValueType_ptr currentValue );
		private:
			void * iDriver;
		};
	}
}
#endif
