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
		,mesh("/home/artur/Dokumenty/projekty/asapi/_exampleProject/assets_int/meshes/Plane.mmp")
	{
		SYSTEMS::GetObject().RENDERER.debugComponent = this;

		const char *vertexShaderSource = vert.c_str();
		const char *fragmentShaderSource = frag.c_str();

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9, vertices, GL_STATIC_DRAW);

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
		    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		    log::error << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
		    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		    log::error << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindAttribLocation(shaderProgram, 0, "vPosition");
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if(!success)
		{
		    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		    log::error << "ERROR::SHADER::linking_FAILED\n" << infoLog << std::endl;
		}

		



	}

	//virtual void OnAttach() override;
	void DebugComponent::OnIsDirty()
	{

	}
	#ifdef IS_EDITOR
	void DebugComponent::OnGUI()
	{

	}
	#endif


	void DebugComponent::Render()
	{
		//log::debug << "logging from Render() on debug component()" << std::endl;
		


		glUseProgram(shaderProgram);

		uint32_t* config = (uint32_t*)mesh.GetRawHandle();

		// //Render
		glBindBuffer(GL_ARRAY_BUFFER, config[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config[1]);

		const uint32_t attributes = config[2];
		for(uint32_t i = 0; i<attributes; ++i)
		{
			const uint32_t index = i*5+3;
			glEnableVertexAttribArray(config[index+0]);
			glVertexAttribPointer(config[index+1]
								, config[index+2]
								, GL_FLOAT
								, GL_FALSE
								, sizeof(GL_FLOAT)*(config[index+3])
								, (void*) (sizeof(GL_FLOAT)*(config[index+4])) );
		}


		const uint32_t size = config[3+attributes*5];
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);
	}
}