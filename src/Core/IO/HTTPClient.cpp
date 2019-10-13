// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.17
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/IO/HTTPClient.h>
#include <string.h>
#include <Core/Log.h>

#define BUFFER_LENGTH 3276800

Meow::Core::IO::HTTPAttachment::HTTPAttachment()
{
}
Meow::Core::IO::HTTPAttachment::~HTTPAttachment()
{
}

Meow::Core::IO::HTTPClient::HTTPClient()
{
    SetMeowClassName("HTTPClient");
}
Meow::Core::IO::HTTPClient::~HTTPClient()
{
}
bool Meow::Core::IO::HTTPClient::Get( std::string url,
                              std::string vhost,
                              Meow::Core::Types::String_ptr document )
{
    bool retval=false;
    try
    {
        std::string request=std::string( "GET /") + url + " HTTP/1.0\r\n" +
                            "Host: " + vhost + std::string("\r\n\r\n");

        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            WriteBytes(request.length(),(void*)request.c_str());
            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                std::string temp=buffer;
				Core::Log::GetLog()->Data("Meow::HTTPClient::Get","ss","Read:",temp.c_str());
				
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s",respcode.c_str());
                            return false;
                        }
						else
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Response code fine.");

                    }
                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
						document->Append(temp);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
							document->Append(temp);
                        }
                    }
                }
                else
                    document->Append(temp);
            }
        }
        catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::Get( std::string url,
                              std::string vhost,
                               Meow::Core::Tools::XMLParser_ptr parser )
{
    bool retval=false;
    try
    {
        std::string request=std::string( "GET /") + url + " HTTP/1.0\r\n" +
                            "Host: " + vhost + std::string("\r\n\r\n");
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            WriteBytes(request.length()+1,(void*)request.c_str());
	 
            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s",respcode.c_str());
                            return false;
                        }
                    }

                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        breakit=parser->ParsePartString(temp,false);
                    }
                    else
                    {
                        offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            breakit=parser->ParsePartString(temp,false);
                            inheader=false;
                        }
                    }
                }
                else
                {
                    breakit=parser->ParsePartString(temp,false);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::Get(
                              std::string url,
                              std::string vhost,
                              Meow::Core::Storage::File_ptr parser )
{
    bool retval=false;
    try
    {
        std::string request=std::string( "GET /") + url + " HTTP/1.0\r\n" +
                            "host: " + vhost + std::string("\r\n\r\n");

        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            WriteBytes(request.length(),(void*)request.c_str());
            unsigned long count=0;
            while (breakit)
            {
                memset(buffer, 0, BUFFER_LENGTH);
                unsigned long real_length=ReadBytes(BUFFER_LENGTH,buffer,false);
                count=count+real_length;
                Meow::Core::Types::Void_ptr clientptr=new Core::Types::Void(buffer,real_length);

                if ( inheader )
                {
                    char copybuffer[BUFFER_LENGTH];
                    memcpy(copybuffer,buffer,BUFFER_LENGTH);
                    std::string temp=copybuffer;
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s",respcode.c_str());
                            return false;
                        }
                    }

                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        char * dptr=(char*)buffer;
                        dptr=&dptr[offset+4];
                        unsigned long length=real_length-offset-4;
                        if ( length>0 )
                        {
                            Meow::Core::Types::Void_ptr trimmed=new Core::Types::Void(dptr,length);
                            parser->WriteBinaryBytes(trimmed);
                        }
                        inheader=false;
                    }
                }
                else
                {
                    parser->WriteBinaryBytes(clientptr);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::Get( std::string url,
                              std::string vhost,
                               Meow::Core::Tools::JSONParser_ptr parser )
{
    bool retval=false;
    std::string doc;
          
    try
    {
        std::string request=std::string( "GET /") + url + " HTTP/1.0\r\n" +
                            "host: " + vhost + std::string("\r\n\r\n");
                            
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            WriteBytes(request.length(),(void*)request.c_str());
            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s",respcode.c_str());
                            return false;
                        }
                    }

                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        // breakit=parser->ParsePartString(temp,false);
                        doc.append(temp);
                    }
                    else
                    {
                        offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            // breakit=parser->ParsePartString(temp,false);
                            doc.append(temp);
                            inheader=false;
                        }
                    }
                }
                else
                {
                    // breakit=parser->ParsePartString(temp,false);
                    doc.append(temp);
                }
            }
           
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    retval=parser->ParseString(doc);
    return true;
}
bool Meow::Core::IO::HTTPClient::Post( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr attachments,
                               Meow::Core::Types::String_ptr outputdocument )
{
    std::string boundary="bbBoundaryaa";
    std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + "\nContent-Type: multipart/form-data; boundary=" + boundary + eol;
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                "\"; filename=\""
                + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol + eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());

            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }

                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        outputdocument->Append(temp);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                            outputdocument->Append(temp);
                        }

                    }
                }
                else
                {
                    outputdocument->Append(temp);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::Post( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr attachments,
                                Meow::Core::Tools::XMLParser_ptr parser )
{
    std::string boundary="bbBoundaryaa";
    std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + "\nContent-Type: multipart/form-data; boundary=" + boundary + eol;
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol + eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());

            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);

                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
					size_t offset=temp.find("<?xml");
					if ( offset!=string::npos )
					{
						temp=temp.substr(offset);
						inheader=false;
					}
                }
				if ( !inheader )
                {
                    breakit=parser->ParsePartString(temp,false);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::Post( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr attachments,
                               Meow::Core::Storage::File_ptr parser )
{
    std::string boundary="bbBoundaryaa";
    std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + "\nContent-Type: multipart/form-data; boundary=" + boundary + eol;
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol + eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());

            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                Meow::Core::Types::Void_ptr clientptr=new Core::Types::Void(buffer,BUFFER_LENGTH);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }

                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        parser->WriteBinaryBytes(clientptr);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                            parser->WriteBinaryBytes(clientptr);
                        }
                    }
                }
                else
                {
                    parser->WriteBinaryBytes(clientptr);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::Post( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr attachments,
                                Meow::Core::Tools::JSONParser_ptr parser )
{
    std::string boundary="bbBoundaryaa";
    std::string typeHyphens="--";
    std::string eol="\r\n";
    std::string doc;
    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + "\nContent-Type: multipart/form-data; boundary=" + boundary + eol;
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol + eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());
            
            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);

                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
					size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        doc.append(temp);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                            doc.append(temp);
                        }

                    }
                }
				if ( !inheader )
                {
                	doc.append(temp);
                    // breakit=parser->ParsePartString(temp,false);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return parser->ParseString(doc);
}
bool Meow::Core::IO::HTTPClient::PostCustom( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr headers,
							   std::string request,
                               Meow::Core::Types::String_ptr outputdocument )
{
//    std::string boundary="bbBoundaryaa";
  //  std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + eol;
		Core::Types::Stack_ptr keys=headers->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
			Core::Types::String_ptr value=(Core::Types::String_ptr)headers->Get(key->GetValueAsString());
			header.append(key->GetValueAsString() + ": " + value->GetValueAsString() + eol);
		}

        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
             header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            Core::Log::GetLog()->Data("Meow::HTTPClient::PostCustom","sss","Write[",request.c_str(),"]");
            WriteBytes(request.length(),(void*)request.c_str());

            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                std::string temp=buffer;
				Core::Log::GetLog()->Data("Meow::HTTPClient::Post","sss","Read[",temp.c_str(),"]");
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
						else
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Response code fine.");

                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }

                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                       	Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Out of header [rfc].");
                        outputdocument->Append(temp);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                       		Core::Log::GetLog()->Data("Meow::HTTPClient::Get","s","Out of header [unix].");
                     	     temp=temp.substr(offset+4);
                            inheader=false;
                            outputdocument->Append(temp);
                        }

                    }
                }
                else
                {
                    outputdocument->Append(temp);
                }
            }
        } catch ( SystemException &e )
        {
			Core::Log::GetLog()->Data("Meow::HTTPClient::Get","ss","Caught System Exception [1]:",e.Diagnostic().c_str());
                     	   
        }
    } catch ( SystemException &e )
    {
			Core::Log::GetLog()->Data("Meow::HTTPClient::Get","ss","Caught System Exception [2]:",e.Diagnostic().c_str());
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::PostCustom( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr headers,
							   std::string request,
                                Meow::Core::Tools::XMLParser_ptr parser )
{
   
    std::string eol="\r\n";
    std::string temp;
    
    char buffer[BUFFER_LENGTH+1];
    strcpy(buffer,"");
    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + eol;
		Core::Types::Stack_ptr keys=headers->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
			Core::Types::String_ptr value=(Core::Types::String_ptr)headers->Get(key->GetValueAsString());
			header.append(key->GetValueAsString() + ": " + value->GetValueAsString() + eol);
		}

        try
        {
            bool breakit=true;
            bool inheader=true;
            std::string temp;
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());

            while (breakit)
            {
                strcpy(buffer,"");
                ReadBytes(BUFFER_LENGTH,buffer);
                

                temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
					size_t offset=temp.find("\n\n");
					if ( offset==string::npos )
					{
						offset=temp.find("\r\n\r\n");
						if ( offset!=string::npos )
						{
							temp=temp.substr(offset+4);
							inheader=false;
							// cout << "I've discovered the end of the header" << endl;
					
						}
						// else
							// cout << "Not at end of the header" << endl;
					}
					else
					{
						temp=temp.substr(offset+2);
						inheader=false;
//						cout << "I've discovered the end of the header" << endl;
					
					}
					
                }
                
				if ( !inheader )
                {
                	// cout << "Not in header buffer:" << temp << endl;
                	// cout << "Not in header" << endl;
                    breakit=parser->ParsePartString(temp,false);
                    if ( !breakit )
                    {
                    	Core::Log::GetLog()->Error("Meow::Core::IO::HTTPClient::PostCustom","sds",
                    		"XML Parsing Error:",
                    		parser->GetLineNumber(),
                    		" error:",
                    		parser->GetLastError().c_str());
  //                  	cout << "Oh dear!" << parser->GetLineNumber() << ":" << parser->GetLastError() <<  endl;
                    }
    //                cout << "error:" << temp << endl;
                    temp="";
                }
            }
        } catch ( SystemException &e )
        {
      //  	cout << "System Exception!" << e.Diagnostic() <<  endl;
        	temp=buffer;
        	size_t offset=temp.find("<");
			if ( offset!=string::npos )
			{
				temp=temp.substr(offset);
			}
       // 	cout << "Buffer:" << temp << endl;
        	if ( !parser->ParsePartString(temp,true) )
        	{
                    	Core::Log::GetLog()->Error("Meow::Core::IO::HTTPClient::PostCustom","sds",
                    		"XML Parsing Error:",
                    		parser->GetLineNumber(),
                    		" error:",
                    		parser->GetLastError().c_str());
                    	// cout << "Oh dear!" << parser->GetLineNumber() << ":" << parser->GetLastError() <<  endl;
            }
        }
    } catch ( SystemException &e )
    {
    	// cout << "Parent system exception." << endl;
        	
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::PostCustom( std::string url,
                               std::string vhost,
                               Meow::Core::Types::Array_ptr headers,
							   std::string request,
                               Meow::Core::Storage::File_ptr parser )
{
        std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + eol;
		Core::Types::Stack_ptr keys=headers->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
			Core::Types::String_ptr value=(Core::Types::String_ptr)headers->Get(key->GetValueAsString());
			header.append(key->GetValueAsString() + ": " + value->GetValueAsString() + eol);
		}

        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());


            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                Meow::Core::Types::Void_ptr clientptr=new Core::Types::Void(buffer,BUFFER_LENGTH);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        parser->WriteBinaryBytes(clientptr);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                            parser->WriteBinaryBytes(clientptr);
                        }
                    }
                }
                else
                {
                    parser->WriteBinaryBytes(clientptr);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}

