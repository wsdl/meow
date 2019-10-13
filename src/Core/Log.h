// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_LOG_H_
#define _CORE_LOG_H_
#include <Core/Object.h>
#include <Core/Types/Stack.h>
#include <Core/Types/Array.h>
#include <iostream>
#include <fstream>
#include <cstdarg>

namespace Meow {
	namespace Core {
		class PerformanceRecord;
		typedef Reference<PerformanceRecord> PerformanceRecord_ptr;
		class PerformanceRecord : public Core::Object
		{
		public:
			PerformanceRecord();
			virtual ~PerformanceRecord();
			unsigned long entry_time;
			unsigned long exit_time;
			std::string recordkey;
		};
		class ProfileRecord;
		typedef Reference<ProfileRecord> ProfileRecord_ptr;
		class ProfileRecord : public Core::Object
		{
		public:
			ProfileRecord();
			virtual ~ProfileRecord();
			std::string key;
			unsigned long minimum_time;
			unsigned long maximum_time;
			unsigned long hits;
			unsigned long average_time;
		};
		class LogCallback;
		typedef Reference<LogCallback> LogCallback_ptr;

		class LogCallback : public Core::Object
		{
		public:
			LogCallback();
			virtual ~LogCallback();
			virtual bool Log( int dateData, std::string header, std::string context, std::string message );
		};

		class Log;
		typedef Reference<Log> Log_ptr;

		class Log : public Core::Object
		{
		public:
			Log( std::string applicationid );
			Log();
			virtual ~Log();

			static Log_ptr GetLog();
			static Log_ptr GetApplicationLog( std::string application );

			/*
				Type specifiers:
						s - char * - must be NULL terminated. If passing a std::string do a .c_str()
						i - int
						l - long
						b - bool
						f - float
						v - ValueType_ptr as parent. Do a getPointerForceIncref( ) on the pointer itself. i.e.:
										Integer_ptr myint=new Integer();
										GetLog()->Debug("MyDemo","v",myint.getPointerForceIncref());

										This ensures reference counting works.
						o - Object_ptr as parent. See instructions for ValueType_ptr
						k - Core::Object_ptr as parent. See instructions for ValueType_ptr
						x - Object is a ptr to an exception

			 */

			void Debug( std::string context, std::string format, ... );
			void Information( std::string context, std::string format, ... );
			void Warning( std::string context, std::string format,  ... );
			void Error( std::string context, std::string format, ... );
			void Compiler( std::string context, std::string format, ... );
			void Sequel( std::string context, std::string format, ... );
			void SequelResult( std::string context, std::string format, ... );
			void Client( std::string context, std::string format, ... );
			void MeowLink( std::string context, std::string format, ... );
			void Security( std::string context, std::string format, ... );
			void Events( std::string event, std::string username, Types::Array_ptr parameters );
			void Header( std::string context, std::string format, ... );
			void Data( std::string context, std::string format, ... );
			void Workflow( std::string context, std::string format, ... );
			void Performance( std::string context, std::string format, ... );
			void Transaction( std::string context, std::string format, ... );
			void Flush();
			void TraceMethodEntry( std::string methodsignature);
			void TraceMethodExit( );
			void DumpTrace();
			void FullTraceMethodEntry( std::string classname,
									   Meow::Core::Types::Array_ptr key,
									   std::string methodname,
									   std::string format,
									   ... );
			void FullTraceMethodExit( std::string classname,
									  Meow::Core::Types::Array_ptr key,
									  std::string methodname,
									  std::string format,
									  ... );
			std::string Offset();
			void Reconfigure();
			void Indent();
			void Outdent();
			void ReconfigureFrom( std::string extent );
			void WriteHeader( std::string context, std::string identifier );
			void TraceOn();
			void TraceOff();

			void SetRemoteLogCallback( LogCallback_ptr log );
		private:
			static Core::Log_ptr iLog;
			static Core::Types::Array_ptr iApplicationLogs;
			Types::Array_ptr iProfile;
			Types::Stack_ptr iRecords;
			unsigned long iOffset;
			ofstream iStream;
			bool iDebug;
			bool iInformation;
			bool iWarning;
			bool iError;
			bool iCompiler;
			bool iSequel;
			bool iSequelResult;
			bool iClient;
			bool iMeowLink;
			bool iSecurity;
			bool iEvents;
			bool iHeader;
			bool iData;
			bool iFlush;
			bool iCout;
			bool iAbortOnCompileMessage;
			bool iRemoteWrite;
			bool iWorkflow;
			bool iTrace;
			bool iPerformance;
			bool iTransaction;
			Types::Array_ptr iRemoteWriteCache;
			LogCallback_ptr iCallback;
		};
	}
} // End of namespace

#endif
