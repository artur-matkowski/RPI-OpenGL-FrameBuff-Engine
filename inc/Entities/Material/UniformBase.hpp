#ifndef _H_UniformBase
#define _H_UniformBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class MaterialType;
	class MaterialInstance;
	class UniformOverrideBase;

	class UniformBase: public object//, public bfu::SerializableClassBase
	{
	protected:
		uint32_t 				m_location = -1;
		MaterialType* 			m_owner;	
	public:
		UniformBase(uint32_t location, MaterialType* owner)
			:m_location(location)
			,m_owner(owner)
		{};
		~UniformBase(){};

		virtual void SendUniform() = 0;
		#ifdef IS_EDITOR
		virtual void OnGUI(const char* UniformName);
		#endif

		inline void SetUniformLocation(const uint32_t& location)
		{
			m_location = location;
		}

		virtual UniformOverrideBase* BuildUniformOverride(MaterialInstance* materialInstanceOverrideOwner) = 0;
	};

	template<typename T>
	class Uniform: public UniformBase
	{
		//bfu::SerializableVar<T>	m_data;
		T	m_data;
	public:
		Uniform(uint32_t location, MaterialType* owner)
			:UniformBase(location, owner)
			//,m_data("m_data", this)
		{};
		~Uniform(){};

		virtual void SendUniform();
		virtual void SendUniform(const T& override) const;

		virtual void SetUniform(const T& in);
		#ifdef IS_EDITOR
		virtual void OnGUI(const char* UniformName);
		#endif
		
		virtual void GetUniform(T& out)
		{
			out = m_data;
		}

		virtual UniformOverrideBase* BuildUniformOverride(MaterialInstance* materialInstanceOverrideOwner);

	};
}

#endif