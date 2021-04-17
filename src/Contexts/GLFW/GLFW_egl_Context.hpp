#ifndef _H_GLFW_egl_Context
#define _H_GLFW_egl_Context
#include "ContextBase.hpp"
#include "KeyCodes.hpp"
#include "backends/imgui_impl_glfw.h"

namespace asapi
{
	void WindowFocusCallback(GLFWwindow* window, int focused);
	
	class GLFW_egl_Context: public ContextBase
	{
		typedef void (GLFW_egl_Context::*PostRenderCallback)(void);
	protected:
		PostRenderCallback 				p_postRenderCallback 	= nullptr;

		GLFWwindow* 					window					= nullptr;
		glm::ivec2 						resolution;
		float 							deltaTime 				= 0.0f;


	void RenderGUIAndSwapBuffer();
	public:
		GLFW_egl_Context(bfu::MemBlockBase* memBlock = bfu::StdAllocatorMemBlock::GetMemBlock())
			:ContextBase(){}
		~GLFW_egl_Context(){};


		virtual void Init(const int argc, const char** argv) override;
		virtual void SwapBuffer() override;
		virtual void MainLoop() override;
		virtual void HandleContextEvents() override;
		virtual void CleanUp() override;
		virtual void GetResolution(uint16_t* X, uint16_t* Y) override;

	};
}

#endif