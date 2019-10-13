// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/IO/Client.h>
#ifdef WIN32
	#include <windows.h>
	#include <winsock.h>
	#include <winsock2.h>
#else
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <string.h>
	#include <unistd.h>
#endif
#include <iostream>
#include <Core/Log.h>



Meow::Core::IO::Client::Client( ) :
    PortNumber(0),
    sock(-1),
    status(closed),
    Usage(false),
    Strict(true),
    Connected(false),
    Secure(false),
    X509Secure(false)
{
    SetMeowClassName("Core/Client");
    HostNames=new Core::Types::Array();
    PeerContext=new Peers::OSSL();
}

Meow::Core::IO::Client::~Client( )
{
    if ( status!=closed )
    {
        Close();
    }
}

void Meow::Core::IO::Client::NotStrict( )
{
    Strict=false;
}

void Meow::Core::IO::Client::InUse( )
{
    Usage=true;
}

void Meow::Core::IO::Client::SetOnComplete( Meow::Core::Types::Stack_ptr stack, Meow::Core::System::Lock_ptr lock )
{
    OnRelease=stack;
    OnReleaseLock=lock;
}

void Meow::Core::IO::Client::Released( )
{
    Usage=false;
    if ( ! OnRelease.IsNull( ) )
    {
        ptr pushable=this;
        if ( !OnReleaseLock.IsNull() )
            OnReleaseLock->Gain();
        OnRelease->Push(pushable);
        if ( !OnReleaseLock.IsNull() )
            OnReleaseLock->Release();
    }
}
bool Meow::Core::IO::Client::IsSecure()
{
    return Secure;
}
void Meow::Core::IO::Client::MakeSecure()
{
    Secure=true;
}
void Meow::Core::IO::Client::MakeX509Secure()
{
    Secure=true;
    X509Secure=true;
}
void Meow::Core::IO::Client::SetX509CACert( std::string cert )
{
    X509CACert=cert;
}
void Meow::Core::IO::Client::SetX509CLICert( std::string cert )
{
    X509CLICert=cert;
}
void Meow::Core::IO::Client::SetX509PrivateKey( std::string key )
{
    X509PrivateKey=key;
}

bool Meow::Core::IO::Client::IsInUse( )
{
    return Usage;
}

void Meow::Core::IO::Client::SetHostName( std::string hostname )
{
	Core::Types::String_ptr str=new Core::Types::String(hostname);
	Core::Types::Array_ptr hosts=str->Explode(",");
    for ( unsigned long i=0; i<hosts->Count(); i++ )
    {
    	Core::Types::String_ptr val=(Core::Types::String_ptr)hosts->Get(i);
    	Core::Types::String_ptr copy=new Core::Types::String(val->GetValue());
        HostNames->Set(HostNames->Count(),(ptr)copy);
    }
}
std::string Meow::Core::IO::Client::GetActiveHost( )
{
    return iActiveHost;
}

void Meow::Core::IO::Client::SetPortNo( unsigned long portno )
{
    PortNumber=portno;
}

