#ifndef _H_UniformBase
#define _H_UniformBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class Material;

	class UniformBase: public object, public bfu::SerializableClassBase
	{
	protected:
		uint32_t 				m_location = -1;
		Material* 				m_owner;	
	public:
		UniformBase(uint32_t location, Material* owner)
			:m_location(location)
			,m_owner(owner)
		{};
		~UniformBase(){};

		virtual void SendUniform() = 0;

		inline void SetUniformLocation(const uint32_t& location)
		{
			m_location = location;
		}
	};

	template<typename T>
	class Uniform: public UniformBase
	{
		bfu::SerializableVar<T>	m_data;
	public:
		Uniform(uint32_t location, Material* owner)
			:UniformBase(location, owner)
			,m_data("m_data", this)
		{};
		~Uniform(){};

		virtual void SendUniform();

		virtual void SetUniform(const T& in);
		
		virtual void GetUniform(T& out)
		{
			out = m_data;
		}

	};
}

#endif