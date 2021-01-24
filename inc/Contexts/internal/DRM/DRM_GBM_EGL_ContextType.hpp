#ifndef _H_DRM_GBM_EGL_ContextType
#define _H_DRM_GBM_EGL_ContextType
#include "ContextBase.hpp"
#include "DevInput.hpp"
#include "glm.hpp"

namespace asapgl
{

	class DRM_GBM_EGL_ContextType: public ContextBase
	{
		devinput 	m_devinput;
		glm::ivec2 	resolution;
	public:

		DRM_GBM_EGL_ContextType()
			:ContextBase(){};
		~DRM_GBM_EGL_ContextType();

		virtual void Init(const int argc, const char** argv) override;
		virtual void SwapBuffer() override;
		virtual void MainLoop() override;
		virtual void HandleContextEvents() override;
		virtual void CleanUp() override;
	};

}

#endif