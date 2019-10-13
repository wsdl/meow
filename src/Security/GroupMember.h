/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _SECURITY_GROUPMEMBER_H_
#define _SECURITY_GROUPMEMBER_H_
#include <Factory.h>
#include <TransientObject.h>
#include <Exceptions/Security.h>

namespace Meow {
	namespace Security {
		class GroupMemberFactory;
		class GroupMember;
		class User;

		typedef Reference<GroupMemberFactory> GroupMemberFactory_ptr;
		typedef Reference<GroupMember> GroupMember_ptr;
		typedef Reference<User> User_ptr;

		class GroupMemberFactory : public Factory
		{
		public:
			GroupMemberFactory();
			~GroupMemberFactory();
			LifecycleObject_ptr GetInstance( std::string service ) ;

		};

		class GroupMember : public TransientObject
		{
		public:
			GroupMember();
			~GroupMember();
			std::string GetName( );
			Security::User_ptr GetUser();
		};
	}
}

#endif
