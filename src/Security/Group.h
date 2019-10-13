/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _SECURITY_GROUP_H_
#define _SECURITY_GROUP_H_
#include <Factory.h>
#include <LifecycleObject.h>
#include <Exceptions/Security.h>
#include <TransientObject.h>
#include <Security/User.h>

namespace Meow {
	namespace Security {
		class GroupFactory;
		class Group;

		typedef Reference<GroupFactory> GroupFactory_ptr;
		typedef Reference<Group> Group_ptr;

		class GroupFactory : public Factory
		{
		public:
			GroupFactory();
			~GroupFactory();
			LifecycleObject_ptr GetInstance( std::string service ) throw (Exceptions::NotFound);
		};

		class Group : public TransientObject
		{
		public:
			Group();
			~Group();
			std::string GetName( );
			void        SetDescription( std::string desc );
			std::string GetDescription( );
			bool		IsAuthorised( Security::User_ptr user );
		private:
			std::string iDescription;
		};
	}
}

#endif
