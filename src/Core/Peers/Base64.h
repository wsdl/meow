// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer - Moved embedded dodgy logic into "peers"
#ifndef MEOW_CORE_PEERS_BASE64_H_
#define MEOW_CORE_PEERS_BASE64_H_
#include <Core/Object.h>
namespace Meow {
	namespace Core {
		namespace Peers {
			class Base64;
			typedef Reference<Base64> Base64_ptr;
			class Base64 : public Object
			{
			public:
				Base64();
				virtual ~Base64();
				static char * encodeBase64(int size, unsigned char *src);
				static int decodeBase64(unsigned char *dest, const char *src);
				static std::string charToHex( char dec );

			private:
				static int isBase64(char c);
				static char encode(unsigned char u);
				static unsigned char decode(char c);

			};
		}
	}
}
#endif
