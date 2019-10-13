// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetClassName and GetClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#ifndef _CORE_REMOTECALL_H_
#define _CORE_REMOTECALL_H_
#include <Core/Object.h>
#include <Core/IO/HTTPClient.h>
#include <Core/Tools/XMLParser.h>
#include <Core/Storage/File.h>
#include <Core/Types/Array.h>

namespace Meow {
	namespace Core {
		namespace Client {
			class RemoteCall;
			typedef Reference<RemoteCall> RemoteCall_ptr;
			class RemoteCallHandler;
			typedef Reference<RemoteCallHandler> RemoteCallHandler_ptr;
			class RemoteCallResponse;
			typedef Reference<RemoteCallResponse> RemoteCallResponse_ptr;

			class RemoteCall : public Meow::Core::Object
			{
			public:
				RemoteCall();
				virtual ~RemoteCall();

				void SetURL( std::string url );
				void SetRemoteHost( std::string remotehost );
				std::string GetURL();
				std::string GetRemoteHost();

				RemoteCallResponse_ptr MakeSynchronousRemoteCall(
						std::string originator,
						std::string correlationid,
						std::string service,
						std::string method,
						std::string key,
						Meow::Core::Types::Array_ptr parameters,
						Meow::Core::IO::HTTPClient_ptr connection );
			private:
				std::string iURL;
				std::string iRemoteHost;
			};

			class RemoteCallHandler : public Core::Tools::XMLParser
			{
			public:
				RemoteCallHandler();
				virtual ~RemoteCallHandler();
				virtual void StartElement( string element, Core::Types::Array_ptr attributes );
				virtual void EndElement( string element );
				virtual void CharacterData( string data );
				RemoteCallResponse_ptr GetResponse();
			private:
				RemoteCallResponse_ptr iResponse;
				std::string iCurrentElement;
				std::string iCurrentCharacterData;
				Core::Types::Array_ptr iCurrentAttributes;
			};
			class RemoteCallResponse : public Object
			{
			public:
				RemoteCallResponse();
				~RemoteCallResponse();
				std::string GetOriginator();
				std::string GetCorrelationID();
				std::string GetPrimeType();
				std::string GetRemoteClassName();
				std::string GetReturned();
				std::string GetException();
				Core::Types::Array_ptr GetExceptionMessages();
				int GetErrorCode();
			private:
				friend class RemoteCallHandler;
				std::string iOriginator;
				std::string iCorrelationID;
				std::string iPrimeType;
				std::string iRemoteClassName;
				std::string iReturned;
				std::string iException;
				Core::Types::Array_ptr iExceptionMessages;
				int iErrorCode;
			};
		}
	}
}
#endif
