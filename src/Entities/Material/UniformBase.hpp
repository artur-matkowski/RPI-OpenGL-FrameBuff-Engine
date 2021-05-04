#ifndef _H_UniformBase
#define _H_UniformBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class MaterialType;
	class MaterialInstance;

	class UniformBase: public object//, public bfu::SerializableClassBase
	{
	protected:
		uint32_t 				m_location = -1;
		bool 					m_isDirty = false;
		char* 					m_name = nullptr;
	public:
		UniformBase(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:m_location(location)
		{
			m_name = (char*)metadataMemBlock->allocate(strlen(uniformName)+1, sizeof(char), alignof(char));
			strcpy(m_name, uniformName);
		};
		~UniformBase();

		virtual void SendUniform() = 0;
		#ifdef IS_EDITOR
		virtual void OnGUI(const char* UniformName);
		#endif

		inline void SetUniformLocation(const uint32_t& location)
		{
			m_location = location;
		}
	};

	template<typename T>
	class Uniform: public UniformBase
	{
		//bfu::SerializableVar<T>	m_data;
		T	m_data;
	public:
		Uniform(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformBase(location, uniformName, metadataMemBlock)
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
	};
}

#endif