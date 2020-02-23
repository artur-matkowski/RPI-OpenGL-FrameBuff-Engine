#ifndef _H_FBOSmesa_ContextType
#define _H_FBOSmesa_ContextType
#include "ContextBase.hpp"

namespace asapgl
{

	class FBOSmesa_ContextType: public ContextBase
	{
		int fbfd = 0;
	    //struct fb_var_screeninfo vinfo;
	    //struct fb_fix_screeninfo finfo;
	    long int screensize = 0;
	    char *fbp = 0;
	    int x = 0, y = 0;
	    long int location = 0;


	    OSMesaContext ctx;

	public:
		class Args: public object{
		};

		FBOSmesa_ContextType(FBOSmesa_ContextType::Args &f);
		~FBOSmesa_ContextType();

		void* getBufferp();

		void DrawBuffer();
	};

}

#endif