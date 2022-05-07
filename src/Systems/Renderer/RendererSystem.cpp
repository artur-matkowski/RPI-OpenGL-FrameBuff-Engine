#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png.h>

#include "RendererSystem.hpp"
#include "Systems.hpp"

#include "RendererComponent.hpp"
#include "DebugComponent.hpp"


#include "Shader.hpp"



namespace asapi{

		std::vector<PointerColider*> 
							v_pointerColiderComponents;

		std::vector<RendererComponent*> 
							v_rendererComponents;

	void RendererSystem::Init()
	{
        bfu::MemBlockBase* metadataMemBlock = SYSTEMS::GetObject().MEMORY.GetSystemsAllocator();

		p_materialsMemBlock = (bfu::MemBlockBase*)metadataMemBlock->allocate(1, sizeof(bfu::StdPreAllocatedMemBlock), alignof(bfu::StdPreAllocatedMemBlock) );
        new (p_materialsMemBlock) bfu::StdPreAllocatedMemBlock(1024*1024, "Renderer-materials");
        SYSTEMS::GetObject().MEMORY.RegisterMemBlock(p_materialsMemBlock);

		bfu::EventSystem& es = SYSTEMS::GetObject().EVENTS;
		es.GetFastEvent("ResizeWindow")->RegisterCallback(this, RendererSystem::ResizeWidowCallback);

		uint16_t resX;
		uint16_t resY;
		SYSTEMS::GetObject().CONTEXT->GetResolution(&resX, &resY);
		m_resolution.x = resX;
		m_resolution.y = resY;


        //http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-9-vbo-indexing/
	}

	RendererSystem::RendererSystem()
	{
		v_rendererComponents.reserve(1024);
		v_pointerColiderComponents.reserve(1024);
	}
	RendererSystem::~RendererSystem()
	{

	}

	


	void RendererSystem::Render()
	{
        glViewport(0, 0, m_resolution.x, m_resolution.y);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		if(m_viewMatrix!=nullptr)
		{
			glm::mat4 viewMatrix = glm::inverse(*m_viewMatrix);

			for(auto it = v_rendererComponents.begin(); it!=v_rendererComponents.end(); ++it)
			{
				(*it)->Render(&m_projectionMatrix, &viewMatrix);
			}
		}
	}
	void RendererSystem::RenderPointerColisions()
	{
		glViewport(0, 0, m_resolution.x, m_resolution.y);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);

		if(m_viewMatrix!=nullptr)
		{
			glm::mat4 viewMatrix = glm::inverse(*m_viewMatrix);

			for(auto it = v_pointerColiderComponents.begin(); it!=v_pointerColiderComponents.end(); ++it)
			{
				(*it)->Render(&m_projectionMatrix, &viewMatrix);
			}
		}
	}
			
	void RendererSystem::RegisterPointerColider(PointerColider* element)
	{
		v_pointerColiderComponents.push_back(element);
	}
	void RendererSystem::UnRegisterPointerColider(PointerColider* element)
	{
		for(auto it = v_pointerColiderComponents.begin(); it!=v_pointerColiderComponents.end(); ++it)
		{
			if(element==*it)
			{
				v_pointerColiderComponents.erase(it);
				break;
			}
		}
	}
	
	void RendererSystem::RegisterRenderer(RendererComponent* element)
	{
		v_rendererComponents.push_back(element);
	}
	void RendererSystem::UnRegisterRenderer(RendererComponent* element)
	{
		for(auto it = v_rendererComponents.begin(); it!=v_rendererComponents.end(); ++it)
		{
			if(element==*it)
			{
				v_rendererComponents.erase(it);
				break;
			}
		}
	}

	void RendererSystem::ResizeWidowCallback(void* receiver, void* data)
	{
		RendererSystem* _this = (RendererSystem*)receiver;
		ResizeWindowArgs* args = (ResizeWindowArgs*)data;

    	_this->m_resolution.x = args->m_width; 
    	_this->m_resolution.y = args->m_height;
	}
	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
// Shader processing section                                                                                  //
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VERTEX_SOURCE  \
			"#version 100\n" \
			"precision mediump float;\n" \
			"attribute vec4 position;\n" \
			"uniform mat4 modelViewMat;\n" \
			"\n" \
			"void main()\n" \
			"{\n" \
			"  gl_Position = modelViewMat * position;\n" \
			"}\n"