bool Meow::Core::IO::HTTPClient::PostCustom( std::string url,
                               std::string vhost,
                                Meow::Core::Types::Array_ptr headers,
							   std::string request,
                                Meow::Core::Tools::JSONParser_ptr parser )
{
   
    std::string eol="\r\n";
    std::string doc="";
    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "host: " + vhost + eol;
		Core::Types::Stack_ptr keys=headers->GetKeys();
		while ( !keys->IsEmpty() )
		{
			Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
			Core::Types::String_ptr value=(Core::Types::String_ptr)headers->Get(key->GetValueAsString());
			header.append(key->GetValueAsString() + ": " + value->GetValueAsString() + eol);
		}

        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            request=header + request;
            WriteBytes(request.length(),(void*)request.c_str());

            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);

                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
//					size_t offset=temp.find("<?xml");
	//				if ( offset!=string::npos )
		//			{
			//			temp=temp.substr(offset);
				//		inheader=false;
			//		}
                }
				if ( !inheader )
                {
               //     breakit=parser->ParsePartString(temp,false);
               		doc.append(temp);
                }
            }
        } catch ( SystemException &e )
        {

        }
    } catch ( SystemException &e )
    {
    }
    return parser->ParseString(doc);
}

bool Meow::Core::IO::HTTPClient::PostWithParameters(
        std::string url,
        std::string vhost,
         Meow::Core::Types::Array_ptr attachments,
         Meow::Core::Types::Array_ptr parameters,
        Meow::Core::Types::String_ptr outputdocument )
{
    std::string boundary="---------------------------101161224926667";
    std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "Host: " + vhost + eol + "Content-Type: multipart/form-data; boundary=" + boundary + eol;
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            Meow::Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
                Meow::Core::Types::String_ptr key=(Meow::Core::Types::String_ptr)keys->Pop();
                Meow::Core::Types::ValueType_ptr value=(Meow::Core::Types::ValueType_ptr)parameters->Get(key->GetValue());
                std::string parameter= typeHyphens + boundary + eol + "Content-Disposition: form-data; name=\"" + key->GetValue() + "\";" + eol +
                                       "Content-Type: text/plain" + eol + eol +
                                       value->GetValueAsString() + eol;
                request.append(parameter);
            }
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
			header.append(request);
			
            WriteBytes(header.length(),(void*)header.c_str());
  //          WriteBytes(request.length(),(void*)request.c_str());
            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        outputdocument->Append(temp);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                            outputdocument->Append(temp);
                        }

                    }
                }
                else
                {
                    outputdocument->Append(temp);
                }
            }
        } catch ( SystemException &e )
        {

        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::PostWithParameters(
		std::string url,
        std::string vhost,
         Meow::Core::Types::Array_ptr attachments,
         Meow::Core::Types::Array_ptr parameters,
         Meow::Core::Tools::XMLParser_ptr parser )
{
    std::string boundary="---------------------------101161224926667";
    std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
       std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "Host: " + vhost + eol + "Content-Type: multipart/form-data; boundary=" + boundary + eol;
       try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            Meow::Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
                Meow::Core::Types::String_ptr key=(Meow::Core::Types::String_ptr)keys->Pop();
                Meow::Core::Types::ValueType_ptr value=(Meow::Core::Types::ValueType_ptr)parameters->Get(key->GetValue());
                std::string parameter= typeHyphens + boundary + eol + "Content-Disposition: form-data; name=\"" + key->GetValue() + "\";" + eol +
                                       "Content-Type: text/plain" + eol + eol +
                                       value->GetValueAsString() + eol;
                request.append(parameter);
            }
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            header.append(request);
			
            WriteBytes(header.length(),(void*)header.c_str());
            while (breakit)
            {
            	ReadBytes(BUFFER_LENGTH,buffer);
                Meow::Core::Types::Void_ptr clientptr=new Core::Types::Void(buffer,BUFFER_LENGTH);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                        }
                    }
                }
				if ( !inheader )
                {
					size_t offset=temp.find("<?xml");
					if ( offset!=string::npos )
                    {
						temp=temp.substr(offset);
					}
                    breakit=parser->ParsePartString(temp,false);
					if (!breakit )
					{
						Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","sssl","Failure parsing data ",parser->GetLastError().c_str()," at position ",parser->GetLineNumber());
					}
                }
            }
        } catch ( SystemException &e )
        {
        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","System Exception (1)");

        }
    } catch ( SystemException &e )
    {
    	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","System Exception (2)");
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::PostWithParameters(
        std::string url,
        std::string vhost,
        Meow::Core::Types::Array_ptr attachments,
        Meow::Core::Types::Array_ptr parameters,
        Meow::Core::Storage::File_ptr parser )
{
    std::string boundary="---------------------------101161224926667";
    std::string typeHyphens="--";
    std::string eol="\r\n";

    try
    {
        std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "Host: " + vhost + eol + "Content-Type: multipart/form-data; boundary=" + boundary + eol;
        try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            Meow::Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
                Meow::Core::Types::String_ptr key=(Meow::Core::Types::String_ptr)keys->Pop();
                Meow::Core::Types::ValueType_ptr value=(Meow::Core::Types::ValueType_ptr)parameters->Get(key->GetValue());
                std::string parameter= typeHyphens + boundary + eol + "Content-Disposition: form-data; name=\"" + key->GetValue() + "\";" + eol +
                                       "Content-Type: text/plain" + eol + eol +
                                       value->GetValueAsString() + eol;
                request.append(parameter);
            }
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            header.append(request);
			
            WriteBytes(header.length(),(void*)header.c_str());
            while (breakit)
            {
                ReadBytes(BUFFER_LENGTH,buffer);
                Meow::Core::Types::Void_ptr clientptr=new Core::Types::Void(buffer,BUFFER_LENGTH);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","ss","Error during response code ",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                        parser->WriteBinaryBytes(clientptr);
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                            parser->WriteBinaryBytes(clientptr);
                        }
                    }
                }


                else
                {
                    parser->WriteBinaryBytes(clientptr);
                }
            }
        } catch ( SystemException &e )
        {
        }
    } catch ( SystemException &e )
    {
    }
    return true;
}
bool Meow::Core::IO::HTTPClient::PostWithParameters(
		std::string url,
        std::string vhost,
         Meow::Core::Types::Array_ptr attachments,
         Meow::Core::Types::Array_ptr parameters,
         Meow::Core::Tools::JSONParser_ptr parser )
{
    std::string boundary="---------------------------101161224926667";
    std::string typeHyphens="--";
    std::string eol="\r\n";
    std::string doc;

    try
    {
       std::string header=std::string( "POST /") + url + " HTTP/1.0" + eol +
                           "Host: " + vhost + eol + "Content-Type: multipart/form-data; boundary=" + boundary + eol;
       try
        {
            bool breakit=true;
            bool inheader=true;
            char buffer[BUFFER_LENGTH];
            std::string request;
            Meow::Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
                Meow::Core::Types::String_ptr key=(Meow::Core::Types::String_ptr)keys->Pop();
                Meow::Core::Types::ValueType_ptr value=(Meow::Core::Types::ValueType_ptr)parameters->Get(key->GetValue());
                std::string parameter= typeHyphens + boundary + eol + "Content-Disposition: form-data; name=\"" + key->GetValue() + "\";" + eol +
                                       "Content-Type: text/plain" + eol + eol +
                                       value->GetValueAsString() + eol;
                request.append(parameter);
            }
            for ( int i=0; i<attachments->Count(); i++ )
            {
                HTTPAttachment_ptr attachment=(HTTPAttachment_ptr)attachments->Get(i);
                std::string header= typeHyphens + boundary + eol  +  "Content-Disposition: form-data; name=\"" + attachment->name +
                                    "\"; filename=\"" + attachment->filename + "\"" + eol + "Content-Type: " + attachment->content_type + eol + eol;
                request.append(header);
                request.append(attachment->data);
                request.append(eol);
            }
            request.append(typeHyphens + boundary + typeHyphens + eol);
            char buffdata[256];
            sprintf(buffdata,"Content-Length: %d",(int)request.length());
            header.append(buffdata);
            header.append(eol);
            header.append(eol);
            header.append(request);
			
            WriteBytes(header.length(),(void*)header.c_str());
            while (breakit)
            {
            	ReadBytes(BUFFER_LENGTH,buffer);
                Meow::Core::Types::Void_ptr clientptr=new Core::Types::Void(buffer,BUFFER_LENGTH);
                std::string temp=buffer;
                if ( inheader )
                {
                    size_t rcode=temp.find("HTTP/1.0");
                    if ( rcode!=string::npos )
                    {
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s",respcode.c_str());
                            return false;
                        }
                    }
                    else
                    {
                        rcode=temp.find("HTTP/1.1");
                        std::string respcode=temp.substr(rcode+9,3);
                        if ( respcode.compare("200") )
                        {
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s","Response code not positive during HTTP I/O");
                        	Core::Log::GetLog()->Data("Meow::HTTPClient::Post","s",respcode.c_str());
                            return false;
                        }
                    }
                    size_t offset=temp.find("\r\n\r\n");
                    if ( offset!=string::npos )
                    {
                        temp=temp.substr(offset+4);
                        inheader=false;
                    }
                    else
                    {
                        size_t offset=temp.find("\n\n");
                        if ( offset!=string::npos )
                        {
                            temp=temp.substr(offset+4);
                            inheader=false;
                        }
                    }
                }
                else 
                {
					doc.append(temp);
                    // breakit=parser->ParsePartString(temp,false);
                }
            }
        } catch ( SystemException &e )
        {
        	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","System Exception (1)");

        }
    } catch ( SystemException &e )
    {
    	Core::Log::GetLog()->Data("Meow::HTTPClient::PostWithParameters","s","System Exception (2)");
    }
    
    return parser->ParseString(doc);
}
