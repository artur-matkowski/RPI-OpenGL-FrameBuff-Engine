#ifndef _H_DRM_GBM_EGL_ContextType
#define _H_DRM_GBM_EGL_ContextType
#include "ContextBase.hpp"

namespace asapgl
{

	class DRM_GBM_EGL_ContextType: public ContextBase
	{
	public:

		DRM_GBM_EGL_ContextType(const int* attributes, const int* contextAttribs, const int argc, const char** argv);
		~DRM_GBM_EGL_ContextType();

		virtual void SwapBuffer();
		virtual void GetResolution(int & width, int & height);
		virtual void HandleContextEvents();
	};

}

#endif