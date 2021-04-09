#ifndef _H_DRM_GBM_EGL_ContextType
#define _H_DRM_GBM_EGL_ContextType
#include "ContextBase.hpp"
#include "DevInput.hpp"
#include "glm.hpp"

namespace asapi
{

	class DRM_GBM_EGL_ContextType: public ContextBase
	{
		typedef void (DRM_GBM_EGL_ContextType::*PostRenderCallback)(void);
		PostRenderCallback 				p_postRenderCallback = nullptr;

		devinput 	m_devinput;
		glm::ivec2 	resolution;


		void RenderGUIAndSwapBuffer();
	public:

		DRM_GBM_EGL_ContextType()
			:ContextBase(){};
		~DRM_GBM_EGL_ContextType();

		virtual void Init(const int argc, const char** argv) override;
		virtual void SwapBuffer() override;
		virtual void MainLoop() override;
		virtual void HandleContextEvents() override;
		virtual void CleanUp() override;
		virtual void GetResolution(uint16_t* X, uint16_t* Y) override;
	};

}

#endif