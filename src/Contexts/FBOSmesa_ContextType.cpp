#include "FBOSmesa_ContextType.hpp"
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>


namespace asapgl
{

	FBOSmesa_ContextType::FBOSmesa_ContextType(FBOSmesa_ContextType::Args &a )
	{
		//FBdev
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

        // Figure out the size of the screen in bytes
        screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

        // Map the device to memory
        fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
        if (fbp == (char*)-1) {
            perror("Error: failed to map framebuffer device to memory");
            exit(4);
        }
        printf("The framebuffer device was mapped to memory successfully.\n");



        //OSmesa


   /* Create an RGBA-mode context */
	#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
	   /* specify Z, stencil, accum sizes */
	   ctx = OSMesaCreateContextExt( OSMESA_BGRA, 16, 0, 0, NULL );
	   printf("OSMesaCreateContextExt( GL_RGBA, 24, 0, 0, NULL )!\n");
	#else
	   ctx = OSMesaCreateContext( GL_RGBA, NULL );
	   printf("OSMesaCreateContext( GL_RGBA, NULL )!\n");
	#endif
	   if (!ctx) {
	      printf("OSMesaCreateContext failed!\n");
	      return;
	   }
	 
	   /* Bind the buffer to the context and make it current */
	   if (!OSMesaMakeCurrent( ctx, fbp, GL_UNSIGNED_BYTE, vinfo.xres+10, vinfo.yres-7 )) {
	      printf("OSMesaMakeCurrent failed!\n");
	   }
	}


	FBOSmesa_ContextType::~FBOSmesa_ContextType()
	{
		OSMesaDestroyContext( ctx );
		munmap(fbp, screensize);
        close(fbfd);
	}


    void* FBOSmesa_ContextType::getBufferp()
    {
        return fbp;
    }

    void FBOSmesa_ContextType::DrawBuffer()
    {
    	glFlush();
    }
}