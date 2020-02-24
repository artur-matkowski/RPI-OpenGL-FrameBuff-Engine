#ifndef _H_DRM_GBM_EGL_ContextType
#define _H_DRM_GBM_EGL_ContextType
#include "ContextBase.hpp"

namespace asapgl
{

	class DRM_GBM_EGL_ContextType: public ContextBase
	{
	public:
		class Args: public object{
		public:
		};

		DRM_GBM_EGL_ContextType(DRM_GBM_EGL_ContextType::Args &f);
		~DRM_GBM_EGL_ContextType();

		void SwapBuffer();
	};

}

#endif