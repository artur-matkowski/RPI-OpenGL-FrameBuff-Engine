#include "FBOSmesa_ContextType.hpp"
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>


namespace asapgl
{

	void FBOSmesa_ContextType::BindTo_DEV_FB0()
	{
		struct fb_var_screeninfo vinfo;
	    struct fb_fix_screeninfo finfo;

		Debug::Trace << "fb os" << std::endl;
		// Open the file for reading and writing
        //fbfd = open("/dev/fb0", O_RDWR);
        fbfd = open("/dev/fb0", O_RDWR);
        if (fbfd == -1) {
            perror("Error: cannot open framebuffer device");
            exit(1);
        }
        printf("The framebuffer device was opened successfully.\n");

        // Get fixed screen information
        if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
            perror("Error reading fixed information");
            exit(2);
        }

        // Get variable screen information
        if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
            perror("Error reading variable information");
            exit(3);
        }

        printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
        width = vinfo.xres;
	    height = vinfo.yres;

        // Figure out the size of the screen in bytes
        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

        // Map the device to memory
        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if (fbp == (char*)-1) {
            perror("Error: failed to map framebuffer device to memory");
            exit(4);
        }
        printf("The framebuffer device was mapped to memory successfully.\n");
	}

	void FBOSmesa_ContextType::InitRT()
	{
		/*
		glGenFramebuffers(1, &FramebufferName);
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);



		glGenTextures(1, &renderedTexture);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, renderedTexture);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 768, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Poor filtering. Needed !
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
*/
	}

	FBOSmesa_ContextType::FBOSmesa_ContextType(FBOSmesa_ContextType::Args &a )
	{
		//FBdev
		BindTo_DEV_FB0();
		//glutInit(&a.ARGC, a.ARGV);


        //OSmesa


   // Create an RGBA-mode context 
	#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
	   // specify Z, stencil, accum sizes 
	   ctx = OSMesaCreateContextExt( OSMESA_BGRA, 24, 0, 0, NULL );
	   printf("OSMesaCreateContextExt( GL_RGBA, 24, 0, 0, NULL )!\n");
	#else
	   ctx = OSMesaCreateContext( GL_RGBA, NULL );
	   printf("OSMesaCreateContext( GL_RGBA, NULL )!\n");
	#endif
	   if (!ctx) {
	      printf("OSMesaCreateContext failed!\n");
	      return;
	   }
	 
	   // Bind the buffer to the context and make it current 
	   if (!OSMesaMakeCurrent( ctx, fbp, GL_UNSIGNED_BYTE, width+10, height-7 )) {
	      printf("OSMesaMakeCurrent failed!\n");
	   }
	   
	}


	FBOSmesa_ContextType::~FBOSmesa_ContextType()
	{
		OSMesaDestroyContext( ctx );
		munmap(fbp, screensize);
        close(fbfd);
	}



    void FBOSmesa_ContextType::SwapBuffer()
    {

    	glFlush();

    	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0,1024,768);



		glClearColor(1.0,0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();

		glBegin(GL_QUADS);
			glTexCoord2f (1.0f,1.0f);
			glVertex3f(0.5, 0.5, 0.0);

			glTexCoord2f (1.0f,0.0f);
			glVertex3f(0.5, 0.0, 0.0);

			glTexCoord2f (0.0f,0.0f);
			glVertex3f(0.0, 0.0, 0.0);

			glTexCoord2f (0.0f,1.0f);
			glVertex3f(0.0, 0.5, 0.0);
		glEnd();
				


    	// Render to our framebuffer
		//glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    }
}