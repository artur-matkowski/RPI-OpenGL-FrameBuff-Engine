#ifndef _H_Shader
#define _H_Shader
#include "ResourceBase.hpp"
#include <GLES2/gl2.h>

namespace asapi
{
	class Shader: public ResourceBase
	{
		uint32_t m_programID = -1;
		Shader(uint32_t id);
	public:
		~Shader();

		inline void UseProgram()
		{
			glUseProgram(m_programID);
		}

		inline uint32_t GetProgramID()
		{
			return m_programID;
		}
		
		static Shader* LoadShader(const char* filename);
	};
}

#endif