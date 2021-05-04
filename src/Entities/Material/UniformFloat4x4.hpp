#ifndef H_UniformFloat4x4
#define H_UniformFloat4x4
#include "UniformBase.hpp"


namespace asapi
{
	class UniformFloat4x4: public UniformBase
	{
		glm::mat4	m_data;
		stream		m_name;
	public:
		UniformFloat4x4(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformBase(location, metadataMemBlock)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		UniformFloat4x4(const UniformFloat4x4& cp) = delete;
		~UniformFloat4x4(){};

		virtual void SendUniform();
		void SendUniform(const glm::mat4& override) const;

		virtual bool Is(const char* name) override { return (strcmp(name, m_name.c_str())==0); }

		void SetUniform(const glm::mat4& in);
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		void GetUniform(glm::mat4& out) { out = m_data; }
	};
}


#endif