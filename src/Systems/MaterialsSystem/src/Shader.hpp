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

		static Shader* LinkShader(GLuint vertex, GLuint fragment);
	public:
		Shader();
		~Shader();

		static std::string 		s_projectPath; //tmp TODO remove that crap

		inline void UseProgram()
		{
			glUseProgram((uint32_t)(size_t)h_shaderHandle);
		}

		inline uint32_t GetProgramID()
		{
			return (uint32_t)(size_t)h_shaderHandle;
		}

		inline bool IsValid()
		{
			return h_shaderHandle!=nullptr;
		}
		
		static Shader* LoadShaderFromFile(const char* filename);
		bool LoadShaderFromResourceRawHandle(const void* resourceHandle);

		
		static void Compile(const char* dest, const char* source);
	};
}

#endif