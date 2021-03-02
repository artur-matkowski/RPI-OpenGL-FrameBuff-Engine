#include "UniformOverride.hpp"

namespace asapi
{
	template<class T>
	UniformOverride<T>::UniformOverride( const Uniform<T>* targetUniform, MaterialInstance* owner)
		:UniformOverrideBase(owner)
		,m_targetUniform(targetUniform)
		//,SERIALIZABLE_VAR_CONSTRUCTOR(m_data, this)
	{

	}

	template<class T>
	void UniformOverride<T>::PushUniformOverride()
	{
		( (Uniform<T>*)m_targetUniform )->SendUniform(m_data);
	}


	template<class T>
	void UniformOverride<T>::PopUniformOverride()
	{
		( (Uniform<T>*)m_targetUniform )->SendUniform();
	}


	template class UniformOverride<float>;
	template class UniformOverride<int>;
	template class UniformOverride<glm::vec3>;
	template class UniformOverride<glm::mat4>;
	template class UniformOverride<ResourcePtr<Texture>>;
}