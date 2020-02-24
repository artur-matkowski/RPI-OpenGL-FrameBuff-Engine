#ifndef _H_OPENGLWRAPPER
#define _H_OPENGLWRAPPER

#define GLEW_OSMESA

//#include <GL/glu_mangle.h>
//#include <GL/glx_mangle.h>

#include <GL/osmesa.h>

//#include <GL/gl.h>
//#include <GL/glu.h>


#include <GL/freeglut.h>











/*
class GLWrapper
	{
	public:
		static void init()
		{
			_mglCreateShader = (PFNGLCREATESHADERPROC)OSMesaGetProcAddress("glCreateShader");
			_mglShaderSource = (PFNGLSHADERSOURCEPROC)OSMesaGetProcAddress("glShaderSource");
			_mglCompileShader = (PFNGLCOMPILESHADERPROC)OSMesaGetProcAddress("glCompileShader");
			_mglCreateProgram = (PFNGLCREATEPROGRAMPROC)OSMesaGetProcAddress("glCreateProgram");
			_mglAttachShader = (PFNGLATTACHSHADERPROC)OSMesaGetProcAddress("glAttachShader");
			_mglLinkProgram = (PFNGLLINKPROGRAMPROC)OSMesaGetProcAddress("glLinkProgram");
			_mglUseProgram = (PFNGLUSEPROGRAMPROC)OSMesaGetProcAddress("glUseProgram");

			_mglUniform1f = (PFNGLUNIFORM1FPROC) OSMesaGetProcAddress("glUniform1f");
			_mglUniform2f = (PFNGLUNIFORM2FPROC) OSMesaGetProcAddress("glUniform2f");
			_mglUniform3f = (PFNGLUNIFORM3FPROC) OSMesaGetProcAddress("glUniform3f");
			_mglUniform4f = (PFNGLUNIFORM4FPROC) OSMesaGetProcAddress("glUniform4f");
			_mglUniform1i = (PFNGLUNIFORM1IPROC) OSMesaGetProcAddress("glUniform1i");
			_mglUniform2i = (PFNGLUNIFORM2IPROC) OSMesaGetProcAddress("glUniform2i");
			_mglUniform3i = (PFNGLUNIFORM3IPROC) OSMesaGetProcAddress("glUniform3i");
			_mglUniform4i = (PFNGLUNIFORM4IPROC) OSMesaGetProcAddress("glUniform4i");
			_mglUniform1fv = (PFNGLUNIFORM1FVPROC) OSMesaGetProcAddress("glUniform1fv");
			_mglUniform2fv = (PFNGLUNIFORM2FVPROC) OSMesaGetProcAddress("glUniform2fv");
			_mglUniform3fv = (PFNGLUNIFORM3FVPROC) OSMesaGetProcAddress("glUniform3fv");
			_mglUniform4fv = (PFNGLUNIFORM4FVPROC) OSMesaGetProcAddress("glUniform4fv");
			_mglUniform1iv = (PFNGLUNIFORM1IVPROC) OSMesaGetProcAddress("glUniform1iv");
			_mglUniform2iv = (PFNGLUNIFORM2IVPROC) OSMesaGetProcAddress("glUniform2iv");
			_mglUniform3iv = (PFNGLUNIFORM3IVPROC) OSMesaGetProcAddress("glUniform3iv");
			_mglUniform4iv = (PFNGLUNIFORM4IVPROC) OSMesaGetProcAddress("glUniform4iv");
			_mglUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) OSMesaGetProcAddress("glUniformMatrix2fv");
			_mglUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) OSMesaGetProcAddress("glUniformMatrix3fv");
			_mglUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) OSMesaGetProcAddress("glUniformMatrix4fv");

			_mglBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) OSMesaGetProcAddress("glBindVertexArray");
			_mglDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) OSMesaGetProcAddress("glDeleteVertexArrays");
			_mglGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) OSMesaGetProcAddress("glGenVertexArrays");
			_mglIsVertexArray = (PFNGLISVERTEXARRAYPROC) OSMesaGetProcAddress("glIsVertexArray");

			_mglGenBuffers = (PFNGLGENBUFFERSPROC) OSMesaGetProcAddress("glGenBuffers");
			_mglBindBuffer = (PFNGLBINDBUFFERPROC) OSMesaGetProcAddress("glBindBuffer");
			_mglBufferData = (PFNGLBUFFERDATAPROC) OSMesaGetProcAddress("glBufferData");
			_mglVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) OSMesaGetProcAddress("glVertexAttribPointer");
			_mglEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) OSMesaGetProcAddress("glEnableVertexAttribArray");

			_mglGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) OSMesaGetProcAddress("glGetAttachedShaders");
			_mglGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) OSMesaGetProcAddress("glGetAttribLocation");
			_mglGetProgramiv = (PFNGLGETPROGRAMIVPROC) OSMesaGetProcAddress("glGetProgramiv");
			_mglGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) OSMesaGetProcAddress("glGetProgramInfoLog");
			_mglGetShaderiv = (PFNGLGETSHADERIVPROC) OSMesaGetProcAddress("glGetShaderiv");
			_mglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) OSMesaGetProcAddress("glGetShaderInfoLog");
			_mglGetShaderSource = (PFNGLGETSHADERSOURCEPROC) OSMesaGetProcAddress("glGetShaderSource");
			_mglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) OSMesaGetProcAddress("glGetUniformLocation");
			_mglGetUniformfv = (PFNGLGETUNIFORMFVPROC) OSMesaGetProcAddress("glGetUniformfv");
			_mglGetUniformiv = (PFNGLGETUNIFORMIVPROC) OSMesaGetProcAddress("glGetUniformiv");
			_mglGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) OSMesaGetProcAddress("glGetVertexAttribdv");
			_mglGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) OSMesaGetProcAddress("glGetVertexAttribfv");
			_mglGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) OSMesaGetProcAddress("glGetVertexAttribiv");
			_mglGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) OSMesaGetProcAddress("glGetVertexAttribPointerv");

			_mglDeleteShader = (PFNGLDELETESHADERPROC) OSMesaGetProcAddress("glDeleteShader");
			_mglDetachShader = (PFNGLDETACHSHADERPROC) OSMesaGetProcAddress("glDetachShader");

			_mglBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) OSMesaGetProcAddress("glBindAttribLocation");
		};

		static PFNGLCREATESHADERPROC _mglCreateShader;
		static PFNGLSHADERSOURCEPROC _mglShaderSource;
		static PFNGLCOMPILESHADERPROC _mglCompileShader;
		static PFNGLCREATEPROGRAMPROC _mglCreateProgram;
		static PFNGLATTACHSHADERPROC _mglAttachShader;
		static PFNGLLINKPROGRAMPROC _mglLinkProgram ;
		static PFNGLUSEPROGRAMPROC _mglUseProgram;

		static PFNGLUNIFORM1FPROC _mglUniform1f;
		static PFNGLUNIFORM2FPROC _mglUniform2f;
		static PFNGLUNIFORM3FPROC _mglUniform3f;
		static PFNGLUNIFORM4FPROC _mglUniform4f;
		static PFNGLUNIFORM1IPROC _mglUniform1i;
		static PFNGLUNIFORM2IPROC _mglUniform2i;
		static PFNGLUNIFORM3IPROC _mglUniform3i;
		static PFNGLUNIFORM4IPROC _mglUniform4i;
		static PFNGLUNIFORM1FVPROC _mglUniform1fv;
		static PFNGLUNIFORM2FVPROC _mglUniform2fv;
		static PFNGLUNIFORM3FVPROC _mglUniform3fv;
		static PFNGLUNIFORM4FVPROC _mglUniform4fv;
		static PFNGLUNIFORM1IVPROC _mglUniform1iv;
		static PFNGLUNIFORM2IVPROC _mglUniform2iv;
		static PFNGLUNIFORM3IVPROC _mglUniform3iv;
		static PFNGLUNIFORM4IVPROC _mglUniform4iv;
		static PFNGLUNIFORMMATRIX2FVPROC _mglUniformMatrix2fv;
		static PFNGLUNIFORMMATRIX3FVPROC _mglUniformMatrix3fv;
		static PFNGLUNIFORMMATRIX4FVPROC _mglUniformMatrix4fv;


		static PFNGLBINDVERTEXARRAYPROC _mglBindVertexArray;
		static PFNGLDELETEVERTEXARRAYSPROC _mglDeleteVertexArrays;
		static PFNGLGENVERTEXARRAYSPROC _mglGenVertexArrays;
		static PFNGLISVERTEXARRAYPROC _mglIsVertexArray;

		static PFNGLGENBUFFERSPROC _mglGenBuffers;
		static PFNGLBINDBUFFERPROC _mglBindBuffer;
		static PFNGLBUFFERDATAPROC _mglBufferData;
		static PFNGLVERTEXATTRIBPOINTERPROC _mglVertexAttribPointer;
		static PFNGLENABLEVERTEXATTRIBARRAYPROC _mglEnableVertexAttribArray;
		static PFNGLGETATTRIBLOCATIONPROC _mglGetAttribLocation;


		static PFNGLGETATTACHEDSHADERSPROC _mglGetAttachedShaders;
		static PFNGLGETPROGRAMIVPROC _mglGetProgramiv;
		static PFNGLGETPROGRAMINFOLOGPROC _mglGetProgramInfoLog;
		static PFNGLGETSHADERIVPROC _mglGetShaderiv;
		static PFNGLGETSHADERINFOLOGPROC _mglGetShaderInfoLog;
		static PFNGLGETSHADERSOURCEPROC _mglGetShaderSource;
		static PFNGLGETUNIFORMLOCATIONPROC _mglGetUniformLocation;
		static PFNGLGETUNIFORMFVPROC _mglGetUniformfv;
		static PFNGLGETUNIFORMIVPROC _mglGetUniformiv;
		static PFNGLGETVERTEXATTRIBDVPROC _mglGetVertexAttribdv;
		static PFNGLGETVERTEXATTRIBFVPROC _mglGetVertexAttribfv;
		static PFNGLGETVERTEXATTRIBIVPROC _mglGetVertexAttribiv;
		static PFNGLGETVERTEXATTRIBPOINTERVPROC _mglGetVertexAttribPointerv;

		static PFNGLDELETESHADERPROC _mglDeleteShader;
		static PFNGLDETACHSHADERPROC _mglDetachShader;

		static PFNGLBINDATTRIBLOCATIONPROC _mglBindAttribLocation;
	};

	#define	glCreateShader GLWrapper::_mglCreateShader
	#define	glShaderSource GLWrapper::_mglShaderSource
	#define	glCompileShader GLWrapper::_mglCompileShader
	#define	glCreateProgram GLWrapper::_mglCreateProgram
	#define	glAttachShader GLWrapper::_mglAttachShader
	#define	glLinkProgram GLWrapper::_mglLinkProgram
	#define	glUseProgram GLWrapper::_mglUseProgram

	#define glUniform1f GLWrapper::_mglUniform1f
	#define glUniform2f GLWrapper::_mglUniform2f
	#define glUniform3f GLWrapper::_mglUniform3f
	#define glUniform4f GLWrapper::_mglUniform4f
	#define glUniform1i GLWrapper::_mglUniform1i
	#define glUniform2i GLWrapper::_mglUniform2i
	#define glUniform3i GLWrapper::_mglUniform3i
	#define glUniform4i GLWrapper::_mglUniform4i
	#define glUniform1fv GLWrapper::_mglUniform1fv
	#define glUniform2fv GLWrapper::_mglUniform2fv
	#define glUniform3fv GLWrapper::_mglUniform3fv
	#define glUniform4fv GLWrapper::_mglUniform4fv
	#define glUniform1iv GLWrapper::_mglUniform1iv
	#define glUniform2iv GLWrapper::_mglUniform2iv
	#define glUniform3iv GLWrapper::_mglUniform3iv
	#define glUniform4iv GLWrapper::_mglUniform4iv
	#define glUniformMatrix2fv GLWrapper::_mglUniformMatrix2fv
	#define glUniformMatrix3fv GLWrapper::_mglUniformMatrix3fv
	#define glUniformMatrix4fv GLWrapper::_mglUniformMatrix4fv

	#define glBindVertexArray GLWrapper::_mglBindVertexArray
	#define glDeleteVertexArrays GLWrapper::_mglDeleteVertexArrays
	#define glGenVertexArrays GLWrapper::_mglGenVertexArrays
	#define glIsVertexArray GLWrapper::_mglIsVertexArray

	#define glGenBuffers GLWrapper::_mglGenBuffers
	#define glBindBuffer GLWrapper::_mglBindBuffer
	#define glBufferData GLWrapper::_mglBufferData
	#define glVertexAttribPointer GLWrapper::_mglVertexAttribPointer
	#define glEnableVertexAttribArray GLWrapper::_mglEnableVertexAttribArray
	#define glGetAttribLocation GLWrapper::_mglGetAttribLocation

	#define mglGetAttachedShaders GLWrapper::_mglGetAttachedShaders
	#define glGetProgramiv GLWrapper::_mglGetProgramiv
	#define glGetProgramInfoLog GLWrapper::_mglGetProgramInfoLog
	#define glGetShaderiv GLWrapper::_mglGetShaderiv
	#define glGetShaderInfoLog GLWrapper::_mglGetShaderInfoLog
	#define glGetShaderSource GLWrapper::_mglGetShaderSource
	#define glGetUniformLocation GLWrapper::_mglGetUniformLocation
	#define glGetUniformfv GLWrapper::_mglGetUniformfv
	#define glGetUniformiv GLWrapper::_mglGetUniformiv
	#define glGetVertexAttribdv GLWrapper::_mglGetVertexAttribdv
	#define glGetVertexAttribfv GLWrapper::_mglGetVertexAttribfv
	#define glGetVertexAttribiv GLWrapper::_mglGetVertexAttribiv
	#define glGetVertexAttribPointerv GLWrapper::_mglGetVertexAttribPointerv

	#define glDeleteShader GLWrapper::_mglDeleteShader
	#define glDetachShader GLWrapper::_mglDetachShader

	#define glBindAttribLocation GLWrapper::_mglBindAttribLocation
*/
#endif