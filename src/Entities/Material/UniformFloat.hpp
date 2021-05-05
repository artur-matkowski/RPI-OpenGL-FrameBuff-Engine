#ifndef H_UniformFloat
#define H_UniformFloat
#include "UniformBase.hpp"


namespace asapi
{
	class UniformFloat: public UniformBase<UniformFloat>
	{
		float		m_data;
		stream		m_name;
	public:
		UniformFloat(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformBase(location, metadataMemBlock)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		UniformFloat(const UniformFloat& cp) = delete;
		~UniformFloat(){};

		virtual void SendUniform();
		void SendUniform(const float& override) const;

		virtual bool Is(const char* name) override { return (strcmp(name, m_name.c_str())==0); }

		void SetUniform(const float& in);
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		void GetUniform(float& out) { out = m_data; }
	};
}

#endif