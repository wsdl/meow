// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_TOOLS_CURL_H_
#define _CORE_TOOLS_CURL_H_
#include <Core/Object.h>
#include <Exception.h>
#include <curl/curl.h>

namespace Meow {
	namespace Core {
		namespace Tools {

			class CurlException : public Meow::Exception
			{
			public:
				CurlException();
				~CurlException();
			};
			
			class Curl;
			typedef Reference<Curl> Curl_ptr;
		
			class Curl : public Object
			{
			public:
				Curl( std::string url );
				virtual ~Curl();
				void ConnectOnly( );
				void Perform( );
				std::string SendAndReceive( std::string buffer );
				
				void SkipPeerVerification();
				void SkipHostnameVerification();
				void FormAdd( std::string name, std::string value );
				void HeaderAdd( std::string header );
				
				std::string ConformantPost( std::string buffer, std::string contenttype );
			private:
				Curl();
				CURL * curl;	
				struct curl_httppost *formpost=NULL;
				struct curl_httppost *lastptr=NULL;
				struct curl_slist *headerlist=NULL;			
			};
		}
	}
}
#endif

