#include "UniformBase.hpp"
#include <GLES2/gl2.h>
#include "MaterialType.hpp"
#include "UniformOverride.hpp"
#include <gtc/type_ptr.hpp>
#ifdef IS_EDITOR
#include "imgui.h"
#endif

namespace asapi
{
	#ifdef IS_EDITOR
	void UniformBase::OnGUI(const char* UniformName)
	{
		ImGui::LabelText("Unsuported Uniform of: ", UniformName);
	}
	#endif


	template<class T>
	void Uniform<T>::SetUniform(const T& in)
	{
		m_data = in;
		m_owner->SetUniformDirty(this);
	}

	template<class T>
	UniformOverrideBase* Uniform<T>::BuildUniformOverride(MaterialInstance* materialInstanceOverrideOwner)
	{
		return new UniformOverride<T>(this, materialInstanceOverrideOwner);
	}

	template class Uniform<int>;
	template<>
	void Uniform<int>::SendUniform()
	{
		glUniform1i(m_location, (int)m_data );
	}
	template<>
	void Uniform<int>::SendUniform(const int& override) const
	{
		glUniform1i(m_location, override );
	}
	template<>
	void Uniform<int>::OnGUI(const char* UniformName)
	{
		ImGui::InputInt(UniformName, &m_data );
	}

	

	template class Uniform<float>;
	template<>
	void Uniform<float>::SendUniform()
	{
		glUniform1f(m_location, (float)m_data );
	}
	template<>
	void Uniform<float>::SendUniform(const float& override) const
	{
		glUniform1f(m_location, override );
	}
	template<>
	void Uniform<float>::OnGUI(const char* UniformName)
	{
		ImGui::InputFloat(UniformName, &m_data );
	}


	template class Uniform<glm::vec3>;
	template<>
	void Uniform<glm::vec3>::SendUniform()
	{
		glUniform3fv(m_location, 1, glm::value_ptr(m_data) );
	}
	template<>
	void Uniform<glm::vec3>::SendUniform(const glm::vec3& override) const
	{
		glUniform3fv(m_location, 3, glm::value_ptr(override) );
	}
	template<>
	void Uniform<glm::vec3>::OnGUI(const char* UniformName)
	{
		ImGui::InputFloat3(UniformName, glm::value_ptr(m_data) );
	}


	template class Uniform<glm::mat4>;
	template<>
	void Uniform<glm::mat4>::SendUniform()
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(m_data) );
	}
	template<>
	void Uniform<glm::mat4>::SendUniform(const glm::mat4& override) const
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(override) );
	}
	template<>
	void Uniform<glm::mat4>::OnGUI(const char* UniformName)
	{
		ImGui::LabelText(UniformName, "%3.2f    %3.2f    %3.2f    %3.2f"
														, glm::value_ptr(m_data)+0
														, glm::value_ptr(m_data)+1 
														, glm::value_ptr(m_data)+2 
														, glm::value_ptr(m_data)+3 );
		ImGui::Text("%3.2f    %3.2f    %3.2f    %3.2f"	, glm::value_ptr(m_data)+4
														, glm::value_ptr(m_data)+5 
														, glm::value_ptr(m_data)+6 
														, glm::value_ptr(m_data)+7 );
		ImGui::Text("%3.2f    %3.2f    %3.2f    %3.2f"	, glm::value_ptr(m_data)+8
														, glm::value_ptr(m_data)+9 
														, glm::value_ptr(m_data)+10 
														, glm::value_ptr(m_data)+11 );
		ImGui::Text("%3.2f    %3.2f    %3.2f    %3.2f"	, glm::value_ptr(m_data)+12
														, glm::value_ptr(m_data)+13 
														, glm::value_ptr(m_data)+14 
														, glm::value_ptr(m_data)+15 );
	}

	template class Uniform<ResourcePtr<Texture>>;
	template<>
	void Uniform<ResourcePtr< Texture >>::SendUniform()
	{
		m_data->BindTexture();
	}
	template<>
	void Uniform<ResourcePtr<Texture>>::SendUniform(const ResourcePtr<Texture>& override) const
	{
		override->BindTexture();
	}
	template<>
	void Uniform<ResourcePtr<Texture>>::OnGUI(const char* UniformName)
	{
		ImGui::LabelText(m_data->GetName(), UniformName);
		auto texid = m_data->GetTextureID();
		ImGui::Image((void*)texid, ImVec2(100.0f, 100.0f));
	}

}