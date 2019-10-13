// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_DIRECTORY_H_
#define _CORE_DIRECTORY_H_
#include <Core/Storage/File.h>
#include <Core/Types/Stack.h>

namespace Meow {
	namespace Core {
		namespace Storage {
			class File;
			class Directory;
			typedef Reference<Directory> Directory_ptr;
			class Directory : public Core::Object
			{
			public:
				Directory();
				Directory(bool doit);
				Directory( std::string abspath );
				virtual ~Directory();
				static Directory_ptr GetDirectoryBase( std::string directory );
				static bool MakeDir( std::string dirname );
				static std::string GetPathSep( );
				bool ExistsChild( string childname );
				bool ExistsFile( string filename );
				Directory_ptr GetChild( string childname, bool createit=false );
				File_ptr GetFile( string filename, bool createit=false );
				void RemoveChild( string childname );
				void RemoveAllChildren();
				Directory_ptr GetParent();
				Core::Types::Stack_ptr GetChildDirectories();
				Core::Types::Stack_ptr GetChildFiles();
				std::string     GetName( );
				static bool     Rename( std::string src, std::string target );
				std::string     GetAbsolutePath();

				static std::string SafeFileName( std::string infilename );

			private:
				friend class File;
				void *          descriptor;
				std::string absolute_path;
				Directory_ptr parent;
			};
		}
	}
}
#endif
