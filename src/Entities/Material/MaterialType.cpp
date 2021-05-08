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
		DEALLOCATE_GLOBAL(p_uniforms);
		log::debug << "MaterialType::~MaterialType() " << std::endl;
	}

	MaterialType::MaterialType(const char* materialName)
	{
		OnIsDirty(materialName);
	}
	void MaterialType::OnIsDirty(const char* shaderName)
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

		if( newUniformsCount > m_uniformsCount && m_uniformsCount!=0)
		{
			for(int i=0; i<m_uniformsCount; ++i)
			{
				p_uniforms[i]->~UniformInterface();
				DEALLOCATE_GLOBAL(p_uniforms[i]);
			}
			DEALLOCATE_GLOBAL(p_uniforms);
		}
		m_uniformsCount = newUniformsCount;

		p_uniforms = (UniformInterface**)materialsMemBlock->allocate(m_uniformsCount, sizeof(UniformInterface*), alignof(UniformInterface*));


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
					systems.RESOURCES.requestResource( &texturePtr, "debug.png" );
		    		((Uniform<ResourcePtr<Texture>>*)p_uniforms[i])->SetUniform(texturePtr);
		    		break;		    		
		    	default:
		    		char buff[128];
		    		sprintf(buff, "%#04X", type);
		    		log::warning << "Unsuported uniform type found in " << shaderName << ": " << name << " type: " << buff << std::endl;
		    		break;
		    }
		}
	}

	#ifdef IS_EDITOR
	void MaterialType::OnGUI()
	{
		std::vector<std::string>* items = SYSTEMS::GetObject().RESOURCES.GetShadersPaths();

		if (ImGui::BeginCombo("Shader resource", m_shaderName))
        {
            for (int n = 0; n < items->size(); n++)
            {
            	const char* displayName = strstr( (*items)[n].c_str(), "/meshes/") + strlen("/meshes/");
                const bool is_selected = strcmp( m_shaderName, (*items)[n].c_str() ) == 0;
                if (ImGui::Selectable(displayName, is_selected))
                {
					strncpy(m_shaderName, displayName, 254);

					log::debug << "updated mesh name " << displayName << std::endl;

					OnIsDirty(m_shaderName);	
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
			p_uniforms[i]->OnGUI();
		}
	}
	#endif
}