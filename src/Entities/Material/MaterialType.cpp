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
		for( std::map<std::string, UniformBase*>::iterator it = m_uniformMap.begin();
			 it != m_uniformMap.end();
			 ++it)
		{
			delete it->second;
		}
		log::debug << "MaterialType::~MaterialType() " << std::endl;
				
	}

	MaterialType::MaterialType(const char* materialName)
		//:SERIALIZABLE_VAR_CONSTRUCTOR(m_usedShader, this)
		//,SERIALIZABLE_VAR_VEC_CONSTRUCTOR(m_usedTextures, this)
	{
		static SYSTEMS& systems = SYSTEMS::GetObject();

		systems.RESOURCES.requestResource( &m_shader, materialName );

		m_shader->UseProgram();


		GLint i;
		GLint count;
		uint32_t location;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 128; // maximum name length
		GLchar name[bufSize] = {0}; // variable name in GLSL
		GLsizei length; // name length	



		glGetProgramiv(m_shader->GetProgramID(), GL_ACTIVE_ATTRIBUTES, &count);
		printf("Active Attributes: %d\n", count);

		for (i = 0; i < count; i++)
		{
		    glGetActiveAttrib(m_shader->GetProgramID(), (GLuint)i, bufSize, &length, &size, &type, name);

		    printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

		    switch(type)
		    {
		    	default:
		    		char buff[128];
		    		sprintf(buff, "%#04X", type);
		    		log::warning << "Unsuported attribute type found in " << materialName << ": " << name << " type: " << buff << std::endl;
		    		break;
		    }
		}



		glGetProgramiv(m_shader->GetProgramID(), GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);

		for (i = 0; i < count; i++)
		{
		    glGetActiveUniform(m_shader->GetProgramID(), (GLuint)i, bufSize, &length, &size, &type, name);

		    printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		    location = glGetUniformLocation(m_shader->GetProgramID(), name);

		    std::string s_name(name);
			ResourcePtr< Texture > texturePtr;
			Uniform<ResourcePtr<Texture>>* textureUniform;

		    //TODO custom_allocator instead of operator new
		    switch(type)
		    {
		    	case GL_FLOAT:
		    		m_uniformMap[s_name] = new Uniform<float>(location, this);
		    		break;
		    	case GL_FLOAT_VEC3:
		    		m_uniformMap[s_name] = new Uniform<glm::vec3>(location, this);
		    		break;
		    	case GL_FLOAT_MAT4:
		    		m_uniformMap[s_name] = new Uniform<glm::mat4>(location, this);
		    		break;
		    	case GL_SAMPLER_2D:
					systems.RESOURCES.requestResource( &texturePtr, "debug.png" );
					textureUniform = new Uniform<ResourcePtr<Texture>>(location, this);
		    		textureUniform->SetUniform(texturePtr);
		    		m_uniformMap[s_name] = textureUniform;
		    		break;		    		
		    	default:
		    		char buff[128];
		    		sprintf(buff, "%#04X", type);
		    		log::warning << "Unsuported uniform type found in " << materialName << ": " << name << " type: " << buff << std::endl;
		    		break;
		    }
		}
	}

	#ifdef IS_EDITOR
	void MaterialType::OnGUI()
	{
		for(auto it = m_uniformMap.begin(); it!=m_uniformMap.end(); ++it)
		{	
			ImGui::Spacing();
			it->second->OnGUI( it->first.c_str() );
		}
	}
	#endif
}