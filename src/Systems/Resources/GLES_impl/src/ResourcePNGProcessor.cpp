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

	void LoadPNG(const char* filename)
	{
		png_structp png_ptr;
	    png_infop info_ptr;
	    unsigned int sig_read = 0;
	    int interlace_type;
	    ::FILE *fp;
		TextureData textureData;
	 
	    if ((fp = fopen(filename, "rb")) == NULL)
	    {
	        log::warning << "File \""<< filename <<"\" not founded" << std::endl;
	        return;
	    }
	 
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
	        log::warning << "Failed to create png_read_struct" << std::endl;
	        fclose(fp);
	        return;
	    }
	 
	    /* Allocate/initialize the memory
	     * for image information.  REQUIRED. */
	    info_ptr = png_create_info_struct(png_ptr);
	    if (info_ptr == NULL) {
	        fclose(fp);
	        png_destroy_read_struct(&png_ptr, NULL, NULL);
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
	        fclose(fp);
	        /* If we get here, we had a
	         * problem reading the file */
	        log::warning << "Failed libpng" << std::endl;
	        return;
	    }
	 
	    /* Set up the output control if
	     * you are using standard C streams */
	    png_init_io(png_ptr, fp);
	 
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



	    textureData.width = (uint32_t*) malloc(row_bytes * height + sizeof(uint32_t)*2 + sizeof(uint8_t));
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
	 
	    /* Close the file */
	    fclose(fp);
	    *textureData.width = width;
	    *textureData.height = height;
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

		asapi::FILE::MMAP _out;
		asapi::UniqueID subresourceID;
		std::string binaryResource;

		bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( std::to_string(0), subresourceID );




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
								, std::to_string(0));

		binaryResource = binaryResourceDir + binaryResource;

		//_out.InitForWrite( binaryResource.c_str(), in_resourceFile->Size());
		//memcpy( _out.Data(), (void*)line.c_str(), line.size() );

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