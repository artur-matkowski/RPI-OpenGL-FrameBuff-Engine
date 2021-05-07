#ifndef _H_UniformBase
#define _H_UniformBase
#include "object.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapi
{
	class MaterialType;
	class MaterialInstance;

	using bfu::stream;



	class UniformInterface: public object
	{
	protected:
		uint32_t 				m_location 	= -1;
		bool 					m_isDirty 	= false;
		stream					m_name;
	public:
		UniformInterface(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:m_location(location)
			,m_name(metadataMemBlock)
		{
			m_name.sprintf(uniformName);
		};
		virtual ~UniformInterface();

		virtual void SendUniform() = 0;
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif

		inline void SetUniformLocation(const uint32_t& location)
		{
			m_location = location;
		}
		bool Is(const char* name) { return (strcmp(name, m_name.c_str())==0); }
		virtual void sscanf(const char*) = 0;
		virtual int sprintf(char*) = 0;
	};



	template<class T>
	class Uniform: public UniformInterface
	{
		T		m_data;
	public:
		Uniform(uint32_t location, const char* uniformName, bfu::MemBlockBase* metadataMemBlock)
			:UniformInterface(location, uniformName, metadataMemBlock)
		{
		};
		Uniform(const Uniform& cp) = delete;
		~Uniform(){};

		virtual void SendUniform();
		void SendUniform(const T& override) const;


		void SetUniform(const T& in) { m_data = in; m_isDirty = true; }
		#ifdef IS_EDITOR
		virtual void OnGUI();
		#endif
		
		inline void GetUniform(T& out) { out = m_data; }
		
		virtual void sscanf(const char*) override;
		virtual int sprintf(char*) override;
	};

}

#endif