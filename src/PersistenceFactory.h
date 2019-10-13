/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef PERSISTENCEFACTORY_H_
#define PERSISTENCEFACTORY_H_
#include <Factory.h>
#include <Filter.h>

namespace Meow {
	class PersistenceFactory;
	typedef Reference<PersistenceFactory> PersistenceFactory_ptr;

	class PersistenceFactory : public Factory
	{
	public:
		PersistenceFactory();
		~PersistenceFactory();

		virtual bool Upgrade( int currentVersion );

		Object_ptr GetPersistenceDescriptor( std::string service );

		virtual unsigned long CountMatches( std::string service, std::string fieldName, Core::ptr value );
		virtual Core::Types::Array_ptr Matches( std::string service, std::string fieldName, Core::ptr value );

		
		virtual unsigned long CountEnumerate( std::string service );
		virtual unsigned long CountEnumerateWithFilter( std::string service, Filter_ptr );
		virtual Core::Types::Array_ptr Enumerate( std::string service );
		virtual Core::Types::Array_ptr EnumerateWithLimit( std::string service, int offset, int limit );
		virtual Core::Types::Array_ptr EnumerateWithLimitAndDirection( std::string service, int offset, int limit, std::string orderby, std::string attribute );
		virtual Core::Types::Array_ptr EnumerateWithFilter( std::string service, Filter_ptr filter );
		virtual Core::Types::Array_ptr EnumerateWithFilterAndDirection( std::string service, Filter_ptr filter, std::string orderby, std::string attribute );
		virtual Core::Types::Array_ptr EnumerateWithFilterAndLimit( std::string service, Filter_ptr filter, int offset, int limit );
		virtual Core::Types::Array_ptr EnumerateWithFilterAndLimitAndDirection( std::string service, Filter_ptr filter, int offset, int limit, std::string orderby, std::string attribute );
		virtual void DestroyWithFilter( std::string service, Filter_ptr filter );
		virtual Core::Types::Array_ptr Search( std::string, unsigned long offset, unsigned long limit, std::string service, Core::Types::Array_ptr constraints=(Core::Types::Array*)NULL );
		virtual unsigned long CountSearch( std::string, std::string service,Core::Types::Array_ptr constraints=(Core::Types::Array*)NULL );

	private:
		Object_ptr iPersistenceDescriptor;
	};
}

#endif
