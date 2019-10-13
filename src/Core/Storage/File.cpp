// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
//  17/11/2013 @DavidSpencer
//		Moved logic into peers for base64
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Registry.h>
#include <Factory.h>
#include <Core/Time/Date.h>
#include <Core/Peers/Base64.h>
#include <stdio.h>
#include <Core/Storage/File.h>
#include <string.h>
#include <Core/Log.h>
#include <Core/Types/Integer.h>
#include <Core/Storage/Directory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define STATE_CLOSED  0
#define STATE_INVALID 1
#define STATE_READ    2
#define STATE_WRITE   3

Meow::Core::Storage::File::File() : status(STATE_CLOSED)
{
    SetMeowClassName("File");
    absolute_path="";
    descriptor=NULL;
    auto_archive=false;
}

Meow::Core::Storage::File::~File()
{
    if ( status==STATE_CLOSED )
        return;

    if ( descriptor!=NULL & ( status==STATE_WRITE || status==STATE_READ ) )
        fclose((FILE*)descriptor);
    status=STATE_CLOSED;
}

bool Meow::Core::Storage::File::OpenForWriting( )
{
    if ( status==STATE_READ )
    {
    	Core::Log::GetLog()->Error("Meow::Core::Storage::File::OpenForWriting","sss","file ",absolute_path.c_str()," is already open for reading and cannot be opened for writing.");
        return false;
    }
    if ( status==STATE_WRITE )
    {
    	Core::Log::GetLog()->Data("Meow::Core::Storage::File::OpenForWriting","sss","file ",absolute_path.c_str()," is already open for writing.");

    	return true;
    }
#ifdef WIN32
    descriptor=(FILE*)fopen(absolute_path.c_str(),"wb");
#else
//     umask(0777);
    descriptor=(FILE*)fopen(absolute_path.c_str(),"a");
#endif
    if ( descriptor==NULL )
    {
        Core::Log::GetLog()->Error("Meow::Core::Storage::File::OpenForWriting","sssd","file ",absolute_path.c_str()," cannot be opened at the system level, STATE_INVALID errno ",errno);
        status=STATE_INVALID;
    }
    else
        status=STATE_WRITE;
    return status==STATE_WRITE;
}

bool Meow::Core::Storage::File::OpenForReading( )
{
    if ( status==STATE_READ )
    {
    	Core::Log::GetLog()->Data("Meow::Core::Storage::File::OpenForReading","sss","file ",absolute_path.c_str()," is already open for reading.");
        return true;
    }
    if ( status==STATE_WRITE )
    {
    	Core::Log::GetLog()->Error("Meow::Core::Storage::File::OpenForReading","sss","file ",absolute_path.c_str()," is already open for writing and cannot be opened for reading.");

        return false;
    }
    // First check to see if there is a file absolute_path + . + mta
    // if there is, call download and use the resultant file descriptor
    std::string metaPath=absolute_path + ".mta";

    descriptor=(FILE*)fopen(metaPath.c_str(),"r");
    if ( descriptor==NULL )
    {
        descriptor=(FILE*)fopen(absolute_path.c_str(),"r");
        if ( descriptor==NULL )
        {
            status=STATE_INVALID;
            Core::Log::GetLog()->Error("Meow::Core::Storage::File::OpenForReading","sss","file ",absolute_path.c_str()," cannot be opened at the system level, STATE_INVALID.");
        }
        else
            status=STATE_READ;
    }
    else
    {
    	// TODO - implement download
    }

    return status==STATE_READ;
}


bool Meow::Core::Storage::File::Clear( )
{
    // close the file if open, then do a "w"
    // then close it
    if ( status==STATE_READ || status==STATE_WRITE )
    {
        if ( descriptor!=NULL )
            fclose((FILE*)descriptor);
        descriptor=NULL;
    }
    status=STATE_CLOSED;
    descriptor=(FILE*)fopen(absolute_path.c_str(),"w");
    if ( descriptor!=NULL )
        fclose((FILE*)descriptor);
    else
        return false;

    return true;
}

bool Meow::Core::Storage::File::RewindToTop( )
{
    // if ( status!=STATE_READ )
    //    return false;
    // cout << "RewindToTop" << endl;
    fseek((FILE*)descriptor,0L,SEEK_SET);
    return true;
}
// size_t written=fwrite(data->GetValue().c_str(),1,data->GetValue().length(),(FILE*)descriptor);
bool Meow::Core::Storage::File::EoF( )
{
    if ( status!=STATE_READ )
        return true;
    return feof((FILE*)descriptor);
}

