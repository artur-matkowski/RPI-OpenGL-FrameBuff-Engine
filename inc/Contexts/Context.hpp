#ifndef _H_CONTEXT
#define _H_CONTEXT
#include <chrono>
#include "DRM_GBM_EGL_ContextType.hpp"
#ifndef _TARGET
#include "Xlib_EGL_ContextType.hpp"
#endif

namespace asapgl
{


	class Context: public object
	{
		ContextBase* 		context;
		std::chrono::duration<double> 
							m_frameDelay;
		bool				m_isRunning;

		int 				m_width;
		int 				m_height;


	public:
		Context();
		~Context();


		void SetFPS(float FPS);
		void CleanUp();

		void MainLoop();


		#ifndef _TARGET
		void initXlib(const int argc, const char** argv);
		#endif
		
		void initDRM(const int argc, const char** argv);
	};


	
	

}



#endif