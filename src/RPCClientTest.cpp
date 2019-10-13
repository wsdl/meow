/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#include <inc/configuration.h>
#include <Core/Debug.h>
#include <Core/Types/String.h>
#include <Core/IO/ConnectionManager.h>
#include <Core/IO/Client.h>
#include <Core/Types/Array.h>
#include <Core/Types/Boolean.h>
#include <Registry.h>
#include <LifecycleObject.h>
#include <iostream>
#include <Core/Log.h>
#include <stdio.h>
#include <string.h>
#include <Core/Log.h>
#include <Core/IO/Client.h>
#include <Core/IO/HTTPClient.h>
#include <Core/Client/RemoteCall.h>
#include <Core/Peers/OSSL.h>
using namespace Meow;


int main( int argc, char ** argv)
{

	if ( argc < 4 )
	{
		cerr << argv[0] << " requires three parameters <service> <method> <key> [parameters]" << endl;
		return (int)false;
	}
    Configuration_ptr conf=Configuration::GetConfiguration();
#ifdef WIN32
    // We assume we are running with our working directory set - this should be read
    // from the windows registry
    conf->InstallFrom("etc\\meow\\ini.cnf");
#else
    conf->InstallFrom("etc/ini.cnf");
#endif
    // Reconfigure the log based on what we know from the configuration file.
    Core::Log::GetLog()->Reconfigure();

    // Now lets interpret the arguments
    // argv[1] = Service
    // argv[2] = Method
    // argv[3] = Key
    // argv[4]+ = Parameters
    std::string service=argv[1];
    std::string method=argv[2];
    std::string key=argv[3];
    Core::Types::Array_ptr parameters=new Core::Types::Array();
    for ( int i=4; i<argc; i++ )
    {
    	parameters->Set(parameters->Count(),new Core::Types::String(argv[i]));
    }


    // Install the registry.
    Registry_ptr registry=Registry::GetRegistry();
    Core::IO::HTTPClient_ptr client=new Core::IO::HTTPClient();
	client->SetHostName("127.0.0.1");
	client->SetPortNo(80);
	client->Open();
    Core::Client::RemoteCall_ptr remote=new Core::Client::RemoteCall();
    remote->SetRemoteHost("127.0.0.1");
    remote->SetURL("s");

    Meow::Core::Client::RemoteCallResponse_ptr response=remote->MakeSynchronousRemoteCall(std::string(argv[0]),
						"",
						service,
						method,
						key,
						parameters,
						client);
    if  ( response->GetException().length() > 0 )
    {
    	cerr << response->GetException() << ":" << endl;
    	Core::Types::Array_ptr messages=response->GetExceptionMessages();
    	Core::Types::Stack_ptr messageKeys=messages->GetKeys();
    	while ( ! messageKeys->IsEmpty() )
    	{
    		Core::Types::ValueType_ptr key=(Core::Types::ValueType_ptr)messageKeys->Pop();
    		Core::Types::ValueType_ptr value=(Core::Types::ValueType_ptr)messages->Get(key->GetValueAsString());
    		cerr << " - " << value->GetValueAsString() << endl;
    	}
    }
    if ( response->GetReturned().length() > 0  )
    	cout << response->GetReturned() << endl;
    registry->Passivate();
	
    return (int)true;
}