Meow::Core::Types::String_ptr Meow::Core::Storage::File::ReadBytes( unsigned int bytelength )
{
    Meow::Core::Types::String_ptr retval=new Meow::Core::Types::String();
    if ( status==STATE_READ )
    {
        char * buffer=(char*)malloc(bytelength+1);
        size_t bufflength;

        bufflength=fread(buffer,1,bytelength,(FILE*)descriptor);
        buffer[bufflength]='\0';
        retval->SetValue(buffer);
        free(buffer);
    }
    return retval;
}

Meow::Core::Types::Void_ptr Meow::Core::Storage::File::ReadBinaryBytes( unsigned int bytelength )
{
    Meow::Core::Types::Void_ptr retval;
    if ( status==STATE_READ )
    {
        char * buffer=(char*)malloc(bytelength+1);
        size_t bufflength;

        bufflength=fread(buffer,1,bytelength,(FILE*)descriptor);
        retval=new Core::Types::Void(buffer,bufflength);
    }
    return retval;
}

void Meow::Core::Storage::File::WriteBytes( Meow::Core::Types::String_ptr data )
{
    if ( status==STATE_WRITE && data->GetValueAsString().length()>0 )
    {
        size_t written=fwrite(data->GetValueAsString().c_str(),1,data->GetValueAsString().length(),(FILE*)descriptor);
        if ( written!=data->GetValueAsString().length() )
        	Core::Log::GetLog()->Error("Meow::File::WriteBytes","s","Could not write as much data as requested.");
    }
    else
    	Core::Log::GetLog()->Error("Meow::File::WriteBytes","s","Could not write data as file was not open for writing or length was 0");
}

void Meow::Core::Storage::File::WriteBinaryBytes( Meow::Core::Types::Void_ptr data )
{
	Meow::Core::Types::Integer_ptr length=new Meow::Core::Types::Integer(data->GetVoidLength());
    if ( status==STATE_WRITE && data->GetVoidLength()>0 )
        fwrite(data->GetValueAsVoid(),1,data->GetVoidLength(),(FILE*)descriptor);
    else
    {
    	Core::Log::GetLog()->Error("Meow::File::WriteBinaryBytes","s","Could not write data as file was not open for writing or length was 0");
        if ( status!=STATE_WRITE )
        	Core::Log::GetLog()->Error("Meow::File::WriteBinaryBytes","s","Not open for writing");
        if ( data->GetVoidLength()==0 )
        	Core::Log::GetLog()->Error("Meow::File::WriteBinaryBytes","s","Zero length data");
    }
}
void Meow::Core::Storage::File::WriteValueType( Meow::Core::Types::ValueType_ptr data )
{
    if ( status==STATE_WRITE && data->GetValueAsString().length()>0 )
    {
        size_t written=fwrite(data->GetValueAsString().c_str(),1,data->GetValueAsString().length(),(FILE*)descriptor);
        if ( written!=data->GetValueAsString().length() )
        	Core::Log::GetLog()->Error("Meow::File::WriteValueType","s","Could not write as much data as requested.");
    }
    else
    	Core::Log::GetLog()->Error("Meow::File::WriteValueType","s","Could not write data as file was not open for writing or length was 0");
}
void Meow::Core::Storage::File::Close()
{
    if ( status==STATE_CLOSED )
        return;

    if ( descriptor!=NULL & ( status==STATE_WRITE || status==STATE_READ ) )
        fclose((FILE*)descriptor);
    descriptor=NULL;
    status=STATE_CLOSED;
}

Meow::Core::Types::String_ptr Meow::Core::Storage::File::GetContents()
{
    Close();
    Core::Types::String_ptr retval=new Core::Types::String();
    if ( OpenForReading() )
    {
        while ( !EoF() )
            retval->Append(ReadBytes(4096));
    }
    else
    	Core::Log::GetLog()->Error("Meow::File::GetContents","s","File is not open for read or write.");
    return retval;
}

