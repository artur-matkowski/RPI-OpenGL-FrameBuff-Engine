#ifndef _H_FBOSmesa_ContextType
#define _H_FBOSmesa_ContextType
#include "ContextBase.hpp"

namespace asapgl
{

	class FBOSmesa_ContextType: public ContextBase
	{
		//fbdev
		int 		fbfd = 0;
	    long int 	screensize = 0;
	    void 		*fbp = 0;
	    int 		width = 0;
	    int 		height = 0;

	    void BindTo_DEV_FB0();


	    //osmesa
	    OSMesaContext ctx;


	    //render target
	    GLuint FramebufferName = 0;
	    GLuint renderedTexture;
	    GLuint depthrenderbuffer;

	    void InitRT();
	public:
		class Args: public object{
		public:
			int 			ARGC;
			char** 			ARGV;
		};

		FBOSmesa_ContextType(FBOSmesa_ContextType::Args &f);
		~FBOSmesa_ContextType();

		void SwapBuffer();
	};

}

#endif