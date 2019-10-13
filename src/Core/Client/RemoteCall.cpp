// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetMeowClassName and GetMeowClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#include <Core/Client/RemoteCall.h>
#include <Core/IO/HTTPClient.h>
#include <Core/Log.h>
#include <Core/Object.h>
#include <Core/IO/Client.h>
#include <Core/Tools/XMLParser.h>
#include <Core/Storage/File.h>
#include <Core/Types/Array.h>
#include <Core/Types/ValueType.h>
#include <Core/Types/Integer.h>
#define BUFFER_LENGTH 32768

Meow::Core::Client::RemoteCall::RemoteCall()
{
    SetMeowClassName("RemoteCall");
    iRemoteHost="127.0.0.1";
    iURL="/cgi-bin/meow.cgi";
}
Meow::Core::Client::RemoteCall::~RemoteCall()
{
}

void Meow::Core::Client::RemoteCall::SetRemoteHost( std::string hostname )
{
    iRemoteHost=hostname;
}

std::string Meow::Core::Client::RemoteCall::GetRemoteHost()
{
    return iRemoteHost;
}

void Meow::Core::Client::RemoteCall::SetURL( std::string url )
{
    iURL=url;
}

std::string Meow::Core::Client::RemoteCall::GetURL( )
{
    return iURL;
}

Meow::Core::Client::RemoteCallResponse_ptr Meow::Core::Client::RemoteCall::MakeSynchronousRemoteCall(
    std::string originator,
    std::string correlationid,
    std::string service,
    std::string method,
    std::string key,
    Core::Types::Array_ptr parameters,
    Core::IO::HTTPClient_ptr connection )
{
    std::string document="<MI><O>" + originator + "</O>" +
                         "<X>" + correlationid + "</X>" +
                         "<S>" + service + "</S>" +
                         "<M>" + method + "</M>" +
                         "<K>" + key + "</K>" +
                         "<PS>";
    for ( int i=0; i<parameters->Count(); i++ )
    {
    	Core::Types::ValueType_ptr plain=(Core::Types::ValueType_ptr)parameters->Get(i);
    	document.append("<P><![CDATA[" + plain->GetValueAsString() + "]]></P>");
    }
    document.append("</PS></MI>");
    Core::Types::Array_ptr httpParms=new Core::Types::Array();
//    httpParms->Set("mode",new Core::Types::String("xmlrpc"));
    Core::Types::Array_ptr httpAttachments=new Core::Types::Array();
    RemoteCallHandler_ptr handler=new RemoteCallHandler();
    Core::IO::HTTPAttachment_ptr attachment=new Core::IO::HTTPAttachment();
    attachment->filename="upload";
    attachment->content_type="text/xml";
    attachment->data=document;
    attachment->name="request";
    try
    {
		httpAttachments->Set(0,(ptr)attachment);
		connection->PostWithParameters(GetURL() + "/xmlrpc",
									   GetRemoteHost(),
									   httpAttachments,
									   httpParms,
									   (Core::Tools::XMLParser_ptr)handler );
    } catch ( SystemException &e )
    {
    	Core::Log::GetLog()->Warning("Meow::Core::Client::RemoteCall::MakeSynchronousRemoteCall","ss","Exception:",e.Diagnostic().c_str());
    }
    return handler->GetResponse();
}

Meow::Core::Client::RemoteCallHandler::RemoteCallHandler()
{
    SetMeowClassName("RemoteCallHandler");
    iResponse=new RemoteCallResponse();

}
Meow::Core::Client::RemoteCallHandler::~RemoteCallHandler()
{
}
void Meow::Core::Client::RemoteCallHandler::StartElement( string element, Core::Types::Array_ptr attributes )
{
    iCurrentElement=element;
    iCurrentCharacterData="";
    iCurrentAttributes=attributes;
}
void Meow::Core::Client::RemoteCallHandler::EndElement( string element )
{
    if ( !element.compare("O") )
        iResponse->iOriginator=iCurrentCharacterData;
    else if ( !element.compare("X") )
        iResponse->iCorrelationID=iCurrentCharacterData;
    else if ( !element.compare("P") )
        iResponse->iPrimeType=iCurrentCharacterData;
    else if ( !element.compare("C") )
        iResponse->iRemoteClassName=iCurrentCharacterData;
    else if ( !element.compare("R") )
    {
        iResponse->iReturned=iCurrentCharacterData;
    }
    else if ( !element.compare("Exception") )
        iResponse->iException=iCurrentCharacterData;
    else if ( !element.compare("Message") )
    {
        iResponse->iExceptionMessages->Set(iResponse->iExceptionMessages->Count(),new Core::Types::String(iCurrentCharacterData));
    }
    else if ( !element.compare("Security") )
    {
    	iResponse->iException="Exceptions/Security";
    	iResponse->iExceptionMessages->Set(iResponse->iExceptionMessages->Count(),new Core::Types::String(iCurrentCharacterData));
    }
    else
    {

    }
}
void Meow::Core::Client::RemoteCallHandler::CharacterData( string data )
{
    iCurrentCharacterData.append(data);
}
Meow::Core::Client::RemoteCallResponse_ptr Meow::Core::Client::RemoteCallHandler::GetResponse()
{
    return iResponse;
}
Meow::Core::Client::RemoteCallResponse::RemoteCallResponse()
{
    SetMeowClassName("RemoteCallResponse");
    iExceptionMessages=new Core::Types::Array();
    iErrorCode=0;
}
Meow::Core::Client::RemoteCallResponse::~RemoteCallResponse()
{
}
std::string Meow::Core::Client::RemoteCallResponse::GetOriginator()
{
    return iOriginator;
}
std::string Meow::Core::Client::RemoteCallResponse::GetCorrelationID()
{
    return iCorrelationID;
}
std::string Meow::Core::Client::RemoteCallResponse::GetPrimeType()
{
    return iPrimeType;
}
std::string Meow::Core::Client::RemoteCallResponse::GetRemoteClassName()
{
    return iRemoteClassName;
}
std::string Meow::Core::Client::RemoteCallResponse::GetReturned()
{
    return iReturned;
}
std::string Meow::Core::Client::RemoteCallResponse::GetException()
{
    return iException;
}
Meow::Core::Types::Array_ptr Meow::Core::Client::RemoteCallResponse::GetExceptionMessages()
{
    return iExceptionMessages;
}
int Meow::Core::Client::RemoteCallResponse::GetErrorCode()
{
    return iErrorCode;
}

