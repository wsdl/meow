/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _SESSION_H_
#define _SESSION_H_
#include <PersistentObject.h>
#include <Factory.h>
#include <Core/Types/Stack.h>
#include <Registry.h>

namespace Meow {
	class SessionFactory;
	class Session;
	class FrameworkMessage;
	typedef Reference<Session> Session_ptr;
	typedef Reference<SessionFactory> SessionFactory_ptr;
	typedef Reference<FrameworkMessage> FrameworkMessage_ptr;

	class SessionFactory : public Factory
	{
	public:
		SessionFactory();
		~SessionFactory();
		virtual LifecycleObject_ptr GetInstance( std::string service );
		Session_ptr GetCurrentSession();
		void    DestroyAllSessionsForUser( Object_ptr user );
	protected:
		virtual void CanWrite();
        virtual void CanRead();
        virtual void CanCreate();
        virtual void CanDelete();
	private:
		Session_ptr iCurrent;
	};
	class FrameworkMessage : public Object
	{
	public:
		FrameworkMessage();
		virtual ~FrameworkMessage();
		std::string iComponent;
		std::string iEventName;
		std::string iMessage;

		std::string ToString();
	};
	
	class Session : public LifecycleObject
	{
	public:
		Session();
		virtual     ~Session();
		std::string 			GetSessionID();
		Object_ptr  			GetCurrentUser();
		void 					PushPrivileges( Object_ptr user );
		void					PopPrivileges( );
		void 					PushFrameworkMessage( std::string component, std::string eventname, std::string message );
		std::string 			ToString();
		void 	    			ClearFrameworkMessages();
        Core::Types::Queue_ptr 	GetFrameworkMessages();
		virtual bool    		IsPurgeable( );
		virtual void    		Restore( );
		virtual void    		Passivate( );
		virtual void    		_Delete( );
		virtual void    		Create( );
		virtual void 			SetVariable( std::string varname, Core::ptr value );
		virtual void 			UnSetVariable( std::string varname );
		virtual Core::ptr 		GetVariable( std::string varname );
		virtual bool 			ExistsVariable( std::string varname );
		virtual Core::Types::Stack_ptr 
								GetVariableKeys();

		
	private:
		Core::Types::Stack_ptr iPrivs;
		Core::Types::Queue_ptr iFrameworkMessages;
		Core::Types::Array_ptr iDataCache;
	};
}
#endif
