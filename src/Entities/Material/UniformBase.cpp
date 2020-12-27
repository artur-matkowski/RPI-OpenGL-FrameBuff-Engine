#include "UniformBase.hpp"
#include <GLES2/gl2.h>
#include "Material.hpp"

namespace asapgl
{
	template<class T>
	void Uniform<T>::SetUniform(const T& in)
	{
		m_data = in;
		m_owner->SetUniformDirty(this);
	}


	template class Uniform<int>;
	template<>
	void Uniform<int>::SendUniform()
	{
		glUniform1i(m_location, (int)m_data );
	}

	template class Uniform<float>;
	template<>
	void Uniform<float>::SendUniform()
	{
		glUniform1f(m_location, (float)m_data );
	}

}