#include "ResourcePNGProcessor.hpp"
#include "ResourceSystem.hpp"
#include "bfu.hpp"
#include <png.h>
#include <GLES2/gl2.h>

namespace asapi
{
	struct TextureData
	{
		uint32_t* 	width;
		uint32_t* 	height;
		uint8_t* 	encoding;
		void* 		data;
	};


	void* SendTextureToGPU(TextureData& textureData)
	{
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		GLuint m_textureID;

		glGenTextures(1, &m_textureID);
		
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	    if(*textureData.encoding == 3)
	    {
	    	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, *textureData.width,
		             *textureData.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		             textureData.data);
			log::debug << "founded encodeing is 3 " << std::endl; 
	    }
	    else
	    {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *textureData.width,
		             *textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		             textureData.data);
			log::debug << "founded encodeing is: " << *textureData.encoding << std::endl;
	    }

	    free(textureData.width);

	    return (void*)(size_t)m_textureID;
	}


	struct ReadInfoStruct
	{
		FILE::MMAP png_file;
		void* read_ptr = 0;
	};

	void read_fn(png_structp png_ptr, png_bytep ptr, uint64_t size)
	{
		ReadInfoStruct* p_info = (ReadInfoStruct*)png_get_io_ptr(png_ptr);

		if( ((size_t)p_info->png_file.Data()+(size_t)p_info->png_file.Size()) < ((size_t)p_info->read_ptr + (size_t)size) )
		{
			png_error(png_ptr,"read error");
		}
		else
		{
			memcpy(ptr, p_info->read_ptr, size);
			p_info->read_ptr = (void*) ((size_t)p_info->read_ptr + (size_t)size);
		}
	}

	void ConvertPNG2BinaryResource(FILE::MMAP& png_file, const char* out_path)
	{
		png_structp png_ptr;
	    png_infop info_ptr;
	    unsigned int sig_read = 0;
	    int interlace_type;
	    ReadInfoStruct userReadInfo;
	    FILE::MMAP out_file;

	    userReadInfo.png_file = std::move(png_file);
	    userReadInfo.read_ptr = userReadInfo.png_file.Data();
	 
	 
	    /* Create and initialize the png_struct
	     * with the desired error handler
	     * functions.  If you want to use the
	     * default stderr and longjump method,
	     * you can supply NULL for the last
	     * three parameters.  We also supply the
	     * the compiler header file version, so
	     * that we know if the application
	     * was compiled with a compatible version
	     * of the library.  REQUIRED
	     */
	    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
	                                     NULL, NULL, NULL);
	 
	    if (png_ptr == NULL) 
	    {
	    	png_file = std::move(userReadInfo.png_file);
	        log::warning << "Failed to create png_read_struct" << std::endl;
	        return;
	    }
	 
	    /* Allocate/initialize the memory
	     * for image information.  REQUIRED. */
	    info_ptr = png_create_info_struct(png_ptr);
	    if (info_ptr == NULL) {
	        png_destroy_read_struct(&png_ptr, NULL, NULL);
	        png_file = std::move(userReadInfo.png_file);
	        log::warning << "Failed to create png_info_struct" << std::endl;
	        return;
	    }
	 
	    /* Set error handling if you are
	     * using the setjmp/longjmp method
	     * (this is the normal method of
	     * doing things with libpng).
	     * REQUIRED unless you  set up
	     * your own error handlers in
	     * the png_create_read_struct()
	     * earlier.
	     */
	    if (setjmp(png_jmpbuf(png_ptr))) {
	        /* Free all of the memory associated
	         * with the png_ptr and info_ptr */
	        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	        png_file = std::move(userReadInfo.png_file);
	        /* If we get here, we had a
	         * problem reading the file */
	        log::warning << "Failed libpng" << std::endl;
	        return;
	    }
	 
	    /* Set up the output control if
	     * you are using standard C streams */
	    //png_init_io(png_ptr, fp);

	    png_set_read_fn(png_ptr, (void*)&userReadInfo, read_fn);
	 
	    /* If we have already
	     * read some of the signature */
	    png_set_sig_bytes(png_ptr, sig_read);
	 
	    /*
	     * If you have enough memory to read
	     * in the entire image at once, and
	     * you need to specify only
	     * transforms that can be controlled
	     * with one of the PNG_TRANSFORM_*
	     * bits (this presently excludes
	     * dithering, filling, setting
	     * background, and doing gamma
	     * adjustment), then you can read the
	     * entire image (including pixels)
	     * into the info structure with this
	     * call
	     *
	     * PNG_TRANSFORM_STRIP_16 |
	     * PNG_TRANSFORM_PACKING  forces 8 bit
	     * PNG_TRANSFORM_EXPAND forces to
	     *  expand a palette into RGB
	     */
	    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
	 
	    png_uint_32 width, height;
	    int bit_depth;
	    int color_type;
	    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	                 &interlace_type, NULL, NULL);
	 
	    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);


	    out_file.InitForWrite(out_path, row_bytes * height + sizeof(uint32_t)*2 + sizeof(uint8_t));

	    if( !out_file.IsValid() )
	    {
	    	log::warning << "Can not initialize binary resource file: " << out_path << std::endl;
	    	return;
	    }

		TextureData textureData; 

	    textureData.width = (uint32_t*) out_file.Data();
	    textureData.height = &(textureData.width[1]);
	    textureData.encoding = (uint8_t*) &(textureData.height[1]);
	    textureData.data = (void*) &(textureData.encoding[1]);




	    *textureData.encoding = (uint8_t) png_get_channels(png_ptr, info_ptr);
	 
	    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	 
	    for (int i = 0; i < height; i++) {
	        // note that png is ordered top to
	        // bottom, but OpenGL expect it bottom to top
	        // so the order or swapped
	        memcpy(((GLubyte*)textureData.data)+(row_bytes * (i)), row_pointers[i], row_bytes);
	    }
	 
	    /* Clean up after the read,
	     * and free any memory allocated */
	    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	 
	    *textureData.width = width;
	    *textureData.height = height;

	    png_file = std::move(userReadInfo.png_file);
	}

	
	void* ResourcePNGProcessor::LoadResource(const char* path)
	{
		asapi::FILE::MMAP file;

		file.InitForRead( path );

		if( !file.IsValid() )
		{
			log::error << "can not open resource binary: " << path << std::endl;
			return 0;
		}

		TextureData textureData; 

		
		textureData.width = (uint32_t*) file.Data();
	    textureData.height = &(textureData.width[1]);
	    textureData.encoding = (uint8_t*) &(textureData.height[1]);
	    textureData.data = (void*) &(textureData.encoding[1]);


	    return SendTextureToGPU( textureData );
	}

	void ResourcePNGProcessor::UnloadResource(void* handle)
	{
		GLuint m_textureID = (GLuint) (size_t) handle;

		glDeleteTextures(1, &m_textureID);
	}


	bool ResourcePNGProcessor::ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
											, asapi::FILE::MMAP* in_resourceFile
											, const char* in_projectPath
											, std::vector<asapi::SubResourceData>* out_resourceBinaries)
	{
		std::string binaryResourceDir = in_projectPath;
		binaryResourceDir += RESOURCE_BINARIES_DIR;
		binaryResourceDir += "/";

		out_resourceBinaries->clear();

		//const std::string binaryResource = binaryResourceDir + std::to_string( in_currentResource->GetResourceID() ) + std::string(".txt.bin");


		std::string databuff( (char*)in_resourceFile->Data(), (char*)in_resourceFile->Data()+in_resourceFile->Size() );
		std::istringstream iss(databuff);

		asapi::UniqueID subresourceID;
		std::string binaryResource;
		char filenameBuff[256];


		bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( in_currentResource.GetFilename(), subresourceID );




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//																								//
		//																								//
		//																								//
		//																								//
		//				This if section needs to be reimplemented in custom implementations.			//
		//				Otherwise binary resource name will not be tranfered between updates			//
		//																								//
		//																								//
		//																								//
		//																								//
		//////////////////////////////////////////////////////////////////////////////////////////////////

		binaryResource = std::to_string( (uint64_t)subresourceID ) 
					+ std::string( ResourcePNGProcessor::GetBinaryOutputFileExtension() ) 
					+ std::string( ".bin" );

		asapi::SubResourceData subresource(
								subresourceID
								, in_currentResource.GetFilename());

		binaryResource = binaryResourceDir + binaryResource;


		ConvertPNG2BinaryResource(*in_resourceFile, binaryResource.c_str());

		out_resourceBinaries->push_back( subresource );
		

		return true;
	}


	const char* ResourcePNGProcessor::GetSuportedResourceFileExtension()
	{
		return ".png";
	}

	const char* ResourcePNGProcessor::GetBinaryOutputFileExtension()
	{
		return ".t2d";
	}


	bfu::stream& operator<<(bfu::stream& st, const ResourcePNGProcessor& out )
	{
		st << "not implemented operator <<";
		return st;
	}
}