#define FRAGMENT_SOURCE  \
			"#version 100\n" \
			"precision mediump float;\n" \
			"varying vec4 vcolor;\n" \
			"\n" \
			"void main()\n" \
			"{\n" \
			"  gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n" \
			"}\n"


	GLuint LoadSingleShader(const char* source, GLenum shaderType, const char* filename)
	{
		char b_source[1024*1024*1];
		GLint isCompiled = GL_FALSE;

		GLuint shader = glCreateShader(shaderType);

		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << "In shader: " << filename << "\n\t" << str << std::endl;

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shader); // Don't leak the shader.
			return 0;
		}

		return shader;
	}

	Shader* RendererSystem::ProcessShader_obsolete(const char* vertex_source, const char* fragment_source, const char* shaderName, Shader* out)
	{
		GLuint vertex, fragment;
		GLint isCompiled = GL_FALSE;
		uint32_t programID = -1;


		programID = glCreateProgram();

		if( vertex_source!=nullptr && fragment_source!=nullptr)
		{
			vertex = LoadSingleShader(vertex_source, GL_VERTEX_SHADER, shaderName);
			fragment = LoadSingleShader(fragment_source, GL_FRAGMENT_SHADER, shaderName);
		}
		else
		{
			vertex = LoadSingleShader(VERTEX_SOURCE, GL_VERTEX_SHADER, shaderName);
			fragment = LoadSingleShader(FRAGMENT_SOURCE, GL_FRAGMENT_SHADER, shaderName);
		}


		glAttachShader(programID, vertex);
		glAttachShader(programID, fragment);
		
		glBindAttribLocation(programID, ATTR_LOC_position, "position");
		glBindAttribLocation(programID, ATTR_LOC_normal, "normal");
		glBindAttribLocation(programID, ATTR_LOC_uv0, "uv0");
		glBindAttribLocation(programID, ATTR_LOC_uv1, "uv1");
		glBindAttribLocation(programID, ATTR_LOC_uv2, "uv2");
		glBindAttribLocation(programID, ATTR_LOC_uv3, "uv3");
		glBindAttribLocation(programID, ATTR_LOC_color0, "color0");
		glBindAttribLocation(programID, ATTR_LOC_color1, "color1");
		glBindAttribLocation(programID, ATTR_LOC_color2, "color2");
		glBindAttribLocation(programID, ATTR_LOC_color3, "color3");

		glLinkProgram(programID);

		glGetProgramiv(programID, GL_LINK_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE || vertex==0 || fragment==0)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

			std::string str(&errorLog[0]);

			log::error << str << std::endl;


			glDetachShader(programID, vertex);
			glDetachShader(programID, fragment);

			//linking fallback shader
			vertex = LoadSingleShader(VERTEX_SOURCE, GL_VERTEX_SHADER, shaderName);
			fragment = LoadSingleShader(FRAGMENT_SOURCE, GL_FRAGMENT_SHADER, shaderName);

			glAttachShader(programID, vertex);
			glAttachShader(programID, fragment);
			
			glBindAttribLocation(programID, ATTR_LOC_position, "position");
			glBindAttribLocation(programID, ATTR_LOC_normal, "normal");
			glBindAttribLocation(programID, ATTR_LOC_uv0, "uv0");
			glBindAttribLocation(programID, ATTR_LOC_uv1, "uv1");
			glBindAttribLocation(programID, ATTR_LOC_uv2, "uv2");
			glBindAttribLocation(programID, ATTR_LOC_uv3, "uv3");
			glBindAttribLocation(programID, ATTR_LOC_color0, "color0");
			glBindAttribLocation(programID, ATTR_LOC_color1, "color1");
			glBindAttribLocation(programID, ATTR_LOC_color2, "color2");
			glBindAttribLocation(programID, ATTR_LOC_color3, "color3");

			glLinkProgram(programID);
		}


		// Always detach shaders after a successful link.
		glDetachShader(programID, vertex);
		glDetachShader(programID, fragment);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

		if(out==nullptr)
			out = new Shader();
			
		out->h_shaderHandle = (tShaderHandle)(size_t)programID;


		return out;
	}
	void RendererSystem::DispouseShader_obsolete(Shader* shader)
	{
		glDeleteProgram((uint32_t)(size_t)shader->h_shaderHandle);
	}
}