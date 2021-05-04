#ifndef _H_UniformBase
#define _H_UniformBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class MaterialType;
	class MaterialInstance;

	using bfu::stream;

	class UniformBase: public object
	{
	protected:
		uint32_t 				m_location = -1;
		bool 					m_isDirty = false;
	public:
		UniformBase(uint32_t location, bfu::MemBlockBase* metadataMemBlock)
			:m_location(location)
		{
		};
		~UniformBase();

		virtual void SendUniform() = 0;
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif

		inline void SetUniformLocation(const uint32_t& location)
		{
			m_location = location;
		}
		virtual bool Is(const char* name) = 0;
	};

	template<typename T>
	class Uniform: public UniformBase
	{
		T			m_data;
		stream		m_name;
	public:
		Uniform(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformBase(location, metadataMemBlock)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		Uniform(const Uniform<T>& cp) = delete;
		~Uniform(){};

		virtual void SendUniform();
		virtual void SendUniform(const T& override) const;

		virtual bool Is(const char* name) override { return (strcmp(name, m_name.c_str())==0); }

		void SetUniform(const T& in);
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		virtual void GetUniform(T& out)
		{
			out = m_data;
		}
	};
}

#endif