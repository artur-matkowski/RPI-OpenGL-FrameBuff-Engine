#include "Xlib_EGL_ContextType.hpp"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <bitforge/utils/bfu.hpp>


namespace asapgl
{
	struct Xlib_EGL_ContextType::XlibData
	{
		Display *x11;
	};

	struct Xlib_EGL_ContextType::EGLData
	{
		EGLDisplay egl;


		EGLContext context;
		EGLSurface surface;
		unsigned int width;
		unsigned int height;


		long unsigned int x11;
	};


	bool Xlib_EGL_ContextType::DisplayOpen(void)
	{
		EGLint major, minor;

		m_XlibData->x11 = XOpenDisplay(NULL);
		if (!m_XlibData->x11) {
			return false;
		}

		m_eglData->egl = eglGetDisplay(m_XlibData->x11);
		if (m_eglData->egl == EGL_NO_DISPLAY) {
			XCloseDisplay(m_XlibData->x11);
			return false;
		}

		if (!eglInitialize(m_eglData->egl, &major, &minor)) {
			XCloseDisplay(m_XlibData->x11);
			return false;
		}

		log::info << "EGL: " << major << "." << minor << std::endl;

		//printf("EGL: %d.%d\n", major, minor);

		return true;
	}




bool Xlib_EGL_ContextType::window_create(const char *name,
				    unsigned int x, unsigned int y,
				    unsigned int width, unsigned int height,
				    const int* attributes, const int* contextAttribs)
{
	XSetWindowAttributes attr;
	unsigned long mask;
	XVisualInfo visual;
	EGLint num_configs;
	XVisualInfo* _INFO;
	XSizeHints hints;
	EGLConfig config;
	int num_visuals;
	EGLint version;
	Window root;
	int screen;
	EGLint vid;


	screen = DefaultScreen(m_XlibData->x11);
	root = RootWindow(m_XlibData->x11, screen);

	if (!eglChooseConfig(m_eglData->egl, attributes, &config, 1, &num_configs)) {
		return false;
	}

	if (!eglGetConfigAttrib(m_eglData->egl, config, EGL_NATIVE_VISUAL_ID, &vid)) {
		return false;
	}

	visual.visualid = vid;

	_INFO = XGetVisualInfo(m_XlibData->x11, VisualIDMask, &visual, &num_visuals);
	if (!_INFO) {
		return false;
	}

	memset(&attr, 0, sizeof(attr));
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = XCreateColormap(m_XlibData->x11, root, _INFO->visual, AllocNone);
	attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	m_eglData->x11 = XCreateWindow(m_XlibData->x11, root, 0, 0, width, height,
				    0, _INFO->depth, InputOutput, _INFO->visual,
				    mask, &attr);
	if (!m_eglData->x11) {
		return false;
	}

	memset(&hints, 0, sizeof(hints));
	hints.x = x;
	hints.y = y;
	hints.width = width;
	hints.height = height;
	hints.flags = USSize | USPosition;

	XSetNormalHints(m_XlibData->x11, m_eglData->x11, &hints);
	XSetStandardProperties(m_XlibData->x11, m_eglData->x11, name, name, None,
			       NULL, 0, &hints);

	eglBindAPI(EGL_OPENGL_ES_API);

	m_eglData->context = eglCreateContext(m_eglData->egl, config,
					   EGL_NO_CONTEXT, contextAttribs);
	if (m_eglData->context == EGL_NO_CONTEXT) {
		return false;
	}

	eglQueryContext(m_eglData->egl, m_eglData->context, EGL_CONTEXT_CLIENT_VERSION, &version);
	log::info << "OpenGL ES: " << version << std::endl;
	
	//printf("OpenGL ES: %d\n", version);

	m_eglData->surface = eglCreateWindowSurface(m_eglData->egl, config,
						 m_eglData->x11, NULL);
	if (m_eglData->surface == EGL_NO_SURFACE) {
		return false;
	}

	XFree(_INFO);

	m_eglData->width = width;
	m_eglData->height = height;

	return true;
}


void Xlib_EGL_ContextType::window_show()
{
	XMapWindow(m_XlibData->x11, m_eglData->x11);

	if (!eglMakeCurrent(m_eglData->egl, m_eglData->surface, m_eglData->surface, m_eglData->context))
		fprintf(stderr, "eglMakeCurrent():\n");

	XFlush(m_XlibData->x11);
}


	Xlib_EGL_ContextType::Xlib_EGL_ContextType(const int* attributes, const int* contextAttribs, const int argc, const char** argv)
		:m_XlibData(new struct XlibData)
		,m_eglData(new struct EGLData)
	{
		const unsigned int width = 640;
		const unsigned int height = 480;

		DisplayOpen();
		/*
		if (!display) {
			fprintf(stderr, "failed to open display\n");
			return;
		}*/

		window_create( "argv[0]", 0, 0, width, height, attributes, contextAttribs);
		/*if (!window) {
			fprintf(stderr, "failed to create window\n");
			return;
		}*/

		window_show();


	}
	
	Xlib_EGL_ContextType::~Xlib_EGL_ContextType()
	{
		eglDestroySurface(m_eglData->egl, m_eglData->surface);
		eglDestroyContext(m_eglData->egl, m_eglData->context);
		XDestroyWindow(m_XlibData->x11, m_eglData->x11);


		eglTerminate(m_eglData->egl);
		XCloseDisplay(m_XlibData->x11);
	}

	void Xlib_EGL_ContextType::SwapBuffer()
	{
		eglSwapBuffers(m_eglData->egl, m_eglData->surface);
		glFlush();
	}
}