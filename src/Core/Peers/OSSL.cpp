// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer - Retaining PolarSSL as main peer for now.
#include <Core/Peers/OSSL.h>
#include <Core/Peers/Base64.h>
#include <iostream>
#include <string.h>
#include <Core/Log.h>
#ifdef  OPENSSL_NO_SSL3_METHOD
#undef  OPENSSL_NO_SSL3_METHOD
#endif
#include <openssl/ssl.h>
#include <openssl/ssl3.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <Core/Storage/Directory.h>
#include <inc/configuration.h>

// #define SSLv3_method           TLS_method
// #define SSLv3_server_method    TLS_server_method
// #define SSLv3_client_method    TLS_client_method


#ifndef _CRT_SECURE_NO_DEPRECATE
	#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#define OpenSSL_add_ssl_algorithms() SSL_library_init()
#define SSLeay_add_ssl_algorithms() SSL_library_init()

void meow_ssl_debug( void * ctx, int level, const char * str )
{

    if ( level<2 )
    	Meow::Core::Log::GetLog()->Data("Meow::Core::Peers::OSSL::Debug","s",str);
}

unsigned char* hmac_sha256(const void *key, int keylen,
                           const unsigned char *data, int datalen,
                           unsigned char *result, unsigned int* resultlen)
{
    return HMAC(EVP_sha256(), key, keylen, data, datalen, result, resultlen);
}

Meow::Core::Peers::OSSLChunk::OSSLChunk()
{
	SetMeowClassName("Core/Peers/OSSL");
	length=0;
	bytes=NULL;
	continueit=true;
}
Meow::Core::Peers::OSSLChunk::~OSSLChunk()
{
	if ( bytes!=NULL )
		free(bytes);
}


Meow::Core::Peers::OSSL::OSSL() :
	server_fd(-1),
	x509_support(false)
{
	SetMeowClassName("Core/Peers/OSSL");
}
Meow::Core::Peers::OSSL::~OSSL()
{
}

