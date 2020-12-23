#ifndef _H_Material
#define _H_Material
#include "ResourcePtr.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "object.hpp"


namespace asapgl
{
	class Material: public bfu::SerializableClassBase, public object
	{
		bfu::SerializableVar<std::string> 			m_usedShader;
		bfu::SerializableVarVector<std::string> 	m_usedTextures;

		ResourcePtr< Shader > 						m_shader;
		std::vector< ResourcePtr< Texture > > 		m_textures;

	public:
		Material(const char*);
		~Material(){};		


		inline void BindMaterial()
		{
			m_shader->UseProgram();

			int size = m_textures.size();

			for(int i=0; i<size; ++i)
			{
				m_textures[i]->BindTexture();
			}

			
			GLint texUnitLoc = glGetUniformLocation(m_shader->GetProgramID(), "texUnit");
			GLint blendLoc = glGetUniformLocation(m_shader->GetProgramID(), "blend");

			glUniform1i(texUnitLoc , 0);
			glUniform1f(blendLoc , 1.0);
		}
	};
}

#endif