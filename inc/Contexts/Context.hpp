#ifndef _H_CONTEXT
#define _H_CONTEXT
#include <chrono>
#include "GlutContext.hpp"
#include "DRM_GBM_EGL_ContextType.hpp"

namespace asapgl
{


	class Context: public object
	{
		ContextBase* 		context;
		std::chrono::duration<double> 
							m_frameDelay;
		GLboolean			m_isRunning;

	public:
		Context();
		~Context();


		void SetFPS(GLfloat FPS);
		void CleanUp();

		void MainLoop();


		template<class T, typename Lambda>
		void init( Lambda&&  f )
		{
			typename T::Args a;


			if(context==0)
			{
				f(a);
				context = new T(a);
				//glewInit();

				//log::info << "GL initialized with version: " << glGetString(GL_VERSION) << std::endl;
				//log::info << "GL vendor: " << glGetString(GL_VENDOR) << std::endl;
				//log::info << "GL renderer: " << glGetString(GL_RENDERER) << std::endl;
				//log::info << "GL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
			}
			else
			{
				//log::warning << "Context already initialized, skipping" << std::endl;
			}
		}



	};

	

}



#endif