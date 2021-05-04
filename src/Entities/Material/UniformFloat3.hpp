#ifndef H_UniformFloat3
#define H_UniformFloat3
#include "UniformBase.hpp"


namespace asapi
{
	class UniformFloat3: public UniformBase
	{
		glm::vec3	m_data;
		stream		m_name;
	public:
		UniformFloat3(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformBase(location, metadataMemBlock)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		UniformFloat3(const UniformFloat3& cp) = delete;
		~UniformFloat3(){};

		virtual void SendUniform();
		void SendUniform(const glm::vec3& override) const;

		virtual bool Is(const char* name) override { return (strcmp(name, m_name.c_str())==0); }

		void SetUniform(const glm::vec3& in);
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		void GetUniform(glm::vec3& out) { out = m_data; }
	};
}


#endif