/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _SECURITY_USER_H_
#define _SECURITY_USER_H_
#include <Factory.h>
#include <TransientObject.h>
#include <Exceptions/Security.h>


namespace Meow {
	namespace Security {
		class User;
		class UserFactory;
		class Group;
		class Collection;

		typedef Reference<User> User_ptr;
		typedef Reference<UserFactory> UserFactory_ptr;
		typedef Reference<Group> Group_ptr;
		typedef Reference<Collection> Collection_ptr;

		class UserFactory : public Factory
		{
		public:
			UserFactory();
			~UserFactory();

			LifecycleObject_ptr GetInstance( std::string service );
			User_ptr GetAnonymousUser();
			
			/* Overriding methods for Delegate Factory support */
			virtual LifecycleObject_ptr FindByPrimaryKey( Core::Types::Array_ptr key );
			virtual LifecycleObject_ptr FindByPrimaryKeyOrRestore( Core::Types::Array_ptr key, Core::Types::Array_ptr attributes );
			virtual LifecycleObject_ptr CreateByPrimaryKey( Core::Types::Array_ptr key, std::string service );
			virtual void DestroyByPrimaryKey( Core::Types::Array_ptr key );
			virtual void MarkChangedExternally( Core::Types::Array_ptr key );
		protected:
			virtual void CanRead();		
			virtual Factory_ptr GetDelegateFactory();
			
		private:
			Factory_ptr delegateFactory;
		};

		class User : public TransientObject
		{
		public:
			User();
			~User();

			virtual std::string GetPublicAlias();

			unsigned long GetUid( );

			virtual void SetUsername( std::string uname );
			virtual void SetEmailAddress( std::string email );
			virtual void SetContactNumber( std::string phone );
			virtual void SetPassword( std::string password );
			virtual void SetRelatedObjectReference( std::string ref );

			virtual std::string GetUsername( );
			virtual std::string GetEmailAddress();
			virtual std::string GetContactNumber();
			virtual std::string GetPassword();
			virtual std::string GetRelatedObjectReference();
		private:
			std::string iUserName;
			std::string iEmailAddress;
			std::string iContactNumber;
			std::string iPassword;
			std::string iRelatedObjectReference;
		};
	}
}
#endif
