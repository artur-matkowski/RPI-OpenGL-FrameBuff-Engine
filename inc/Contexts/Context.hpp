#ifndef _H_CONTEXT
#define _H_CONTEXT
#include <chrono>
#include "GlutContext.hpp"
#include "FBOSmesa_ContextType.hpp"

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
			}
			else
			{
				Debug::Trace(DebugLevel::WARNING) << "Context already initialized, skipping" << std::endl;
			}
		}



	};

	

}



#endif