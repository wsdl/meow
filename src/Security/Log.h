/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _SECURITY_LOG_H_
#define _SECURITY_LOG_H_
#include <PersistentObject.h>
#include <PersistenceFactory.h>
#include <Exceptions/Security.h>
#include <Core/Time/Date.h>


namespace Meow {
	namespace Security {
		class Log;
		class LogFactory;

		typedef Reference<Log> Log_ptr;
		typedef Reference<LogFactory> LogFactory_ptr;

		class LogFactory : public PersistenceFactory
		{
		public:
			LogFactory();
			~LogFactory();

			LifecycleObject_ptr GetInstance( std::string service );
			virtual std::string GetFriendlyName();
			virtual bool IsPublicApplication();
			virtual void RegisterEvents();
			virtual void OnEvent( std::string event, Meow::Core::Types::Array_ptr parameters );
			void RecordLoginAttempt( Core::Types::Array_ptr parameters );

		};

		class Log : public PersistentObject
		{
		public:
			Log();
			~Log();

			void SetWhen( Core::Time::Date_ptr whentime );
			void SetAction( std::string action );
			void SetUsername( std::string username );
			void SetOutcome( std::string outcome );

			Core::Time::Date_ptr GetWhen();
			std::string GetAction();
			std::string GetUsername();
			std::string GetOutcome();

			void Record( std::string action, std::string username, std::string outcome );
			virtual Core::ptr ExecuteMethod( std::string methodname, Meow::Core::Types::Array_ptr parameters ) ;

		};
	}
}
#endif
