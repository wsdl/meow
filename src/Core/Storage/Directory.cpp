/// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Storage/Directory.h>
#include <Core/Types/String.h>
#include <Core/Log.h>
#include <inc/configuration.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32
	#include <windows.h>
#endif
#define DIRECTORY_MASK 0777
#define PATH_MAX 4096
#include <Core/Types/Constraint.h>


Meow::Core::Storage::Directory_ptr Meow::Core::Storage::Directory::GetDirectoryBase( std::string dirtype )
{
	  Configuration_ptr conf=Configuration::GetConfiguration();
    Meow::Core::Storage::Directory_ptr retval;
    if ( conf->IsSet(dirtype) )
    {
        std::string root=conf->Get(dirtype);
        Core::Log::GetLog()->Data("Meow::Core::Storage::Directory::GetDirectoryBase","sss",dirtype.c_str(),"=",root.c_str());
        retval=new Directory(root);
    }
    return retval;


}

bool Meow::Core::Storage::Directory::MakeDir( std::string dirname )
{
#ifdef WIN32
    return CreateDirectory(dirname.c_str(),NULL);
#else
	return mkdir(dirname.c_str(),DIRECTORY_MASK);
#endif
}
std::string Meow::Core::Storage::Directory::GetPathSep( )
{
#ifdef WIN32
    return "\\";
#else
    return "/";
#endif
}

Meow::Core::Storage::Directory::Directory()
{

    SetMeowClassName("Directory");
    descriptor=NULL;
    Configuration_ptr conf=Configuration::GetConfiguration();
    char buf[PATH_MAX];
    getcwd(buf,PATH_MAX);
    string root=buf;
    if ( conf->IsSet("root") )
        root=conf->Get("root");
    absolute_path=root;
    descriptor=(DIR*)opendir(root.c_str());
}
Meow::Core::Storage::Directory::Directory( bool doit )
{

    SetMeowClassName("Directory");
    descriptor=NULL;
    ;
}

Meow::Core::Storage::Directory::Directory( std::string abspath )
{
    descriptor=NULL;
    absolute_path=abspath;
    descriptor=(DIR*)opendir(abspath.c_str());

}
Meow::Core::Storage::Directory::~Directory()
{

    if ( descriptor!=NULL )
        closedir((DIR*)descriptor);
}

bool Meow::Core::Storage::Directory::ExistsChild( string childname )
{
    bool retval=false;

    if ( !childname.compare("..") )
        return false;
    if ( !childname.compare(".") )
        return false;

    if ( descriptor!=NULL )
    {
        struct dirent *ep;
        rewinddir((DIR*)descriptor);
        while ( ep=readdir((DIR*)descriptor) )
        {
            if (!strcmp(ep->d_name,childname.c_str()))
            {
                string fullpath=absolute_path;
#ifndef WIN32
                fullpath.append("/");
#else
                fullpath.append("\\");
#endif

                fullpath.append(childname);

                DIR * subdir=opendir(fullpath.c_str());
                if ( subdir )
                {
                    retval=true;
                    closedir(subdir);
                    break;
                }
            }
        }
        rewinddir((DIR*)descriptor);
    }
    return retval;
}
bool Meow::Core::Storage::Directory::ExistsFile( string filename )
{
    bool retval=false;

    if ( !filename.compare("..") )
        return false;
    if ( !filename.compare(".") )
        return false;

    if ( descriptor!=NULL )
    {
        struct dirent *ep;
        rewinddir((DIR*)descriptor);
        while ( ep=readdir((DIR*)descriptor) )
        {
            if (!strcmp(ep->d_name,filename.c_str()))
            {
                string fullpath=absolute_path;
#ifndef WIN32
                fullpath.append("/");
#else
                fullpath.append("\\");
#endif
                fullpath.append(filename);

                DIR * subdir=opendir(fullpath.c_str());
                if ( !subdir )
                {
                    retval=true;
                    break;
                }
                else
                    closedir(subdir);
            }
        }
        rewinddir((DIR*)descriptor);
    }
    return retval;
}



