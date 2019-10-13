/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _SECURITY_SESSION_H_
#define _SECURITY_SESSION_H_
#include <PersistentObject.h>
#include <PersistenceFactory.h>

namespace Meow {
	namespace Security {
		class SessionFactory;
		class Session;
		typedef Reference<Session> Session_ptr;
		typedef Reference<SessionFactory> SessionFactory_ptr;

		class SessionFactory : public PersistenceFactory
		{
		public:
			SessionFactory();
			~SessionFactory();
			LifecycleObject_ptr GetInstance( std::string service );
			virtual std::string GetFriendlyName();
			virtual bool IsPublicApplication();
		protected:
			virtual void CanWrite();
			virtual void CanRead() ;
			virtual void CanCreate();
			virtual void CanDelete();
		private:
		};

		class Session : public PersistentObject
		{
		public:
			Session();
			virtual  ~Session();
		};
	}
}


#endif
