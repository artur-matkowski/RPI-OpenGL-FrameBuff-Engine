#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png.h>

#include "RendererSystem.hpp"
#include "ContextBase.hpp"
#include "Systems.hpp"



namespace asapgl{

	void RendererSystem::SendTextureToGPU(void *textureImage)
	{

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &m_textureID);
		
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mi_width,
	             mi_height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
	             textureImage);
	}

	void RendererSystem::BindTexture()
	{
		//TODO texture binding
		//LOG_DEBUG("prev tex ID:%d, current tex ID:%d", m_previouslyBindedTextureID, m_textureID);
		//if(m_previouslyBindedTextureID != m_textureID)
		{
			glBindTexture(GL_TEXTURE_2D, m_textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}

	void RendererSystem::Init()
	{

		png_structp png_ptr;
	    png_infop info_ptr;
	    unsigned int sig_read = 0;
	    int color_type, interlace_type;
	    FILE *fp;
	 
	    if ((fp = fopen("debug.png", "rb")) == NULL)
	    {
	        log::warning << "File \""<< "debug.png" <<"\" not founded" << std::endl;
	        return ;
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
	        return ;
	    }
	 
	    /* Allocate/initialize the memory
	     * for image information.  REQUIRED. */
	    info_ptr = png_create_info_struct(png_ptr);
	    if (info_ptr == NULL) {
	        fclose(fp);
	        png_destroy_read_struct(&png_ptr, NULL, NULL);
	        log::warning << "Failed to create png_info_struct" << std::endl;
	        return ;
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
	        return ;
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

	    /* That's it */
	    mi_width = width;
	    mi_height = height;
	    m_textureImage = textureImage;
		m_sizeOfImage = row_bytes * height;

		 SendTextureToGPU( m_textureImage );
	}

	RendererSystem::RendererSystem()
	{

	}
	RendererSystem::~RendererSystem()
	{

	}

	void RendererSystem::SetupEvents()
	{
    	bfu::CallbackId id;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

		events.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
	    {
		    ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
	    	m_width = args->m_width; 
	    	m_height = args->m_height; 
			log::debug << "resolution update invoked on RendererSystem: " << m_width << "x" << m_height  << std::endl;
	    });
	}


	void RendererSystem::Render()
	{
		static const char *vertex_source =
			"#version 100\n"
			"attribute vec4 position;\n"
			"attribute vec4 color;\n"
			"attribute vec2 texCoord;\n"
			"\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"\n"
			"void main()\n"
			"{\n"
			"  gl_Position = position;\n"
			"  vcolor = color;\n"
			"  UV = texCoord;\n"
			"}\n";

		static const char *fragment_source =
			"#version 100\n"
			"precision mediump float;\n"
			"varying vec4 vcolor;\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texUnit;\n"
			"uniform float blend;\n"
			"\n"
			"void main()\n"
			"{\n"
			//"  gl_FragColor = texture2D(texUnit, UV) * vcolor;\n"
			"  gl_FragColor = texture2D(texUnit, UV) * vcolor * blend;\n"
			//"  gl_FragColor = UV.xyyy * blend;\n"
			"}\n";

		static GLfloat vertices[] = {
			 0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
		};

		static GLfloat texCoords[] = {
			 0.0f,  0.0f,
			 0.0f,  1.0f,
			 1.0f,  1.0f, 
		};

		static GLfloat colors[] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
		};

		GLuint vertex, fragment, program;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_source, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_source, NULL);
		glCompileShader(fragment);

		GLint isCompiled = 0;
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragment, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(fragment); // Don't leak the shader.
		}



		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "color");
		glBindAttribLocation(program, 2, "texCoord");

		//BindTexture();


		glLinkProgram(program);
		glFlush();

		glUseProgram(program);
		glFlush();

		GLint texUnitLoc = glGetUniformLocation(program, "texUnit");
		GLint blendLoc = glGetUniformLocation(program, "blend");

		log::debug << "texUnitLoc: " << texUnitLoc << std::endl;
		log::debug << "blendLoc: " << blendLoc << std::endl;
		log::debug << "m_textureID: " << m_textureID << std::endl;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glUniform1i(texUnitLoc , 0);
		glUniform1f(blendLoc , 1.0);

		glViewport(0, 0, m_width, m_height);
		glFlush();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glFlush();

		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		glFlush();

		glEnableVertexAttribArray(0);
		glFlush();

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, colors);
		glFlush();

		glEnableVertexAttribArray(1);
		glFlush();

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
		glFlush();

		glEnableVertexAttribArray(2);
		glFlush();

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glFlush();

	}
			
	

}