Meow::Core::Storage::Directory_ptr Meow::Core::Storage::Directory::GetChild( string childname, bool createit )
{
	Directory_ptr dir=new Directory(false);
    if ( !childname.compare("..") )
        return dir;
    if ( !childname.compare(".") )
        return dir;

    if ( descriptor!=NULL )
    {
       	rewinddir((DIR*)descriptor);

        struct dirent *ep;
        bool found=false;
        while ( ep=readdir((DIR*)descriptor) )
        {
			Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ss","Checking:",ep->d_name);	
            if (!strcmp(ep->d_name,childname.c_str()))
            {
                string fullpath=absolute_path;
#ifndef WIN32
                fullpath.append("/");
#else
                fullpath.append("\\\\");
#endif

                fullpath.append(childname);
                DIR * subdir=opendir(fullpath.c_str());
                if ( subdir )
                {
                    dir->descriptor=(void*)opendir(fullpath.c_str());
                    if ( dir->descriptor )
                    {
						Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ss","Found",fullpath.c_str());
                        dir->absolute_path=fullpath;
                        found=true;
                    }
                    closedir(subdir);

                }
                else
				{
                    Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ss","CouldNotOpen",fullpath.c_str());
					errno=0;	
					int fd=open(absolute_path.c_str(),O_RDWR | O_DIRECTORY);
					mkdirat(fd,childname.c_str(),DIRECTORY_MASK);	
					close(fd);
					Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ssss","Make Directory ",fullpath.c_str()," errno: ",strerror(errno));
					dir->descriptor=opendir(fullpath.c_str());
					if ( dir->descriptor )
                    {
						Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ss","Made",fullpath.c_str());
                        dir->absolute_path=fullpath;
                        found=true;
                    }
					else
						
                    closedir(subdir);
				}
            }
        }
		
        rewinddir((DIR*)descriptor);
        // descriptor=NULL;
        if ( !found )
        	Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ss","CouldNotFind",childname.c_str());
    }
	else
	{
		Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","s","Descriptor NULL");	
	}
    return dir;
}

void Meow::Core::Storage::Directory::RemoveChild( std::string filename )
{
    // Support unlink when you have a .mta file
    string fullpath=absolute_path;
#ifndef WIN32
    fullpath.append("/");
#else
    fullpath.append("\\");
#endif

    fullpath.append(filename);

    if ( ExistsChild(filename) )
    {
        if ( rmdir(fullpath.c_str())!= 0 )
        {
        	Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ssss","Could not remove ",fullpath.c_str()," because ",strerror(errno));
        }

    }
    else if ( ExistsFile(filename) )
    {
        if ( unlink(fullpath.c_str()) )
        	Core::Log::GetLog()->Error("Meow::Core::Storage::Directory::GetChild","ssss","Could not remove ",fullpath.c_str()," because ",strerror(errno));

    }
    else
    {
    	Core::Log::GetLog()->Warning("Meow::Core::Storage::Directory::GetChild","ss","Could not find to remove ",filename.c_str());
    }
}

void Meow::Core::Storage::Directory::RemoveAllChildren()
{
    if ( descriptor!=NULL )
    {
        struct dirent *ep;
        while ( ep=readdir((DIR*)descriptor) )
        {
            string fullpath=absolute_path;
#ifndef WIN32
            fullpath.append("/");
#else
            fullpath.append("\\");
#endif

            fullpath.append(ep->d_name);

            DIR * subdir=opendir(fullpath.c_str());
            if ( subdir )
            {
                Directory_ptr subdirectory=new Directory();
                subdirectory->descriptor=subdir;
                subdirectory->absolute_path=fullpath;
                subdirectory->parent=this;
                subdirectory->RemoveAllChildren();
                closedir(subdir);
                subdirectory->descriptor=NULL;
            }
            else
                unlink(fullpath.c_str());
        }
        rewinddir((DIR*)descriptor);
    }
}

