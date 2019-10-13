#include <fstream>
#include <inc/configuration.h>
#include <Core/Types/String.h>
#include <Core/System/LinkLibrary.h>
#include <Core/Types/Array.h>
#include <Core/Log.h>
#include <Core/Time/Date.h>
#include <Core/Types/Integer.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
Configuration::Configuration()
{
    meow_startup=NULL;
    meow_security=NULL;
    meow_end=NULL;
    iCacheChanges=0;
    iCache=new Meow::Core::Types::Array();
    iLibraries=new Meow::Core::Types::Array();
    iExtensions=new Meow::Core::Types::Array();
    iLoaded=false;
}

Configuration_ptr Configuration::GetConfiguration( std::string linkname )
{
    if ( gConfiguration.IsNull() )
    {
        gConfiguration=new Configuration();
        if ( !gConfiguration->iLoaded )
        {
            gConfiguration->InstallFrom(linkname);
            gConfiguration->iLoaded=true;
        }
    }
    return gConfiguration;
}
Configuration::~Configuration()
{
}

void Configuration::MarkUpdated( std::string service )
{
}

long Configuration::LastUpdated( std::string service )
{
    long retval=0;
    return retval;
}

void Configuration::Passivate( )
{
    Rewrite(iCache);
}
void Configuration::Rewrite( Meow::Core::Types::Array_ptr pairs )
{
    ofstream outputfile;
    std::string filename=Get("conf.file") + ".new";
    outputfile.open(filename.c_str(),ios::trunc);
    outputfile.close();
    outputfile.open(filename.c_str(),ios::app);
    Meow::Core::Types::Stack_ptr keys=pairs->GetKeys();
    while ( !keys->IsEmpty() )
    {
        Meow::Core::Types::String_ptr key=(Meow::Core::Types::String_ptr)keys->Pop();
        Meow::Core::Types::String_ptr value=(Meow::Core::Types::String_ptr)pairs->Get(key->GetValue());
    }
    outputfile.close();
    // We really don't want to fail between these two, but we've minimised the
    // operation as much as possible.
    unlink(Get("conf.file").c_str());
    rename(filename.c_str(),Get("conf.file").c_str());
}
void Configuration::InstallFrom( std::string filename )
{
    ifstream inputfile;
    inputfile.open(filename.c_str(),ios::in);
    iCache->Set("conf.file",new Meow::Core::Types::String(filename));
    if ( inputfile.is_open() )
    {
        int lineno=1;
        while ( !inputfile.eof() )
        {
            int  pos;
            string buffer;

            getline(inputfile,buffer);
            if ( buffer.substr(0,1).compare("#") ||
                    buffer.substr(0,1).compare("\n") ||
                    buffer.substr(0,1).compare("\0'") )
            {
                pos=buffer.find_first_of("=");
                if ( pos>0 )
                {
                    string line=buffer;
                    string property=buffer.substr(0,pos);
                    string value=buffer.substr(pos+1);

                    if ( property.compare("extension") )
                    {
                        if ( iCache->IsSet(property) )
                        {
                            Meow::Core::Types::String_ptr valueobject=(Meow::Core::Types::String_ptr)iCache->Get(property);
                           // valueobject->SetValue(valueobject->GetValue() + value);
			    valueobject->SetValue(value);
                        }
                        else
                        {
                        	Meow::Core::Types::String_ptr valueobject=new  Meow::Core::Types::String();
                            valueobject->SetValue(value);
                            iCache->Set(property,(Meow::Core::ptr)valueobject);
                        }
                    }
                    else
                    {
                        AddExtension(value);
                    }
                }
            }
            lineno++;
        }
        inputfile.close();
    }
}
void  Configuration::AddExtension( string definition )
{
    if ( !iExtensions->IsSet(definition) )
    {
    	try
    	{
			Meow::Core::System::LinkLibrary_ptr library=new Meow::Core::System::LinkLibrary();
			library->Install(definition);
			iExtensions->Set(definition,(Meow::Core::ptr)library);

			void (*bootStrap)(void);
			bootStrap=(void (*)())library->GetFunction("Boot");
			bootStrap();

    	} catch ( SystemException &e )   
    	{
    		e.SetMessage("[Configuration] Cannot install extension " + definition);
    		throw e;
    	}
    }
}
bool   Configuration::IsSet( string name )
{
    return iCache->IsSet(name);
}
void   Configuration::UnSet( string name )
{
    iCache->Unset(name);
}
string Configuration::Get( string name )
{
    Meow::Core::Types::String_ptr returnobject=new Meow::Core::Types::String();
    if ( iCache->IsSet(name ) )
        returnobject=(Meow::Core::Types::String_ptr)iCache->Get(name);
    return returnobject->GetValue();
}
void Configuration::Set( string name, string value )
{
    Meow::Core::Types::String_ptr valueobject=new Meow::Core::Types::String();
    valueobject->SetValue(value);

    iCache->Set(name,(Meow::Core::ptr)valueobject);
}
bool Configuration::CheckMandatories()
{
	return true;
}
Meow::Core::Types::Stack_ptr Configuration::GetKeys()
{
    return iCache->GetKeys();
}

Meow::Core::Types::Array_ptr Configuration::GetExtensions( )
{
    return iExtensions;
}

Configuration_ptr Configuration::gConfiguration=(Configuration*)NULL;

