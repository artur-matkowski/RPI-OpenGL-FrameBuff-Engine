#ifndef _H_MaterialComponent
#define _H_MaterialComponent
#include "ComponentBase.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <bitforge/utils/bfu.hpp>
#include "ResourcePtr.hpp"

namespace asapgl
{
	class MaterialComponent: public ComponentBase
	{
		bfu::SerializableVar<std::string> 			m_usedShader;
		bfu::SerializableVarVector<std::string> 	m_usedTextures;

		ResourcePtr< Shader > 						m_shader;
		std::vector< ResourcePtr< Texture > > 		m_textures;



	public:
		MaterialComponent();
		~MaterialComponent(){};


		virtual void OnAttach(){};
		virtual void OnDetach(){};


		inline void Render()
		{
			m_shader->UseProgram();

			int size = m_textures.size();

			for(int i=0; i<size; ++i)
			{
				m_textures[i]->BindTexture();
			}

			
			GLint texUnitLoc = glGetUniformLocation(m_shader->Get(), "texUnit");
			GLint blendLoc = glGetUniformLocation(m_shader->Get(), "blend");

			glUniform1i(texUnitLoc , 0);
			glUniform1f(blendLoc , 1.0);
		};
		
	};
}

#endif