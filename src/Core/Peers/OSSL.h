// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer - Retaining OpenSSL as main peer for now.
#ifndef _MEOW_OSSL_H_
#define _MEOW_OSSL_H_
#include <Core/Object.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <string>
namespace Meow {
	namespace Core {
		namespace Peers {
			class OSSLChunk : public Core::Object
			{
			public: 	
				OSSLChunk();
				~OSSLChunk();
				unsigned long length;
				unsigned char * bytes;
				bool continueit;
			};
			typedef Reference<OSSLChunk> OSSLChunk_ptr;

			class OSSL;
			typedef Reference<OSSL> OSSL_ptr;
			class OSSL : public Core::Object
			{
			public:
					OSSL();
					virtual ~OSSL();

					bool Init( std::string server_name, int server_port );
					bool InitX509( std::string ca_cert,
								   std::string cli_cert,
								   std::string cli_key,
								   std::string server_name,
								   int server_port );
					void DiagnoseError( int rc );
					void DiagnoseSSLError( int rc );
					bool Close( );
					bool Write( std::string buffer );
					OSSLChunk_ptr Read( int bufferlength );

					static std::string MD5( std::string in );
					static std::string _SHA256( std::string in );
					static std::string _SHA1( std::string in );

					static void * RawMD5( std::string in );
					static void * RawSHA256( std::string in );
					static void * RawSHA1( std::string in );

					static std::string SHA256HMAC( std::string in, std::string secret );

			private:
				int server_fd;
				BIO* sbio;
				SSL_CTX *sctx;//Context
				SSL_SESSION *ssesn; //Session
				SSL *sconn; //Connection
				bool x509_support;
				X509 *cacert;
				X509 *clicert;
				RSA *rsa;
			};
		}
	}
}

#endif
