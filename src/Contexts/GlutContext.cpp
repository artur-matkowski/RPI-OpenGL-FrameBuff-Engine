#include "GlutContext.hpp"
#include <GL/glut.h>


namespace asapgl
{

	GLUT_ContextType::GLUT_ContextType(GLUT_ContextType::Args &a )
	{
/*
		glutInit(&a.ARGC, a.ARGV);
		glutInitDisplayMode(a.DISPLAY_MODE);
		glutInitWindowSize(a.WIDTH, a.HEIGHT);
		glutCreateWindow(a.NAME.c_str());
		//glutIdleFunc(render);
		*/
	}


	GLUT_ContextType::~GLUT_ContextType()
	{

	}

    void GLUT_ContextType::SwapBuffer()
    {
    	//glutSwapBuffers();
    }
}