bool Meow::Core::Peers::OSSL::Init( std::string server_name, int server_port )
{
	 int ret=-99;

	 x509_support=false;

	 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ssd",server_name.c_str(),":",server_port);
	 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","s","ssl_init");

	 //Initialising the SSL Library
	 SSL_library_init(); /* load encryption & hash algorithms for SSL */
	 SSL_load_error_strings(); /* load the error strings for good error reporting */
	 ERR_load_BIO_strings();
	 OpenSSL_add_all_algorithms();
	 ERR_load_crypto_strings();

	 //Creating and Setting up SSL Context
	 std::string protocol = "";
	 char hostcnfString[1024];
	 sprintf(hostcnfString,"%s:%d",server_name.c_str(),server_port);
	 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ss","ssl_init - hostcnfString:",hostcnfString);
	 if ( Configuration::GetConfiguration()->IsSet(hostcnfString) )
		 protocol=Configuration::GetConfiguration()->Get(hostcnfString);
	 else
		 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ss","ssl_init - protocol","Not found in cnf default is TLSv1.2");

	 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ss","ssl_init - protocol",protocol.c_str());
	 if(protocol=="SSLv23")
 	 	 sctx = SSL_CTX_new(SSLv23_method());
	 // else if(protocol == "SSLv3")
	 //	 sctx = SSL_CTX_new(SSLv3_method());
	 else
	 	 sctx = SSL_CTX_new(TLSv1_2_client_method());
	 
	 // sctx = SSL_CTX_new(SSLv3_method());
	 if (!sctx) 
{
	 	 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","sss","ssl_init - abort","Can't initialise protocol:",protocol.c_str());
	 	 return false;
//	    exit(0);
	 }
	 /* verification */
	 SSL_CTX_set_verify(sctx, SSL_VERIFY_NONE, NULL);
	 /* Set the verification depth to 0 */
	// SSL_CTX_set_verify_depth(sctx,0);

	 sbio = BIO_new_ssl_connect(sctx);
	 BIO_get_ssl(sbio,&sconn);
	 SSL_set_mode(sconn, SSL_MODE_AUTO_RETRY);	
	 int err = ERR_get_error();
	 if( sbio == NULL)
	 {
	 	 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ssd","ssl_init - abort","Can't initiate BIO:",err);
		 return false;
	 }

	 char serverString[1024];
	 sprintf(serverString,"%s:%d",server_name.c_str(),server_port);
	 int res2=BIO_set_conn_hostname(sbio, serverString);
	 err = ERR_get_error();
	 if ( res2 != 1 )
	 {
		 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ss","BIO_set_conn_hostname failed",ERR_reason_error_string(err));
		 return false;
     }
	 res2=BIO_do_connect(sbio);
	 err = ERR_get_error();
	 if ( res2 != 1 )
	 {
		 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::Init","ss","BIO_do_connect failed",ERR_reason_error_string(err));
		 return false;
     }
	 
	 return true;
}
bool Meow::Core::Peers::OSSL::InitX509( std::string ca_cert,
			   std::string cli_cert,
			   std::string cli_key,
			   std::string server_name,
			   int server_port )
{
	   int ret;
	   x509_support=true;
	   memset(&ssesn,0,sizeof(SSL_SESSION));
	   memset(&sconn,0,sizeof(SSL));
	   memset(&sctx,0,sizeof(SSL_CTX));

	   //Initialising the SSL Library
	   SSL_library_init(); /* load encryption & hash algorithms for SSL */
	   SSL_load_error_strings(); /* load the error strings for good error reporting */
	   ERR_load_BIO_strings();
	   OpenSSL_add_all_algorithms();
	   ERR_load_crypto_strings();

	 
	   //Creating and Setting up SSL Context
		 sctx = SSL_CTX_new(TLSv1_2_client_method());
	    /* verification */
	    SSL_CTX_set_verify(sctx, SSL_VERIFY_PEER, NULL);
	    /* Set the verification depth to 1 */
	    SSL_CTX_set_verify_depth(sctx,1);
	    sconn=SSL_new(sctx);

	    std::string certstore_path = "/etc/ssl/certs/ca-certificates.crt";
	    if ( Configuration::GetConfiguration()->IsSet("sslcert.store") )
	    	certstore_path=Configuration::GetConfiguration()->Get("sslcert.store");
	    ret = SSL_CTX_load_verify_locations(sctx, certstore_path.c_str(), NULL);
	    if (ret == 0) {
	    	Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::InitX509","ss","Unable to load the trust store from",certstore_path.c_str());
	        return false;
	    }

		 sbio = BIO_new_ssl_connect(sctx);
		 BIO_get_ssl(sbio,&sconn);
		 SSL_set_mode(sconn, SSL_MODE_AUTO_RETRY);
		 int err = ERR_get_error();
		 if( sbio == NULL)
			 return false;

		 char serverString[1024];
		 sprintf(serverString,"%s:%d",server_name.c_str(),server_port);
		 int res2=BIO_set_conn_hostname(sbio, serverString);
		 err = ERR_get_error();
		 if ( res2 != 1 )
		 {
			 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::InitX509","ss","BIO_set_conn_hostname failed",ERR_reason_error_string(err));
			 return false;
	     }

		 SSL_set_cipher_list(sconn, "MD5,SHA256");
		 //SSL_set_cipher_list(sconn, "DEFAULT");

		 res2=BIO_do_connect(sbio);
		 err = ERR_get_error();
		 if ( res2 != 1 )
		 {
			 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::InitX509","ss","BIO_do_connect failed",ERR_reason_error_string(err));
			 return false;
	     }

		 res2 = BIO_do_handshake(sbio);
		 err = ERR_get_error();
		 if ( res2 != 1 )
		 {
			 Core::Log::GetLog()->Client("Meow::Core::Peers::OSSL::InitX509","ss","BIO_do_handshake failed",ERR_reason_error_string(err));
			 return false;

		 }
		 /* Step 1: verify a server certificate was presented during the negotiation */
		 if((SSL_get_peer_certificate(sconn)) == NULL) {
			 Core::Log::GetLog()->Data("Meow::Core::Peers::OSSL::InitX509","s","No Certificate presented during negotiation");
			 return false;
		 }
		 /* Step 2: verify the result of chain verification */
	     if (SSL_get_verify_result(sconn) != X509_V_OK) {
	    	 Core::Log::GetLog()->Data("Meow::Core::Peers::OSSL::InitX509","s","Unable to verify connection result");
	    	 return false;
	     }
	     return true;

}
void Meow::Core::Peers::OSSL::DiagnoseError( int rc )
{
	char * answer=(char*)"OK";
	switch (rc)
	{

		case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT: answer=(char*)" unable to get issuer certificate"; break;
		case X509_V_ERR_UNABLE_TO_GET_CRL: answer=(char*)" unable to get certificate CRL"; break;
		case X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE: answer=(char*)" unable to decrypt certificate's signature"; break;
		case X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE: answer=(char*)" unable to decrypt CRL's signature"; break;
		case X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY: answer=(char*)" unable to decode issuer public key"; break;
		case X509_V_ERR_CERT_SIGNATURE_FAILURE: answer=(char*)" certificate signature failure"; break;
		case X509_V_ERR_CRL_SIGNATURE_FAILURE: answer=(char*)" CRL signature failure"; break;
		case X509_V_ERR_CERT_NOT_YET_VALID: answer=(char*)" certificate is not yet valid"; break;
		case X509_V_ERR_CERT_HAS_EXPIRED: answer=(char*)" certificate has expired"; break;
		case X509_V_ERR_CRL_NOT_YET_VALID: answer=(char*)" CRL is not yet valid"; break;
		case X509_V_ERR_CRL_HAS_EXPIRED: answer=(char*)" CRL has expired"; break;
		case X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD: answer=(char*)" format error in certificate's notBefore field"; break;
		case X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD: answer=(char*)" format error in certificate's notAfter field"; break;
		case X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD: answer=(char*)" format error in CRL's lastUpdate field"; break;
		case X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD: answer=(char*)" format error in CRL's nextUpdate field"; break;
		case X509_V_ERR_OUT_OF_MEM: answer=(char*)" out of memory"; break;
		case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT: answer=(char*)" self signed certificate"; break;
		case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN: answer=(char*)" self signed certificate in certificate chain"; break;
		case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY: answer=(char*)" unable to get local issuer certificate"; break;
		case X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE: answer=(char*)" unable to verify the first certificate"; break;
		case X509_V_ERR_CERT_CHAIN_TOO_LONG: answer=(char*)" certificate chain too long"; break;
		case X509_V_ERR_CERT_REVOKED: answer=(char*)" certificate revoked"; break;
		case X509_V_ERR_INVALID_CA: answer=(char*)" invalid CA certificate"; break;
		case X509_V_ERR_PATH_LENGTH_EXCEEDED: answer=(char*)" path length constraint exceeded"; break;
		case X509_V_ERR_INVALID_PURPOSE: answer=(char*)" unsupported certificate purpose"; break;
		case X509_V_ERR_CERT_UNTRUSTED: answer=(char*)" certificate not trusted"; break;
		case X509_V_ERR_CERT_REJECTED: answer=(char*)" certificate rejected"; break;
		case X509_V_ERR_SUBJECT_ISSUER_MISMATCH: answer=(char*)" subject issuer mismatch"; break;
		case X509_V_ERR_AKID_SKID_MISMATCH: answer=(char*)" authority and subject key identifier mismatch"; break;
		case X509_V_ERR_AKID_ISSUER_SERIAL_MISMATCH: answer=(char*)" authority and issuer serial number mismatch"; break;
		case X509_V_ERR_KEYUSAGE_NO_CERTSIGN: answer=(char*)"key usage does not include certificate signing"; break;
		case X509_V_ERR_APPLICATION_VERIFICATION: answer=(char*)" application verification failure"; break;
	}
	Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::DiagnoseError","s",answer);
}
void Meow::Core::Peers::OSSL::DiagnoseSSLError( int rc )
{
	char * answer=(char*)"OK";
	switch (rc)
		{
			case SSL_ERROR_NONE: answer=(char*)" The TLS/SSL I/O operation completed. This result code is returned if and only if ret > 0."; break;
			case SSL_ERROR_ZERO_RETURN: answer=(char*)" The TLS/SSL connection has been closed. "; break;
			case SSL_ERROR_WANT_READ: answer=(char*)"The operation did not complete; the same TLS/SSL I/O function should be called again later. "; break;
			case SSL_ERROR_WANT_WRITE: answer=(char*)" The operation did not complete; the same TLS/SSL I/O function should be called again later."; break;
			case SSL_ERROR_WANT_CONNECT: answer=(char*)"The operation did not complete; the same TLS/SSL I/O function should be called again later. "; break;
			case SSL_ERROR_WANT_ACCEPT: answer=(char*)" The operation did not complete; the same TLS/SSL I/O function should be called again later."; break;
			case SSL_ERROR_WANT_X509_LOOKUP: answer=(char*)" The operation did not complete because an application callback set by SSL_CTX_set_client_cert_cb() has asked to be called again"; break;
			case SSL_ERROR_SYSCALL: answer=(char*)"SSL_ERROR_SYSCALL - Some I/O error occurred."; break;
			case SSL_ERROR_SSL: answer=(char*)" A failure in the SSL library occurred, usually a protocol error. The OpenSSL error queue contains more information on the error."; break;
		}
	Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::DiagnoseSSLError","s",answer);
}
bool Meow::Core::Peers::OSSL::Close( )
{
	if ( x509_support )
	{
	     X509_free(clicert);
	     X509_free(cacert);
	}

	BIO_free_all(sbio);
}
bool Meow::Core::Peers::OSSL::Write( std::string buffer )
{
	int ret;
	bool failure=true;
	do
	{
		// ret = SSL_write( sconn, (const unsigned char*)buffer.c_str(), buffer.length() );
		ret = BIO_write(sbio,(const unsigned char*)buffer.c_str(), buffer.length() );
		if (ret < 0)
		{
			if (!BIO_should_retry(sbio))
			{
				Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::Write","s","BIO_should_retry test failed on BIO_write");
				failure=false;
				break;
			}
			else
			{
				Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::Write","s","BIO_should_retry allowed");
				continue;
				/* Any more checks for the reason for the retry and handling goes here */
			}
		}
		else if ( ret<buffer.length() )
		{
			buffer=&buffer[ret];
			continue;
		}
		else
		{
			failure=false;
		}
	} while ( failure );

	return true;
}
Meow::Core::Peers::OSSLChunk_ptr Meow::Core::Peers::OSSL::Read( int bufferlength )
{
	Meow::Core::Peers::OSSLChunk_ptr retval=new Meow::Core::Peers::OSSLChunk();
		retval->length=0;
		retval->bytes=(unsigned char*)calloc(bufferlength,1);
		int ret=0;
		do
		{
			bufferlength-=ret;
			ret = BIO_read( sbio, &retval->bytes[retval->length], bufferlength );
			if (ret > 0)
			{
				Core::Log::GetLog()->Error("Continue:","s",retval->bytes);
				retval->length+=ret;
				retval->continueit=true;
				continue;
			}
			else if(ret == 0)
			{
				retval->length+=ret;
				Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::Read","s","Reached the end of the data stream");
				//continue;
				 retval->continueit=false;
				 break;

			}
			else if (ret < 0)
			{
				if (!BIO_should_retry(sbio))
				{
					Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::Read","s","BIO_should_retry test failed in BIO_read");
					retval->continueit=false;
					break;
				}
				else
				{
					/* Any more checks for the reason for the retry and handling goes here */
					Core::Log::GetLog()->Error("Meow::Core::Peers::OSSL::Read","s","BIO_should_retry allowed");
					retval->continueit=true;
					ret=0;
					continue;
				}

			}

		 }
		 while( true );
	return retval;
}
std::string Meow::Core::Peers::OSSL::MD5( std::string in )
{
	char buffer[256];
	unsigned char md[16];
	strcpy(buffer,"");
	Core::Storage::Directory_ptr dir=Core::Storage::Directory::GetDirectoryBase("data.temp");

	Core::Storage::File_ptr tempFile=dir->GetFile("tempdata." + Core::Types::ValueType::IntegerToString(dir->GetProcessID()) + ".tmp",true);
	tempFile->Clear();
	tempFile->OpenForWriting();
	tempFile->WriteBytes(new Core::Types::String(in));
	tempFile->Close();
	std::string filename=tempFile->GetDirName();
	MD5_CTX *c;
	MD5_Init(c);
    MD5_Update(c, in.c_str(), sizeof(in));
	MD5_Final(md, c);
	for ( int i=0; i<16; i++ )
		sprintf(buffer,"%s%02x",buffer,md[i]);
	return buffer;
}
std::string Meow::Core::Peers::OSSL::_SHA256( std::string in )
{
	char buffer[256];
		unsigned char md[32];
		strcpy(buffer,"");
		SHA256((const unsigned char*)in.c_str(),in.length()+1,md);
		for ( int i=0; i<32; i++ )
		  sprintf(buffer,"%s%02x",buffer,md[i]);

		return (const char*)buffer;
}
std::string Meow::Core::Peers::OSSL::_SHA1( std::string in )
{
	char buffer[256];
	unsigned char md[32];
	strcpy(buffer,"");
	SHA1((const unsigned char*)in.c_str(),in.length(),md);
	for ( int i=0; i<20; i++ )
	  sprintf(buffer,"%s%02x",buffer,md[i]);

	return (const char*)buffer;
}
void * Meow::Core::Peers::OSSL::RawMD5( std::string in )
{
	unsigned char * md;
	md=(unsigned char *)malloc(16);
	Core::Storage::Directory_ptr dir=Core::Storage::Directory::GetDirectoryBase("data.temp");
	Core::Storage::File_ptr tempFile=dir->GetFile("tempdata." + Core::Types::ValueType::IntegerToString(dir->GetProcessID()) + ".tmp",true);
	tempFile->Clear();
	tempFile->OpenForWriting();
	tempFile->WriteBytes(new Core::Types::String(in));
	tempFile->Close();
	std::string filename=tempFile->GetDirName();
	MD5_CTX *c;
	MD5_Init(c);
	MD5_Update(c, in.c_str(), sizeof(in));
	MD5_Final(md, c);
	return md;
}
void * Meow::Core::Peers::OSSL::RawSHA256( std::string in )
{
	unsigned char * buffer;
	buffer=(unsigned char *)malloc(32);
	SHA256((const unsigned char*)in.c_str(),in.length(),buffer);
	return buffer;
}
void * Meow::Core::Peers::OSSL::RawSHA1( std::string in )
{
	unsigned char * buffer;
	buffer=(unsigned char *)malloc(20);
	SHA1((const unsigned char*)in.c_str(),in.length(),buffer);
	return buffer;
}
std::string Meow::Core::Peers::OSSL::SHA256HMAC( std::string in, std::string secret )
{
	char output[32];
	hmac_sha256((const unsigned char*)secret.c_str(),secret.length(),(const unsigned char*)in.c_str(),in.length(),(unsigned char*)output,0);
	char * base64_output=Core::Peers::Base64::encodeBase64(32,(unsigned char*)output);
	std::string retval=base64_output;
	free(base64_output);

	return retval;
}
