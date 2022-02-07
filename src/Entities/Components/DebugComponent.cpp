#include "DebugComponent.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>

namespace asapi
{
	std::string vert = "attribute vec4 vPosition; \n"
	 "void main() \n"
	 "{ \n"
	 " gl_Position = vPosition; \n"
	 "} \n";

	std::string frag = "precision mediump float; \n"
	 "void main() \n"
	 "{ \n"
	 " gl_FragColor = vec4(1.0, 0.2, 0.2, 1.0); \n"
	 "} \n";

	float vertices[] = {
	    -0.5f, -0.5f, -5.0f,
	     0.5f, -0.5f, -5.0f,
	     0.0f,  0.5f, -5.0f
	};  


	unsigned int VBO;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	DebugComponent::DebugComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<DebugComponent>(mBlock)
	{



	}

	//virtual void OnAttach() override;
	void DebugComponent::OnIsDirty()
	{

	}
	
	#ifndef IS_EDITOR
	void DebugComponent::OnGUI()
	{
		ImGui::Text("DebugComponent::OnGUI()");
	}
	#endif


}