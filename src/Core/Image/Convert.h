// Copyright (c) 2017 David Spencer.
// All rights reserved.
// Version: 1.0
// Change History:
//	9/08/2017 @DavidSpencer
//		Add in the image conversion routines.
//		Expose the fact for now our built in is libgd.
#ifndef _CORE_IMAGE_CONVERSION_H_
#define _CORE_IMAGE_CONVERSION_H_
#include <Core/Object.h>
#include <Exception.h>
#include <Core/Storage/File.h>
#include <Core/Types/Void.h>
#include <gd.h>

namespace Meow {
	namespace Core {
		namespace Image {

			class ImageException : public Meow::Exception
			{
			public:
				ImageException();
				~ImageException();
			};
			
			class Convert;
			typedef Reference<Convert> Convert_ptr;
		
			class Convert : public Object
			{
			public:
				Convert();
				virtual ~Convert();
				bool CanConvert(std::string frommime, std::string tomime );				
				void LoadImage( Core::Types::Void_ptr infile, std::string frommime);
				void LoadImage( Core::Storage::File_ptr infile, std::string frommime);
				void SaveImage( Core::Storage::File_ptr outfile, std::string tomime);
				void Resize( int width, int height );
				int GetWidth();
				int GetHeight();
				
			private:
				gdImagePtr image;
						
			};
		}
	}
}
#endif
