#include "UniformOverride.hpp"

namespace asapgl
{
	template<class T>
	UniformOverride<T>::UniformOverride( const Uniform<T>* targetUniform, MaterialInstance* owner)
		:UniformOverrideBase(owner)
		,m_targetUniform(targetUniform)
		,m_data("m_data", this)
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
}