long Meow::Core::Storage::File::Count()
{
    long size=0;
    if ( OpenForReading() || OpenForWriting() )
    {
        long position=ftell((FILE*)descriptor);
        fseek((FILE*)descriptor,0,SEEK_END);
        size=ftell((FILE*)descriptor);
        fseek((FILE*)descriptor,0,SEEK_SET);
    }
    else
    	Core::Log::GetLog()->Error("Meow::File::Count","s","File is not open for read or write.");

    return size;
}
void Meow::Core::Storage::File::WriteLine( Meow::Core::Types::String_ptr line )
{
    if ( OpenForWriting() )
    {
        char * buffer=(char*)"\n";
        fwrite(line->GetValue().c_str(),1,line->GetValue().length(),(FILE*)descriptor);
        fwrite(buffer,1,1,(FILE*)descriptor);
    }
    else
    	Core::Log::GetLog()->Error("Meow::File::Count","s","File is not open for writing.");
}
Meow::Core::Types::String_ptr Meow::Core::Storage::File::ReadLine( )
{
	Core::Types::String_ptr retval=new Core::Types::String();
    if ( OpenForReading() )
    {
        char buffer[1048576];
        strcpy(buffer,"");
        fgets(buffer,1048576,(FILE*)descriptor);
        int buflen=strlen(buffer);
        if ( buflen>1 )
        {
            if ( buffer[buflen-1]=='\n' )
                buffer[buflen-1]='\0';
        }
        retval->SetValue(buffer);
    }
    else
       	Core::Log::GetLog()->Error("Meow::File::ReadLine","s","File is not open for read.");
    return retval;
}
Meow::Core::Types::String_ptr Meow::Core::Storage::File::ReadLineEx( )
{
	Core::Types::String_ptr retval=new Core::Types::String();
    if ( OpenForReading() )
    {
        char buffer[4096];
        strcpy(buffer,"");
        fgets(buffer,4096,(FILE*)descriptor);
        int buflen=strlen(buffer);
        if ( buflen>1 )
        {
            if ( buffer[buflen-1]=='\n' )
                buffer[buflen-1]='\0';
            if ( buffer[buflen-2]=='\r' )
                buffer[buflen-2]='\0';
        }
        retval->SetValue(buffer);
    }
    else
       	Core::Log::GetLog()->Error("Meow::File::ReadLineEx","s","File is not open for read.");
    return retval;
}

void Meow::Core::Storage::File::Base64EncodeFile( std::string source, std::string target )
{
    FILE * in=(FILE*)fopen(source.c_str(),"r");
    FILE * out=(FILE*)fopen(target.c_str(),"wb");

    int slen;
    fseek(in,0,SEEK_END);
    slen=ftell(in);
    fseek(in,0,SEEK_SET);
    unsigned char *buffer=(unsigned char*)malloc(slen);
    fread(buffer,1,slen,in);

    char * outbuffer=Meow::Core::Peers::Base64::encodeBase64(slen,buffer);
    fwrite(outbuffer,1,strlen(outbuffer)+1,out);
    free(buffer);
    free(outbuffer);
    fclose(in);
    fclose(out);
    chmod(target.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH); 
}
void Meow::Core::Storage::File::Base64DecodeFile( std::string source, std::string target )
{
    umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    unlink(target.c_str());
    FILE * in=(FILE*)fopen(source.c_str(),"r");
    FILE * out=(FILE*)fopen(target.c_str(),"wb");

    int slen;
    fseek(in,0,SEEK_END);
    slen=ftell(in);
    fseek(in,0,SEEK_SET);
    char *buffer=(char*)malloc(slen);
    unsigned char *outbuffer=(unsigned char*)malloc(slen);;
    fread(buffer,1,slen,in);
    int dlen=Meow::Core::Peers::Base64::decodeBase64(outbuffer,buffer);
    fwrite(outbuffer,1,dlen,out);
    free(buffer);
    free(outbuffer);
    fclose(in);
    fclose(out);
    chmod(target.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IWOTH);
}
std::string Meow::Core::Storage::File::GetBaseName()
{
	return basename;
}
std::string Meow::Core::Storage::File::GetDirName()
{
	return absolute_path;
}

bool Meow::Core::Storage::File::CopyFile(std::string source, std::string destination)
{
	FILE *fp1, *fp2;
	int key;

	fp1=(FILE*)fopen(source.c_str(), "rb");

	if(fp1==NULL)
	{
		fprintf(stderr, "Can't open source file\n");
	    return false;
	  }

	  fp2=(FILE*)fopen(destination.c_str(), "wb");

	  if(fp2==NULL)
	  {
	    fclose(fp1);
	    fprintf(stderr, "Can't open dest file\n");
	    return false;
	  }

	  while((key=fgetc(fp1))!=EOF)
	  {
	    fputc(key,fp2);
	  }

	  fclose(fp1);
	  fclose(fp2);
	  return true;
}
void * Meow::Core::Storage::File::GetDescriptor( )
{
	return descriptor;
}
