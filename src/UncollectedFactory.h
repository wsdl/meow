/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _MEOW_UNCOLLECTEDFACTORY_H_
#define _MEOW_UNCOLLECTEDFACTORY_H_
#include <Factory.h>
namespace Meow
{
	class UncollectedFactory;
	typedef Reference<UncollectedFactory> UncollectedFactory_ptr;

	class UncollectedFactory : public Factory
	{
	public:
		UncollectedFactory();
		~UncollectedFactory();

		virtual LifecycleObject_ptr FindByPrimaryKey( Core::Types::Array_ptr key );

		virtual LifecycleObject_ptr CreateByPrimaryKey( Core::Types::Array_ptr key,
				std::string service );

		virtual void DestroyByPrimaryKey( Core::Types::Array_ptr key );

		virtual void MarkChangedExternally( Core::Types::Array_ptr key );
	};
}
#endif