Meow::Core::Storage::File_ptr Meow::Core::Storage::Directory::GetFile( string filename, bool createit )
{
	File_ptr retval=new File();
    retval->iDirectory=this;
    retval->basename=filename;
    if ( !filename.compare("..") )
        return retval;
    if ( !filename.compare(".") )
        return retval;
    if ( descriptor!=NULL )
    {
        struct dirent *ep;
        bool found=false;
        while ( ep=readdir((DIR*)descriptor) )
        {
            if (!strcmp(ep->d_name,filename.c_str()))
            {
                string fullpath=absolute_path;
#ifndef WIN32
                fullpath.append("/");
#else
                fullpath.append("\\");
#endif

                fullpath.append(filename);

                DIR * subdir=opendir(fullpath.c_str());
                if ( !subdir )
                {
                    retval->absolute_path=fullpath;
                    found=true;
                }
                else
                {
                    closedir(subdir);
                }
            }

        }
        if ( !found && createit )
        {
            string fullpath=absolute_path;
            fullpath.append("/");
            fullpath.append(filename);
            retval->absolute_path=fullpath;
        }
        rewinddir((DIR*)descriptor);
    }
    return retval;
}
Meow::Core::Types::Stack_ptr Meow::Core::Storage::Directory::GetChildDirectories()
{
    Meow::Core::Types::Stack_ptr retval=new Meow::Core::Types::Stack();
    if ( descriptor!=NULL )
    {
        struct dirent *ep;
        while ( ep=readdir((DIR*)descriptor))
        {
            string fullpath=absolute_path;
#ifndef WIN32
            fullpath.append("/");
#else
            fullpath.append("\\\\");
#endif
            fullpath.append(ep->d_name);

            DIR * subdir=opendir(fullpath.c_str());
            if ( subdir )
            {
            	Meow::Core::Types::String_ptr child=new Meow::Core::Types::String(ep->d_name);
                retval->Push((ptr)child);
                closedir(subdir);
            }
        }
        rewinddir((DIR*)descriptor);
    }
    return retval;
}
Meow::Core::Types::Stack_ptr Meow::Core::Storage::Directory::GetChildFiles()
{
    Meow::Core::Types::Stack_ptr retval=new Meow::Core::Types::Stack();
    if ( descriptor!=NULL )
    {
        struct dirent *ep;

        while ( ep=readdir((DIR*)descriptor))
        {
            string fullpath=absolute_path;
#ifndef WIN32
            fullpath.append("/");
#else
            fullpath.append("\\");
#endif

            fullpath.append(ep->d_name);

            DIR * subdir=opendir(fullpath.c_str());
            if ( !subdir )
            {
                Meow::Core::Types::String_ptr child=new Meow::Core::Types::String(ep->d_name);
                retval->Push((ptr)child);
            }
            else
                closedir(subdir);
        }
        rewinddir((DIR*)descriptor);
    }
    return retval;
}

std::string Meow::Core::Storage::Directory::GetName( )
{
    return absolute_path;
}

Meow::Core::Storage::Directory_ptr Meow::Core::Storage::Directory::GetParent()
{
    return parent;
}


bool Meow::Core::Storage::Directory::Rename( std::string src, std::string target )
{
#ifdef WIN32
    return MoveFile(src.c_str(),target.c_str());
#else
    // TODO implement in Unix
    return false;
#endif
}

std::string Meow::Core::Storage::Directory::GetAbsolutePath()
{
	return absolute_path;
}

std::string Meow::Core::Storage::Directory::SafeFileName( std::string infilename )
{
	// Core::Types::Constraint_ptr constraint=new Core::Types::Constraint();
	// constraint->SetExpression(CONSTRAINT_FILENAME);
	// bool meetsPattern=constraint->Test(new Core::Types::String(infilename));
	// if ( meetsPattern )
    // return infilename;
	// So we need to translate all the bad characters.
	char * fileNameData=strdup(infilename.c_str());
	for ( int i=0; i<infilename.length(); i++ )
	{
		if ( ! ( ( fileNameData[i]>= 48 && fileNameData[i]<=57 ) ||
			( fileNameData[i]>= 65 && fileNameData[i] <= 90 ) ||
			   ( fileNameData[i]>= 97 && fileNameData[i] <= 122 ) ||
				fileNameData[i]=='.' ||
				fileNameData[i]=='-' ||
				fileNameData[i]=='_' ) )
			fileNameData[i]='_';
	}
	std::string retval=fileNameData;
	
	free(fileNameData);		
	return retval;
}
