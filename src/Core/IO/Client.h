// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_CLIENT_H_
#define _CORE_CLIENT_H_
#include <Core/Object.h>
#include <Core/Types/Stack.h>
#include <Core/System/Lock.h>
#include <Core/Types/Array.h>
#include <Core/Peers/OSSL.h>

namespace Meow {
	namespace Core {
		namespace IO {
			class ConnectionManager;
			typedef enum {closed,open,reading,writing} ClientStatus;

			class Client;
			typedef Reference<Client> Client_ptr;

			class Client : public Core::Object
			{
			public:
				Client();
				~Client();
				void InUse();
				void Released();
				bool IsInUse();
				bool IsConnected();
				bool IsSecure();
				void MakeSecure();
				void MakeX509Secure();
				void SetX509CACert( std::string cert );
				void SetX509CLICert( std::string cert );
				void SetX509PrivateKey( std::string key );
				void SetHostName( std::string hostname );
				void SetPortNo( unsigned long portno );
				void NotStrict();
				std::string GetActiveHost();

				friend class ConnectionManager;
				friend class ServiceThread;

				void Open( );
				void WriteBytes( unsigned long length, void * buffer );
				unsigned long ReadBytes( unsigned long length, void * buffer, bool binary=false );
				void Close( );

				ClientStatus  GetStatus();

				void SetOnComplete( Core::Types::Stack_ptr stack, Core::System::Lock_ptr lock );
			private:
				unsigned long PortNumber;
				std::string iActiveHost;
				Meow::Core::Types::Array_ptr HostNames;
				long sock;
				ClientStatus status;
				bool Usage;
				bool Connected;
				bool Strict;
				Meow::Core::Types::Stack_ptr OnRelease;
				Meow::Core::System::Lock_ptr OnReleaseLock;
				bool Secure;
				bool X509Secure;
				Meow::Core::Peers::OSSL_ptr     PeerContext;
				std::string X509CACert;
				std::string X509CLICert;
				std::string X509PrivateKey;
			};
		}
	}
}
#endif
