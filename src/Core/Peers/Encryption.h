// Copyright (c) 2015 David Spencer
// All rights reserved.
// Version: 3.2
// Change History:
//	13/11/2015
#ifndef MEOW_CORE_PEERS_ENCRYPTION_H_
#define MEOW_CORE_PEERS_ENCRYPTION_H_
#include <Core/Object.h>
#include <Core/Types/String.h>
namespace Meow {
	namespace Core {
		namespace Peers {
			class Encryption;
			typedef Reference<Encryption> Encryption_ptr;
			class Encryption : public Object
			{
			public:
				Encryption();
				virtual ~Encryption();
				void AddPrivateKey( std::string user,
					std::string privateky,
					std::string passphrase );
				void AddPublicKey( std::string user,
					std::string publickey );
				Core::Types::ValueType_ptr Decrypt( std::string user,
					Core::Types::String_ptr plainText );
				Core::Types::String_ptr Encrypt( std::string user,
					Core::Types::ValueType_ptr cypherText );
			private:
				void * __context;
			};
		}
	}
}
#endif
