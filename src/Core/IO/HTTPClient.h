// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_HTTPCLIENT_H_
#define _CORE_HTTPCLIENT_H_
#include <Core/Object.h>
#include <Core/IO/Client.h>
#include <Core/Types/String.h>
#include <Core/Tools/XMLParser.h>
#include <Core/Tools/JSONParser.h>
#include <Core/Storage/File.h>

namespace Meow {
	namespace Core {
		namespace IO {
			class HTTPClient;
			typedef Reference<HTTPClient> HTTPClient_ptr;
			class HTTPAttachment;
			typedef Reference<HTTPAttachment> HTTPAttachment_ptr;

			class HTTPAttachment : public Core::Object
			{
			public:
				HTTPAttachment();
				~HTTPAttachment();

				std::string filename;
				std::string content_type;
				std::string data;
				std::string name;
			};

			class HTTPClient : public Client
			{
			public:
				HTTPClient();
				~HTTPClient();


			bool Get( std::string url,
					  std::string vhost,
					  Meow::Core::Types::String_ptr document );
			bool Get( std::string url,
					  std::string vhost,
					  Meow::Core::Tools::XMLParser_ptr parser );
			bool Get( std::string url,
					  std::string vhost,
					  Meow::Core::Storage::File_ptr parser );
			bool Get( std::string url,
					  std::string vhost,
					  Meow::Core::Tools::JSONParser_ptr parser );
			bool PostCustom( std::string url,
					   std::string vhost,
					   Core::Types::Array_ptr headers,
					   std::string body,
					   Meow::Core::Types::String_ptr outputdocument );
			bool PostCustom( std::string url,
					   std::string vhost,						
					   Core::Types::Array_ptr headers,
					   std::string body,
					   Meow::Core::Tools::XMLParser_ptr parser );
			bool PostCustom( std::string url,
					   std::string vhost,
					   Core::Types::Array_ptr headers,
					   std::string body,
					   Meow::Core::Storage::File_ptr parser );
			bool PostCustom( std::string url,
					   std::string vhost,						
					   Core::Types::Array_ptr headers,
					   std::string body,
					   Meow::Core::Tools::JSONParser_ptr parser );
			bool Post( std::string url,
					   std::string vhost,
					   Core::Types::Array_ptr attachments,
					   Meow::Core::Types::String_ptr outputdocument );
			bool Post( std::string url,
					   std::string vhost,
					   Core::Types::Array_ptr attachments,
					   Meow::Core::Tools::XMLParser_ptr parser );
			bool Post( std::string url,
					   std::string vhost,
					   Meow::Core::Types::Array_ptr attachments,
					   Meow::Core::Storage::File_ptr parser );
			bool Post( std::string url,
					   std::string vhost,
					   Core::Types::Array_ptr attachments,
					   Meow::Core::Tools::JSONParser_ptr parser );
			bool PostWithParameters( std::string url,
									 std::string vhost,
									 Meow::Core::Types::Array_ptr attachments,
									 Meow::Core::Types::Array_ptr parameters,
									 Meow::Core::Types::String_ptr outputdocument );
			bool PostWithParameters( std::string url,
									 std::string vhost,
									 Meow::Core::Types::Array_ptr attachments,
									 Meow::Core::Types::Array_ptr parameters,
									 Meow::Core::Tools::XMLParser_ptr parser );
			bool PostWithParameters( std::string url,
									 std::string vhost,
									 Meow::Core::Types::Array_ptr attachments,
									 Meow::Core::Types::Array_ptr parameters,
									 Meow::Core::Storage::File_ptr parser );
			bool PostWithParameters( std::string url,
									 std::string vhost,
									 Meow::Core::Types::Array_ptr attachments,
									 Meow::Core::Types::Array_ptr parameters,
									 Meow::Core::Tools::JSONParser_ptr parser );

			};
		}
	}
}
#endif
