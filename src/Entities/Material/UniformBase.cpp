#include "UniformBase.hpp"
#include <GLES2/gl2.h>
#include "MaterialType.hpp"
#include "UniformOverride.hpp"
#include <gtc/type_ptr.hpp>

namespace asapi
{
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

	template class Uniform<glm::mat4>;
	template<>
	void Uniform<glm::mat4>::SendUniform()
	{
		glUniformMatrix4fv(m_location, 16, false, glm::value_ptr(m_data) );
	}
	template<>
	void Uniform<glm::mat4>::SendUniform(const glm::mat4& override) const
	{
		glUniformMatrix4fv(m_location, 16, false, glm::value_ptr(override) );
	}

}