#ifndef _H_Shader
#define _H_Shader
#include "RendererSystem.hpp"
#include <GLES2/gl2.h>

namespace asapi
{
	class RendererSystem;

	class Shader
	{
		friend RendererSystem;

		tShaderHandle 			h_shaderHandle = nullptr;

		const char* vertex_source = nullptr;
		const char* fragment_source = nullptr;
		const char* shaderName = "nullptr";

		Shader();

		static Shader* LinkShader(GLuint vertex, GLuint fragment);
	public:
		~Shader();

		inline void UseProgram()
		{
			glUseProgram((uint32_t)(size_t)h_shaderHandle);
		}

		inline uint32_t GetProgramID()
		{
			return (uint32_t)(size_t)h_shaderHandle;
		}
		
		//static Shader* LoadShaderFromSource(const char* vertex_source, const char* fragment_source, const char* filename);
		static Shader* LoadShaderFromFile(const char* filename);

		
		static void Compile(const char* dest, const char* source);
	};
}

#endif