void Meow::Core::IO::Client::Open( )
{
    if ( Connected )
    {
        Core::Log::GetLog()->Data("Meow::Client::Open","s","Already connected");
        return;
    }
    if ( status!=closed )
    {
    	Core::Log::GetLog()->Data("Meow::Client::Open","s","Already connected status is not closed");
        SystemException exception;
        throw exception;
    }
    if ( !IsSecure( ) )
    {
        struct sockaddr_in  address;
        struct in_addr       inaddr;
        struct hostent    *    host;

        Connected=false;
        unsigned long CurrentHost=0;
        while ( !Connected && CurrentHost<HostNames->Count() )
        {
        	Core::Types::String_ptr HostName=(Core::Types::String_ptr)HostNames->Get(CurrentHost);
            host=gethostbyname(HostName->GetValue().c_str());
            if ( host==NULL )
            {
            	Core::Log::GetLog()->Data("Meow::Client::Open","ss","gethostbyname - ",HostName->GetValue().c_str());
            }
            else
            {
                if ((sock=socket(PF_INET,SOCK_STREAM,0))<0 )
                {
                	Core::Log::GetLog()->Data("Meow::Client::Open","s","Could not create socket");
                }
                else
                {
                    int keepalive=1;
#ifndef WIN32
                    setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,&keepalive,sizeof(int));
#else
                    setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,(char *)&keepalive,sizeof(int));
#endif
                    address.sin_family=AF_INET;
                    address.sin_port=htons(PortNumber);
                    memcpy(&address.sin_addr,host->h_addr_list[0],sizeof(address.sin_addr));
                    if (connect(sock,(struct sockaddr * )&address,sizeof(address)))
                    {
                    	Core::Log::GetLog()->Data("Meow::Client::Open","ss","Could not connect to endpoint ",HostName->GetValue().c_str());
                    }
                    else
                    {
                    	Core::Log::GetLog()->Data("Meow::Client::Open","s","Successfully connected");
                        status=open;
                        Connected=true;
                        iActiveHost=HostName->GetValue();
                    }
                }
            }
            CurrentHost++;
        }
        if ( !Connected )
        {
        	Core::Log::GetLog()->Data("Meow::Client::Open","s","Could not connect to endpoint");
            SystemException exception;
            throw exception;
        }
    }
    else
    {
        Connected=false;
        unsigned long CurrentHost=0;
        while ( !Connected && CurrentHost<HostNames->Count() )
        {
        	Core::Types::String_ptr HostName=(Core::Types::String_ptr)HostNames->Get(CurrentHost);
            std::string curhost=HostName->GetValue();
            if ( X509Secure )
            {
            	if ( PeerContext->InitX509(X509CACert,
            			X509CLICert,
            			X509PrivateKey,
            			curhost,
            			PortNumber) )
                {
            		Core::Log::GetLog()->Data("Meow::Client::Open","s","Successfully connected (X509)");
                    status=open;
                    Connected=true;
                    iActiveHost=HostName->GetValue();
                }
                else
                {
                	Core::Log::GetLog()->Data("Meow::Client::Open","ss","Could (X509) not connect to endpoint ",HostName->GetValueAsString().c_str());
                }
            }
            else
            {
                if ( PeerContext->Init( curhost, PortNumber) )
                {
                	Core::Log::GetLog()->Data("Meow::Client::Open","s","Successfully connected (Secure)");
                    status=open;
                    Connected=true;
                    iActiveHost=HostName->GetValue();
                }
                else
                {
                	Core::Log::GetLog()->Data("Meow::Client::Open","ss","Could (Secure) not connect to endpoint ",HostName->GetValueAsString().c_str());
                }
            }
            CurrentHost++;
        }
        if ( !Connected )
        {
        	Core::Log::GetLog()->Data("Meow::Client::Open","s","Could not connect to any endpoint defined under this name.");
            SystemException exception;
            throw exception;
        }
    }
}
void Meow::Core::IO::Client::WriteBytes( unsigned long length, void * buffer )
{
    unsigned long bytessent;
    unsigned long debug;
    status=writing;
    Core::Log::GetLog()->Data("Meow::Client::WriteBytes","issss",length,"bytes","[\n",(char*)buffer,"\n]");
    if ( !Secure )
    {
#ifdef WIN32
        bytessent=send(sock,(const char*)buffer,length,0);
#else
        bytessent=send(sock,buffer,length,0);
#endif
        status=open;
        if ( bytessent!=length )
        {
            status=closed;
            Core::Log::GetLog()->Data("Meow::Client::WriteBytes","s","Could not write the data in plain mode ");
            SystemException exception;
            throw exception;
        }
    }
    else
    {
        int len=(int)length;
        std::string buf=(char*)buffer;
        if ( ! PeerContext->Write(buf) )
        {
        	Core::Log::GetLog()->Data("Meow::Client::WriteBytes","s","Could not write the data in secure mode");
            SystemException exception;
            throw exception;
        }

    }
    Core::Log::GetLog()->Data("Meow::Core::IO::Client::WriteBytes","s","Written");

}
unsigned long Meow::Core::IO::Client::ReadBytes( unsigned long length, void * buffer, bool binary ) 
{
    unsigned long bytessent;
    unsigned long debug;
    bool retval=true;
    char message[256];

    sprintf(message,"Starting to read %d bytes (max)",(int)length);
    Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","sls","Reading ",length," bytes.");
    if ( !Secure )
    {
        status=reading;
#ifdef WIN32
        bytessent=recv(sock,(char*)buffer,length,0);
#else
        bytessent=recv(sock,buffer,length,0);
#endif
    }
    else
    {
        int len=(int)length;
		Meow::Core::Peers::OSSLChunk_ptr buf=PeerContext->Read(len);
		Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","ss","Read:",buf->bytes);
//        buffer=(void*)buf.c_str();
		memcpy(buffer,buf->bytes,buf->length);
		bytessent=buf->length;
		retval=buf->continueit;
    }
    status=open;
    if ( bytessent==0 )
    {
    	Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","s","Read no bytes, stall.");
        SystemException exception;
        throw exception;
    }
    if ( bytessent!=length )
	{

		Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","sls","Only able to read ",bytessent," bytes, you may need to retry.");
		retval=false;
	}


/*    if ( retval && Strict)
    {
        status=closed;
        Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","s","Strict mode and binary underrun.");
        SystemException exception;
        throw exception;
    }
    else */ 
    if ( !binary )
    {
        ((char*)buffer)[bytessent]='\0';
        Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","sss","Non binary[",buffer,"]");
    }
    else
    	Core::Log::GetLog()->Data("Meow::Core::IO::Client::ReadBytes","s","Binary data received of correct length");
    return bytessent;
}
void Meow::Core::IO::Client::Close( )
{
    if ( Connected )
    {
    	Core::Log::GetLog()->Data("Meow::Core::IO::Client::Close","s","Close");
        if ( !Secure )
        {
#ifdef WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            status=closed;
        }
        else
        {
            PeerContext->Close();
            status=closed;
        }
        Core::Log::GetLog()->Data("Meow::Core::IO::Client::Close","s","Closed");
    }
    Connected=false;

}

Meow::Core::IO::ClientStatus Meow::Core::IO::Client::GetStatus( )
{
    return status;
}

bool Meow::Core::IO::Client::IsConnected()
{
    return Connected;
}
