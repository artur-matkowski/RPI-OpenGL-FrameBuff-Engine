#ifndef _H_GLUTCONTEXT
#define _H_GLUTCONTEXT
#include "ContextBase.hpp"

namespace asapgl
{

	class GLUT_ContextType: public ContextBase
	{
	public:
		class Args: public object{
		public:
			GLuint 			WIDTH;
			GLuint 			HEIGHT;
			int 			ARGC;
			char** 			ARGV;
			std::string 	NAME;
			GLuint 			DISPLAY_MODE;
		};

		GLUT_ContextType(GLUT_ContextType::Args &f );
		~GLUT_ContextType();
		
		void SwapBuffer();
	};
}


#endif