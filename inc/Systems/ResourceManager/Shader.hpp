#ifndef _H_Shader
#define _H_Shader
#include "ResourceBase.hpp"
#include <GLES2/gl2.h>

namespace asapi
{
	class Shader
	{
		uint32_t m_programID = -1;
		Shader(uint32_t id);
	public:
		~Shader();

		inline void UseProgram()
		{
			glUseProgram(m_programID);
			// auto e = glGetError();
			// log::debug << "using program: " << m_programID << " " << e << std::endl;
		}

		inline uint32_t GetProgramID()
		{
			return m_programID;
		}
		
		static Shader* LoadShader(const char* filename);
	};
}

#endif