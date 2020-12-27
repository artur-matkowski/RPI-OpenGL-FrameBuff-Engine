#include "Material.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>


namespace asapgl
{
	Material::Material(const char* materialName)
		:bfu::SerializableClassBase()
		,m_usedShader("m_usedShader", this)
		,m_usedTextures("m_usedTextures", this)
	{
		SYSTEMS& systems = SYSTEMS::GetObject();

		systems.RESOURCES.requestResource( &m_shader, "---" );


		ResourcePtr< Texture > tmp;
		systems.RESOURCES.requestResource( &tmp, "debug.png" );
		m_textures.push_back(tmp);


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
		    		log::warning << "Unsuported attribute type found in " << materialName << ": " << name << std::endl;
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

		    switch(type)
		    {
		    	case GL_FLOAT:
		    		m_uniformMap[std::string(name)] = new Uniform<float>(location, this);
		    		break;
		    	default:
		    		log::warning << "Unsuported uniform type found in " << materialName << ": " << name << std::endl;
		    		break;
		    }
		}

	}
}