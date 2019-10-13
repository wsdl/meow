// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_FILE_H_
#define _CORE_FILE_H_
#include <Core/Object.h>
#include <Core/Types/String.h>
#include <Core/Types/Array.h>
#include <Core/Types/Void.h>

namespace Meow {
	namespace Core {
		namespace Storage {
			class Directory;
			class File;
			typedef Reference<Directory> Directory_ptr;
			typedef Reference<File> File_ptr;
			class File : public Core::Object
			{
			public:
				File();
				virtual ~File();

				bool OpenForWriting();
				bool OpenForReading();
				bool Clear();
				bool RewindToTop();

				// Read operations
				bool EoF();
				Meow::Core::Types::String_ptr ReadBytes( unsigned int bytelength );
				Meow::Core::Types::Void_ptr ReadBinaryBytes( unsigned int bytelength );

				// Write operations
				void WriteBytes( Meow::Core::Types::String_ptr data );
				void WriteValueType( Meow::Core::Types::ValueType_ptr );
				void WriteBinaryBytes( Meow::Core::Types::Void_ptr data );
				void Close();
				Meow::Core::Types::String_ptr GetContents();
				long Count();
				void WriteLine( Meow::Core::Types::String_ptr line );
				Meow::Core::Types::String_ptr ReadLine( );
				Meow::Core::Types::String_ptr ReadLineEx( );


				static void Base64DecodeFile( std::string source, std::string target );
				static void Base64EncodeFile( std::string source, std::string target );


				std::string GetBaseName();
				std::string GetDirName();
				bool CopyFile(std::string source, std::string destination);
				void * GetDescriptor();
			private:
				friend class Directory;
				std::string absolute_path;
				std::string basename;
				void * descriptor;
				int status;
				Directory_ptr iDirectory;
				bool auto_archive;
			};
		}
	}
}
#endif
