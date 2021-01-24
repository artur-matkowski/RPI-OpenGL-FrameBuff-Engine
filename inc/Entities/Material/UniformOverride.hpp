#ifndef _H_UniformOverride
#define _H_UniformOverride
#include "MaterialInstance.hpp"
#include "MaterialType.hpp"
#include "Serialization.hpp"

namespace asapgl
{
	class MaterialInstance;

	class UniformOverrideBase: public object
	{
	protected:
		MaterialInstance* m_owner = 0;
	public:
		UniformOverrideBase(MaterialInstance* owner)
			:m_owner(owner)
		{};
		~UniformOverrideBase(){};


		virtual void PushUniformOverride() = 0;
		virtual void PopUniformOverride() = 0;
	};



	template<class T>
	class UniformOverride: public UniformOverrideBase
	{
	protected:
		const Uniform<T>* 				m_targetUniform;
		//SERIALIZABLE_VAR(T)				m_data;
		T m_data;
	public:
		UniformOverride( const Uniform<T>* targetUniform, MaterialInstance* owner);
		~UniformOverride(){};

		virtual void PushUniformOverride();
		virtual void PopUniformOverride();
		
	};
}

#endif