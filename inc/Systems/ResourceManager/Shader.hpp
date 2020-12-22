#ifndef _H_Shader
#define _H_Shader
#include <GLES2/gl2.h>

namespace asapgl
{
	class Shader
	{
		uint32_t m_programID = -1;
	public:
		Shader(const char*);
		~Shader();

		inline void UseProgram()
		{
			glUseProgram(m_programID);
		}

		inline uint32_t Get()
		{
			return m_programID;
		}
		
	};
}

#endif