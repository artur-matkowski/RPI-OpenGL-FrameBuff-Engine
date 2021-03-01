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
		//glm::mat4 m = glm::mat4(1.0f);
		log::debug << "Ccalling SendUniform() for mat4" << std::endl;

		glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(override) );
	}
	template<>
	void Uniform<glm::mat4>::OnGUI(const char* UniformName)
	{
		ImGui::InputFloat4(UniformName, glm::value_ptr(m_data)+0 );
		ImGui::InputFloat4(UniformName, glm::value_ptr(m_data)+4 );
		ImGui::InputFloat4(UniformName, glm::value_ptr(m_data)+8 );
		ImGui::InputFloat4(UniformName, glm::value_ptr(m_data)+12 );
	}

}