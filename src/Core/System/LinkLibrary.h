// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_LINKLIBRARY_H_
#define _CORE_LINKLIBRARY_H_
#include <Core/Object.h>
#include <SystemException.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace System {
			class LinkLibrary : public Core::Object
			{
			public:
				LinkLibrary();
				~LinkLibrary();

				void Install( std::string libraryname );
				// Throws SystemException if it fails.
				void * GetFunction( std::string ordinalname );
			private:
				long LoadCount;
				void * Library;
			};
			typedef Reference<LinkLibrary> LinkLibrary_ptr;
		}
	}
} // End of namespace

#endif
