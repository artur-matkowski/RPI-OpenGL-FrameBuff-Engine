#include "Texture.hpp"
#include "Systems.hpp"
#include <png.h>
#include "bfu.hpp"

namespace asapi
{
	void Texture::LoadPNG(const char* filename, TextureData& out_textureData)
	{
		png_structp png_ptr;
	    png_infop info_ptr;
	    unsigned int sig_read = 0;
	    int interlace_type;
	    ::FILE *fp;
	 
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



	    out_textureData.width = (uint32_t*) malloc(row_bytes * height + sizeof(uint32_t)*2 + sizeof(uint8_t));
	    out_textureData.height = &(out_textureData.width[1]);
	    out_textureData.encoding = (uint8_t*) &(out_textureData.height[1]);
	    out_textureData.data = (void*) &(out_textureData.encoding[1]);


	    *out_textureData.encoding = (uint8_t) png_get_channels(png_ptr, info_ptr);
	 
	    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	 
	    for (int i = 0; i < height; i++) {
	        // note that png is ordered top to
	        // bottom, but OpenGL expect it bottom to top
	        // so the order or swapped
	        memcpy(((GLubyte*)out_textureData.data)+(row_bytes * (i)), row_pointers[i], row_bytes);
	    }
	 
	    /* Clean up after the read,
	     * and free any memory allocated */
	    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	 
	    /* Close the file */
	    fclose(fp);
	    *out_textureData.width = width;
	    *out_textureData.height = height;
	}


	Texture::Texture(const char* path)
	{
		#ifdef IS_EDITOR
		int size = strlen(path);
		const char* tmp = &path[size-1];
		for(; *tmp!='/'; tmp-=1); // < find where file name is starting
		strncpy(name, tmp+1, 255);
		name[255] = '\0';
		#endif


		TextureData textureData;

		LoadPNG(path, textureData);

		SendTextureToGPU(textureData);
	}

	Texture::~Texture()
	{

	}

	void Texture::SendTextureToGPU(TextureData& textureData)
	{
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &m_textureID);
		
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	}


	void Texture::Compile(const char* dest, const char* source)
	{
		::FILE *src, *dst;
		long int srcSize, dstSize;

		src = fopen (source,"rb");
		dst = fopen (dest,"wb");
		fseek(src, 0L, SEEK_END); 
		srcSize = ftell(src); 
		fseek(src, 0L, SEEK_SET);

		char* buff = new char[srcSize];

		fread(buff, sizeof(char), srcSize, src);
		fwrite(buff, 1, srcSize, dst);

		delete buff;

		fclose(src); 
		fclose(dst); 
	}
}