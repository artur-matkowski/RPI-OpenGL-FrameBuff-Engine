#include "Texture.hpp"
#include "Systems.hpp"
#include <png.h>
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	void* Texture::LoadPNG(const char* filename)
	{
		png_structp png_ptr;
	    png_infop info_ptr;
	    unsigned int sig_read = 0;
	    int color_type, interlace_type;
	    FILE *fp;
	 
	    if ((fp = fopen(filename, "rb")) == NULL)
	    {
	        log::warning << "File \""<< filename <<"\" not founded" << std::endl;
	        return 0;
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
	        return 0;
	    }
	 
	    /* Allocate/initialize the memory
	     * for image information.  REQUIRED. */
	    info_ptr = png_create_info_struct(png_ptr);
	    if (info_ptr == NULL) {
	        fclose(fp);
	        png_destroy_read_struct(&png_ptr, NULL, NULL);
	        log::warning << "Failed to create png_info_struct" << std::endl;
	        return 0;
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
	        return 0;
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
	    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
	                 &interlace_type, NULL, NULL);
	 
	    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	    log::debug << "====" << bit_depth << std::endl;
	    GLubyte* textureImage = (unsigned char*) malloc(row_bytes * height);
	 
	    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
	 
	    for (int i = 0; i < height; i++) {
	        // note that png is ordered top to
	        // bottom, but OpenGL expect it bottom to top
	        // so the order or swapped
	        memcpy(textureImage+(row_bytes * (height-1-i)), row_pointers[i], row_bytes);
	    }
	 
	    /* Clean up after the read,
	     * and free any memory allocated */
	    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	 
	    /* Close the file */
	    fclose(fp);
	    m_width = width;
	    m_height = height;
	    //m_sizeOfImage = row_bytes * height;

		return (void*) textureImage;

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

		void *textureImage = 0;

		textureImage = LoadPNG(path);

		SendTextureToGPU( textureImage );
	}

	Texture::~Texture()
	{

	}

	void Texture::SendTextureToGPU(void *textureImage)
	{
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &m_textureID);
		
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width,
	             m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	             textureImage);
	}


	void Texture::Compile(const char* dest, const char* source)
	{
		FILE *src, *dst;
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