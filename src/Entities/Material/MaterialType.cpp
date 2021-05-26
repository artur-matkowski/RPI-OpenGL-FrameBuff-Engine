#include "MaterialType.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>
#ifdef IS_EDITOR
#include "imgui.h"
#endif


namespace asapi
{

	MaterialType::~MaterialType()
	{
		for(int i=0; i<m_uniformsCount; ++i)
		{
			p_uniforms[i]->~UniformInterface();
			DEALLOCATE_GLOBAL(p_uniforms[i]);
		}
		if(p_uniforms!=nullptr)
			DEALLOCATE_GLOBAL(p_uniforms);
		log::debug << "MaterialType::~MaterialType() " << std::endl;
	}

	MaterialType::MaterialType(const char* materialName)
	{
		char path[MAX_PATH_SIZE];

		snprintf(path, MAX_PATH_SIZE-1, "%s/assets_ext/materials/%s.mat", SYSTEMS::GetObject().RESOURCES.GetProjectPath(), materialName);


		SYSTEMS::IO::MMAP mat;

		mat.InitForRead(path);

		if( !mat.IsValid() )
		{
			log::warning << "Could not find material: " << path << std::endl;
			return;
		}
		

		#ifdef IS_EDITOR
		strncpy(m_MaterialName, materialName, 254);
		strncpy(m_shaderName, (char*)mat.Data(), 254);
		#endif

		LoadShader( (char*)mat.Data() );

		char* data = (char*)mat.Data();
		for(; *data!='\0'; ++data );
		++data;

		for(int i=0; i<m_uniformsCount; ++i)
		{
			p_uniforms[i]->sscanf(data);
			
			for(; *data!='\0'; ++data );
			++data;
		}


	}
	void MaterialType::LoadShader(const char* shaderName)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();

		bfu::MemBlockBase* materialsMemBlock = systems.RENDERER.GetMaterialsMemBlock();
		bfu::MemBlockBase* metadataMemBlock = systems.MEMORY.GetSystemsAllocator();

		systems.RESOURCES.requestResource( &m_shader, shaderName );

		m_shader->UseProgram();


		GLint i;
		GLint count;
		uint32_t location;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 128; // maximum name length
		GLchar name[bufSize] = {0}; // variable name in GLSL
		GLsizei length; // name length	
		int32_t newUniformsCount = 0;


		glGetProgramiv(m_shader->GetProgramID(), GL_ACTIVE_UNIFORMS, &newUniformsCount);
		printf("Active Uniforms: %d\n", newUniformsCount);


		for(int i=0; i<m_uniformsCount; ++i)
		{
			p_uniforms[i]->~UniformInterface();
			DEALLOCATE_GLOBAL(p_uniforms[i]);
		}

		if( newUniformsCount > m_uniformsCount )
		{
			if( p_uniforms!=nullptr )
				DEALLOCATE_GLOBAL(p_uniforms);

			p_uniforms = (UniformInterface**)materialsMemBlock->allocate(newUniformsCount, sizeof(UniformInterface*), alignof(UniformInterface*));
		}
		m_uniformsCount = newUniformsCount;

		

