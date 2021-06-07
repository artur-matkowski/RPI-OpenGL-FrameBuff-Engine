#include "UniformBase.hpp"
#include "MaterialType.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>
#include <gtc/type_ptr.hpp>
#include <cstdio>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{

	UniformInterface::~UniformInterface()
	{}


	#ifdef IS_EDITOR
	bool UniformInterface::OnGUI()
	{
		ImGui::LabelText("Unsuported Uniform %s", m_name.c_str());
		return false;
	}
	#endif


	template class Uniform<float>;
	template<>
	void Uniform<float>::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform1f(m_location, (float)m_data );
			m_isDirty = false;
		}
	}
	template<>
	void Uniform<float>::SendUniform(const float& override) const
	{
		glUniform1f(m_location, override );
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<float>::OnGUI()
	{
		return ImGui::InputFloat(m_name.c_str(), &m_data );
	}
	#endif
	template<>
	void Uniform<float>::sscanf(const char* str )
	{
		::sscanf(str, "%f", &m_data);
		m_isDirty = true;
	}
	template<>
	int Uniform<float>::sprintf(char* str)
	{
		return ::sprintf(str, "%f", m_data);
	}



	template class Uniform<glm::vec3>;
	template<>
	void Uniform<glm::vec3>::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform3fv(m_location, 1, glm::value_ptr(m_data) );
			m_isDirty = false;
		}
	}
	template<>
	void Uniform<glm::vec3>::SendUniform(const glm::vec3& override) const
	{
		glUniform3fv(m_location, 1, glm::value_ptr(override) );
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<glm::vec3>::OnGUI()
	{
		return ImGui::InputFloat3(m_name.c_str(), glm::value_ptr(m_data) );
	}
	#endif
	template<>
	void Uniform<glm::vec3>::sscanf(const char* str )
	{
		::sscanf(str, "%f:%f:%f", &m_data.x, &m_data.y, &m_data.z);
		m_isDirty = true;
	}
	template<>
	int Uniform<glm::vec3>::sprintf(char* str)
	{
		return ::sprintf(str, "%f:%f:%f", m_data.x, m_data.y, m_data.z);
	}


	template class Uniform<glm::vec4>;
	template<>
	void Uniform<glm::vec4>::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform4fv(m_location, 1, glm::value_ptr(m_data) );
			m_isDirty = false;
		}
	}
	template<>
	void Uniform<glm::vec4>::SendUniform(const glm::vec4& override) const
	{
		glUniform4fv(m_location, 1, glm::value_ptr(override) );
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<glm::vec4>::OnGUI()
	{
		return ImGui::InputFloat4(m_name.c_str(), glm::value_ptr(m_data) );
	}
	#endif
	template<>
	void Uniform<glm::vec4>::sscanf(const char* str )
	{
		::sscanf(str, "%f:%f:%f:%f", &m_data.r, &m_data.g, &m_data.b, &m_data.a);
		m_isDirty = true;
	}
	template<>
	int Uniform<glm::vec4>::sprintf(char* str)
	{
		return ::sprintf(str, "%f:%f:%f:%f", m_data.r, m_data.g, m_data.b, m_data.a);
	}


	template class Uniform<glm::mat4>;
	template<>
	void Uniform<glm::mat4>::SendUniform()
	{
		if(m_isDirty)
		{
			glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(m_data) );
			m_isDirty = false;
		}
	}
	template<>
	void Uniform<glm::mat4>::SendUniform(const glm::mat4& override) const
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(override) );
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<glm::mat4>::OnGUI()
	{
		if (ImGui::TreeNode("Used model matrix"))
		{
			const float* data = glm::value_ptr(m_data);


			ImGui::LabelText(m_name.c_str(), "%.3f    %.3f    %.3f    %.3f"		
															, data[0]
															, data[4] 
															, data[8] 
															, data[12] );
			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[1]
															, data[5] 
															, data[9] 
															, data[13] );
			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[2]
															, data[6] 
															, data[10] 
															, data[14] );
			ImGui::Text("%.3f    %.3f    %.3f    %.3f"		, data[3]
															, data[7] 
															, data[11] 
															, data[15] );

	        ImGui::TreePop();
	    }
	    return false;
	}
	#endif
	template<>
	void Uniform<glm::mat4>::sscanf(const char* str )
	{
		::sscanf(str, "%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:"
					, &m_data[0][0], &m_data[0][1], &m_data[0][2], &m_data[0][3]
					, &m_data[1][0], &m_data[1][1], &m_data[1][2], &m_data[1][3]
					, &m_data[2][0], &m_data[2][1], &m_data[2][2], &m_data[2][3]
					, &m_data[3][0], &m_data[3][1], &m_data[3][2], &m_data[3][3]);
		m_isDirty = true;
	}
	template<>
	int Uniform<glm::mat4>::sprintf(char* str)
	{
		return ::sprintf(str, "%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:%f:"
					, &m_data[0][0], &m_data[0][1], &m_data[0][2], &m_data[0][3]
					, &m_data[1][0], &m_data[1][1], &m_data[1][2], &m_data[1][3]
					, &m_data[2][0], &m_data[2][1], &m_data[2][2], &m_data[2][3]
					, &m_data[3][0], &m_data[3][1], &m_data[3][2], &m_data[3][3]);
	}



	template class Uniform<int32_t>;
	template<>
	void Uniform<int32_t>::SendUniform()
	{
		if(m_isDirty)
		{
			glUniform1i(m_location, (int)m_data );
			m_isDirty = false;
		}
	}
	template<>
	void Uniform<int32_t>::SendUniform(const int32_t& override) const
	{
		glUniform1i(m_location, (int)override );
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<int32_t>::OnGUI()
	{
		return ImGui::InputInt(m_name.c_str(), &m_data );
	}
	#endif
	template<>
	void Uniform<int32_t>::sscanf(const char* str )
	{
		::sscanf(str, "%d", &m_data);
		m_isDirty = true;
	}
	template<>
	int Uniform<int32_t>::sprintf(char* str)
	{
		return ::sprintf(str, "%d", m_data);
	}





	template class Uniform<ResourcePtr<Texture>>;
	template<>
	void Uniform<ResourcePtr<Texture>>::SendUniform()
	{
		if(m_isDirty)
		{
			m_data->BindTexture();
		}
	}
	template<>
	void Uniform<ResourcePtr<Texture>>::SendUniform(const ResourcePtr<Texture>& override) const
	{
		override->BindTexture();
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<ResourcePtr<Texture>>::OnGUI()
	{
        bool ret = false;

		if (ImGui::BeginCombo("Texture resource", m_data->GetName() ))
        {
			std::vector<std::string>* items = SYSTEMS::GetObject().RESOURCES.GetTexturesPaths();

            for (int n = 0; n < items->size(); n++)
            {
            	const char* displayName = strstr( (*items)[n].c_str(), "/textures/") + strlen("/textures/");
                const bool is_selected = strcmp( m_data->GetName(), (*items)[n].c_str() ) == 0;
                if (ImGui::Selectable(displayName, is_selected))
                {
                	if( strcmp(m_data->GetName(), displayName)!=0 )
                	{
						log::debug << "updating texture name from: " << m_data->GetName() << " to: " << displayName << std::endl;

						SYSTEMS::GetObject().RESOURCES.requestResource( &m_data, displayName );
			    		m_isDirty = true;
			    		ret = true;
                	}
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }


		if( m_data.GetRawPtr()!=nullptr )
		{
			//ImGui::LabelText(m_name.c_str(), m_data->GetName());
			void* my_void_ptr = (void*)(intptr_t)m_data->GetTextureID();
			ImGui::Image(my_void_ptr, ImVec2(100.0f, 100.0f));

		}
		return ret;
	}
	#endif
	template<>
	void Uniform<ResourcePtr<Texture>>::sscanf(const char* str )
	{
		SYSTEMS::GetObject().RESOURCES.requestResource( &m_data, str );
		m_isDirty = true;
	}
	template<>
	int Uniform<ResourcePtr<Texture>>::sprintf(char* str)
	{
		int ret = -1;

		#ifdef IS_EDITOR
		if( m_data.GetRawPtr()!=nullptr )
		{
			ret = ::sprintf(str, "%s", m_data->GetName());
		}
		#else
		log::error << "can not save material in player" << std::endl;
		#endif

		return ret;
	}
}