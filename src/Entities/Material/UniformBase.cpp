#include "UniformBase.hpp"
#include <GLES2/gl2.h>
#include "MaterialType.hpp"
#include "UniformOverride.hpp"

namespace asapgl
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

}