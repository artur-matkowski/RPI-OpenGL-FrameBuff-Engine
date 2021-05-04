#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png.h>

#include "RendererSystem.hpp"
#include "Systems.hpp"

#include "RendererComponent.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"


#define ATTR_LOC_position 	0
#define ATTR_LOC_normal 	1
#define ATTR_LOC_uv0 		2
#define ATTR_LOC_uv1 		3
#define ATTR_LOC_uv2 		4
#define ATTR_LOC_uv3 		5
#define ATTR_LOC_color0		6
#define ATTR_LOC_color1		7
#define ATTR_LOC_color2		8
#define ATTR_LOC_color3		9


namespace asapi{

		std::vector<RendererComponent*> 
							v_rendererComponents;

	void RendererSystem::Init()
	{
        p_metadataMemBlock = SYSTEMS::GetObject().MEMORY.GetSystemsAllocator();

		p_materialsMemBlock = (bfu::MemBlockBase*)p_metadataMemBlock->allocate(1, sizeof(bfu::StdPreAllocatedMemBlock), alignof(bfu::StdPreAllocatedMemBlock) );
        new (p_materialsMemBlock) bfu::StdPreAllocatedMemBlock(1024*1024, "Renderer-materials");
        SYSTEMS::GetObject().MEMORY.RegisterMemBlock(p_materialsMemBlock);

        p_metadataMemBlock = SYSTEMS::GetObject().MEMORY.GetSystemsAllocator();

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
// Mesh processing section                                                                                    //
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
//                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool RendererSystem::ProcessMesh(Mesh* mesh)
	{
		
        bool*       fp_hasPosition = (bool*) mesh->h_meshHandle;
        bool*       fp_hasNormals = &fp_hasPosition[1];
        uint32_t*   fp_arraySize = (uint32_t*) &fp_hasPosition[2];
        uint32_t*   fp_numUvChannels = &fp_arraySize[1];
        uint32_t*   fp_indiciesCount = &fp_numUvChannels[1];
        float*      fp_vertexData = (float*) &fp_indiciesCount[1];
        int*        fp_indiciesData = (int*) &fp_vertexData[*fp_arraySize];
        
		mesh->h_meshHandle = nullptr; // set handle to invalid state, as we retrieved what we needed

        const uint32_t vertexfields = (*fp_hasPosition ? 3 : 0)
                                + (*fp_hasNormals ? 3 : 0)
                                + *fp_numUvChannels * 2;

        const uint32_t atributesInUse = (*fp_hasPosition ? 1 : 0)
                                + (*fp_hasNormals ? 1 : 0)
                                + *fp_numUvChannels;



       	uint32_t* config = STD_NEW(atributesInUse*5+4, uint32_t);
       	uint32_t index = 3;

        config[2] = atributesInUse; // present attributes count

        glGenBuffers(1, &config[0]);
        glBindBuffer(GL_ARRAY_BUFFER, config[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * *fp_arraySize * vertexfields, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * *fp_arraySize * vertexfields, fp_vertexData);


        glGenBuffers(1, &config[1]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* *fp_indiciesCount, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)* *fp_indiciesCount, fp_indiciesData);




        if(fp_hasPosition)
        {
            config[index] = ATTR_LOC_position; 
            ++index;
            config[index] = 0; 
            ++index;
            config[index] = 3; 
            ++index;
            config[index] = vertexfields; 
            ++index;
            config[index] = 0; 
            ++index;
            //glEnableVertexAttribArray(0);
            //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, nullptr);
        }

        if(*fp_hasNormals)
        {
            config[index] = ATTR_LOC_normal;
            ++index;
            config[index] = 0;
            ++index;
            config[index] = 3;
            ++index;
            config[index] = vertexfields;
            ++index;
            config[index] = 0;
            ++index;
        }

        for(uint32_t UVchannel = 0; UVchannel<*fp_numUvChannels; ++UVchannel)
        {
            config[index] = ATTR_LOC_uv0 + UVchannel;
            ++index;
            config[index] = 2;
            ++index;
            config[index] = 2;
            ++index;
            config[index] = vertexfields;
            ++index;
            config[index] = 3;
            ++index;
            // glEnableVertexAttribArray(2);
            // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*3) );
        }

        config[index] = *fp_indiciesCount;

        mesh->h_meshHandle = (tMeshHandle)config;
	}

	bool RendererSystem::DispouseMesh(Mesh* mesh)
	{
		uint32_t* config = (uint32_t*)mesh->h_meshHandle;
		bool ret = false;
		if( config != nullptr)
		{
			glDeleteBuffers(1, &config[0]);
			glDeleteBuffers(1, &config[1]);
			DEALLOCATE_GLOBAL(config);
			ret = true;
		}
		return ret;
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

	Shader* RendererSystem::ProcessShader(const char* vertex_source, const char* fragment_source, const char* shaderName)
	{
		GLuint vertex, fragment;
		GLint isCompiled = GL_FALSE;
		uint32_t programID = -1;


		programID = glCreateProgram();


		vertex = LoadSingleShader(vertex_source, GL_VERTEX_SHADER, shaderName);
		fragment = LoadSingleShader(fragment_source, GL_FRAGMENT_SHADER, shaderName);


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
			
		Shader* ret = new Shader();
		ret->h_shaderHandle = (tShaderHandle)(size_t)programID;


		return ret;
	}
	bool RendererSystem::DispouseShader(Shader* shader)
	{
		glDeleteProgram((uint32_t)(size_t)shader->h_shaderHandle);
		return true;
	}
}