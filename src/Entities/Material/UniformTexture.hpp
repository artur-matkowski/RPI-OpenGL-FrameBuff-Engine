#ifndef H_UniformTexture
#define H_UniformTexture
#include "UniformBase.hpp"
#include "MaterialType.hpp"


namespace asapi
{
	class UniformTexture: public UniformInterface
	{
		ResourcePtr<Texture>		m_data;
		stream						m_name;
	public:
		UniformTexture(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformInterface(location, metadataMemBlock)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		UniformTexture(const UniformTexture& cp) = delete;
		~UniformTexture(){};

		virtual void SendUniform();
		void SendUniform(const ResourcePtr<Texture>& override) const;

		virtual bool Is(const char* name) override { return (strcmp(name, m_name.c_str())==0); }

		void SetUniform(const ResourcePtr<Texture>& in);
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		void GetUniform(ResourcePtr<Texture>& out) { out = m_data; }
	};
}

#endif