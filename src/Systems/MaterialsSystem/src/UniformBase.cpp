#include "UniformBase.hpp"
#include "Systems.hpp"
#include <GLES2/gl2.h>
#include <gtc/type_ptr.hpp>
#include <cstdio>
#include "ResourcePNGProcessor.hpp"
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
		return m_isDirty = ImGui::InputFloat(m_name.c_str(), &m_data );
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
		return m_isDirty = ImGui::InputFloat3(m_name.c_str(), glm::value_ptr(m_data) );
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
		return m_isDirty = ImGui::InputFloat4(m_name.c_str(), glm::value_ptr(m_data) );
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
		return m_isDirty = ImGui::InputInt(m_name.c_str(), &m_data );
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





	template class Uniform<ResourcePNGSharedReference>;
	template<>
	void Uniform<ResourcePNGSharedReference>::SendUniform()
	{
		if(m_isDirty)
		{
			GLuint texId = (GLuint)(size_t) m_data.GetRawHandle();
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, texId );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
	}
	template<>
	void Uniform<ResourcePNGSharedReference>::SendUniform(const ResourcePNGSharedReference& override) const
	{
		GLuint texId = (GLuint)(size_t) m_data.GetRawHandle();

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texId );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	#ifdef IS_EDITOR
	template<>
	bool Uniform<ResourcePNGSharedReference>::OnGUI()
	{
        bool ret = false;
        uint64_t oldID = m_data.GetUuid();
        m_data.OnGUI_caller();

        ret = oldID!=m_data.GetUuid();

		if( m_data.GetRawHandle()!=nullptr )
		{
			//ImGui::LabelText(m_name.c_str(), m_data->GetName());
			void* my_void_ptr = (void*)(intptr_t)m_data.GetRawHandle();
			ImGui::Image(my_void_ptr, ImVec2(100.0f, 100.0f));

		}
		return ret;
	}
	#endif
	template<>
	void Uniform<ResourcePNGSharedReference>::sscanf(const char* str )
	{
		uint64_t uuid;
		UniqueID ID;

		::sscanf(str, "%llu", &uuid);
		ID = uuid;

		ResourcePNGSharedReference::InitializeObject( ID, &m_data );

		m_isDirty = true;
	}
	template<>
	int Uniform<ResourcePNGSharedReference>::sprintf(char* str)
	{
		int ret = -1;

		#ifdef IS_EDITOR
		ret = ::sprintf(str, "%llu", m_data.GetUuid());
		#else
		log::error << "can not save material in player" << std::endl;
		#endif

		return ret;
	}





}