		for (i = 0; i < m_uniformsCount; ++i)
		{
		    glGetActiveUniform(m_shader->GetProgramID(), (GLuint)i, bufSize, &length, &size, &type, name);

		    printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		    location = glGetUniformLocation(m_shader->GetProgramID(), name);

		    std::string s_name(name);
			ResourcePtr< Texture > texturePtr;

		    //TODO custom_allocator instead of operator new
		    switch(type)
		    {
		    	case GL_FLOAT:
		    		p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<float>), alignof(Uniform<float>));
		    		new (p_uniforms[i]) Uniform<float>(location, name, metadataMemBlock);
		    		break;
		    	case GL_FLOAT_VEC3:
		    		p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<glm::vec3>), alignof(Uniform<glm::vec3>));
		    		new (p_uniforms[i]) Uniform<glm::vec3>(location, name, metadataMemBlock);
		    		break;
		    	case GL_FLOAT_MAT4:
		    		p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<glm::mat4>), alignof(Uniform<glm::mat4>));
		    		new (p_uniforms[i]) Uniform<glm::mat4>(location, name, metadataMemBlock);
		    		break;
		    	case GL_SAMPLER_2D:
		    		p_uniforms[i] = (UniformInterface*)materialsMemBlock->allocate(1, sizeof(Uniform<ResourcePtr<Texture>>), alignof(Uniform<ResourcePtr<Texture>>));
		    		new (p_uniforms[i]) Uniform<ResourcePtr<Texture>>(location, name, metadataMemBlock);
					// systems.RESOURCES.requestResource( &texturePtr, "debug.png" );
		   //  		((Uniform<ResourcePtr<Texture>>*)p_uniforms[i])->SetUniform(texturePtr);
		    		break;		    		
		    	default:
		    		char buff[128];
		    		sprintf(buff, "%#04X", type);
		    		log::warning << "Unsuported uniform type found in " << shaderName << ": " << name << " type: " << buff << std::endl;
		    		break;
		    }
		}

		p_modelViewUniform = (Uniform<glm::mat4>*) GetUniformPtr("modelViewMat");
	}

	#ifdef IS_EDITOR
	void MaterialType::OnGUI()
	{
		std::vector<std::string>* items = SYSTEMS::GetObject().RESOURCES.GetShadersPaths();
        bool isAltered = false;

		if (ImGui::BeginCombo("Shader resource", m_shaderName))
        {
            for (int n = 0; n < items->size(); n++)
            {
            	const char* displayName = strstr( (*items)[n].c_str(), "/shaders/") + strlen("/shaders/");
                const bool is_selected = strcmp( m_shaderName, (*items)[n].c_str() ) == 0;
                if (ImGui::Selectable(displayName, is_selected))
                {
                	if( strcmp(m_shaderName, displayName)!=0 )
                	{
						strncpy(m_shaderName, displayName, MATERIAL_MAX_NAME_LENGTH-1);

						log::debug << "updated shader name: " << displayName << std::endl;

						LoadShader(m_shaderName);
						isAltered = true;
                	}
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

		for(int i=0; i<m_uniformsCount; ++i)
		{
			ImGui::Spacing();
			isAltered |= p_uniforms[i]->OnGUI();
		}

		if(isAltered)
		{
			OnIsDirty();
		}
	}
	void MaterialType::OnIsDirty()
	{
		char extPath[MAX_PATH_SIZE];

		if( strlen(m_MaterialName)!=0  )
		{
			snprintf(extPath, MAX_PATH_SIZE-1, "%s/assets_ext/materials/%s.mat", SYSTEMS::GetObject().RESOURCES.GetProjectPath(), m_MaterialName);

			SYSTEMS::IO::MMAP mat;

			mat.InitForWrite(extPath, 4096);
			char* data = (char*)mat.Data();

			data += snprintf(data, MATERIAL_MAX_NAME_LENGTH, m_shaderName) + 1;

			for(int i=0; i<m_uniformsCount; ++i)
			{
				data += p_uniforms[i]->sprintf(data) + 1;
			}
		}
		else
		{
			log::warning << "Could not save material as name length is 0 " << std::endl;
		}
		SYSTEMS::GetObject().RESOURCES.OnRenderersDirty();
	}
	#endif

	void MaterialType::Compile(const char* dest, const char* source)
	{
		FILE *src, *dst;
		long int srcSize, dstSize;

		src = fopen (source,"rb");
		fseek(src, 0L, SEEK_END); 
		srcSize = ftell(src); 
		fseek(src, 0L, SEEK_SET);

		char* buff = new char[srcSize];

		fread(buff, sizeof(char), srcSize, src);

		delete buff;

		fclose(src); 


		dst = fopen (dest,"wb");
		fwrite(buff, 1, srcSize, dst);
		fwrite(" \n ", 1, 3, dst);
		fclose(dst); 
	}
}