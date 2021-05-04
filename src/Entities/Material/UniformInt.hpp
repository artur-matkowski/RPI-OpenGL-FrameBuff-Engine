#ifndef H_UniformInt
#define H_UniformInt
#include "UniformBase.hpp"


namespace asapi
{
	class UniformInt: public UniformInterface
	{
		int32_t		m_data;
		stream		m_name;
	public:
		UniformInt(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformInterface(location, metadataMemBlock)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		UniformInt(const UniformInt& cp) = delete;
		~UniformInt(){};

		virtual void SendUniform();
		void SendUniform(const int32_t& override) const;

		virtual bool Is(const char* name) override { return (strcmp(name, m_name.c_str())==0); }

		void SetUniform(const int32_t& in);
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		void GetUniform(int32_t& out) { out = m_data; }
	};
}

#endif