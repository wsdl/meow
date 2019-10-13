#include <Core/Image/Convert.h>
#include <Core/Log.h>
Meow::Core::Image::ImageException::ImageException()
{
	SetMeowClassName("Core/Image/ImageException");
}

Meow::Core::Image::ImageException::~ImageException()
{

}
Meow::Core::Image::Convert::Convert() 
{
	SetMeowClassName("Core/Image/Convert");
	image=NULL;
}
Meow::Core::Image::Convert::~Convert() 
{
	if ( image )
	{
		gdImageDestroy(image);
	}
}
bool Meow::Core::Image::Convert::CanConvert(std::string frommime, std::string tomime )
{
	return ( ( frommime=="image/png" || frommime=="image/gif"  || frommime=="image/vnd.wap.wbmp" || frommime=="image/jpeg" || frommime=="image/jfif" || frommime=="image/jpg" || frommime=="image/webp" || frommime=="image/tiff" || frommime=="image/bmp" ) && ( tomime=="image/png" || tomime=="image/gif"  || tomime=="image/vnd.wap.wbmp" || tomime=="image/jpeg" || tomime=="image/jfif" || tomime=="image/jpg" || tomime=="image/webp" || tomime=="image/tiff" || tomime=="image/bmp" ) );

}
void Meow::Core::Image::Convert::LoadImage( Core::Types::Void_ptr bytes, std::string frommime )
{
	int size=bytes->GetVoidLength();
	void * data=bytes->GetValueAsVoid();
	if ( frommime=="image/png" )
	{
		image=gdImageCreateFromPngPtr( size,data );
	}
	else if ( frommime=="image/gif" )
	{
		image=gdImageCreateFromGifPtr( size,data );
	}
	else if ( frommime=="image/vnd.wap.wbmp" )
	{
		image=gdImageCreateFromWBMPPtr( size,data );
	}
	else if ( frommime=="image/jpeg" || frommime=="image/jfif" || frommime=="image/jpg" )
	{
		image=gdImageCreateFromJpegPtr( size,data );
	}
	else if ( frommime=="image/webp") 
	{
		image=gdImageCreateFromWebpPtr( size,data );
	}
	else if ( frommime=="image/tiff") 
	{
		image=gdImageCreateFromTiffPtr( size,data );			
	}
	else if ( frommime=="image/bmp" )
	{
		image=gdImageCreateFromBmpPtr( size,data );
	}
	if ( !image )
	{
		ImageException e;
		e.SetMessage("Image could not be loaded from data.");
		throw e;
	}
}
void Meow::Core::Image::Convert::LoadImage( Core::Storage::File_ptr infile, std::string frommime)
{
	if ( infile->OpenForReading( ) )
	{	
		Core::Types::Void_ptr bytes=infile->ReadBinaryBytes(infile->Count());
		int size=bytes->GetVoidLength();
		void *  data=bytes->GetValueAsVoid();
		if ( frommime=="image/png" )
		{
			image=gdImageCreateFromPngPtr( size,data );
		}
		else if ( frommime=="image/gif" )
		{
			image=gdImageCreateFromGifPtr( size,data );
		}
		else if ( frommime=="image/vnd.wap.wbmp" )
		{
			image=gdImageCreateFromWBMPPtr( size,data );
		}
		else if ( frommime=="image/jpeg" || frommime=="image/jfif" || frommime=="image/jpg" )
		{
			image=gdImageCreateFromJpegPtr( size,data );
		}
		else if ( frommime=="image/webp") 
		{
			image=gdImageCreateFromWebpPtr( size,data );
		}
		else if ( frommime=="image/tiff") 
		{
			image=gdImageCreateFromTiffPtr( size,data );			
		}
		else if ( frommime=="image/bmp" )
		{
			image=gdImageCreateFromBmpPtr( size,data );
		}
		infile->Close();
	}
	if ( !image )
	{
		ImageException e;
		e.SetMessage("Image could not be loaded from file.");
		throw e;
	}
}
void Meow::Core::Image::Convert::SaveImage( Core::Storage::File_ptr outfile, std::string tomime)
{
	if ( image && outfile->OpenForWriting() )
	{
	//	outfile->Clear();
		FILE * file=(FILE*)outfile->GetDescriptor();
		int outputSize=0;
		void * outputBytes=NULL;
				
		if ( tomime=="image/png" )
		{
			outputBytes=gdImagePngPtr( image,&outputSize);				
		}
		else if ( tomime=="image/gif" )
		{
			outputBytes=gdImageGifPtr(  image,&outputSize);
		}
		else if ( tomime=="image/vnd.wap.wbmp" )
		{
			outputBytes=gdImageWBMPPtr(  image,&outputSize,4);
		}
		else if ( tomime=="image/jpeg" || tomime=="image/jfif" || tomime=="image/jpg" )
		{
			outputBytes=gdImageJpegPtr(  image,&outputSize,4 );
		}
		else if ( tomime=="image/webp")
		{
			outputBytes=gdImageWebpPtr(  image,&outputSize);
		}
		else if ( tomime=="image/tiff") 
		{
			outputBytes=gdImageTiffPtr(  image,&outputSize);			
		}
		else if ( tomime=="image/bmp" )
		{
			outputBytes=gdImageBmpPtr(  image,&outputSize,4);
		}
		if ( !outputBytes )
		{
			ImageException e;
			e.SetMessage("Cannot save as either image is not valid or the file cannot be opened for writing.");
			throw e;
		}
		else
		{
			Core::Types::Void_ptr memoryArea=new Core::Types::Void(outputBytes,outputSize);
			outfile->WriteBinaryBytes(memoryArea);
			outfile->Close();
		}
	}
	else
	{
		ImageException e;
		e.SetMessage("Cannot save as either image is not valid or the file cannot be opened for writing.");
		throw e;
	}

}
void Meow::Core::Image::Convert::Resize( int width, int height )
{
	gdImagePtr targetImage=gdImageCreate(width,height);	
	gdImageCopyResampled(targetImage,image,0,0,0,0,width,height,gdImageSX(image),gdImageSY(image));
	gdImageDestroy(image);
	image=targetImage;	
}
int Meow::Core::Image::Convert::GetWidth( )
{
	int retval=0;
	if ( image )
		retval=gdImageSX(image);
	return retval;
}
int Meow::Core::Image::Convert::GetHeight( )
{
	int retval=0;
	if ( image )
		retval=gdImageSY(image);
	return retval;
}
