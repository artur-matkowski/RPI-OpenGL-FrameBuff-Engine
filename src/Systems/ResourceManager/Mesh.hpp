#ifndef _H_Mesh
#define _H_Mesh
#include <vector>
#include "RendererSystem.hpp"
#include "glm.hpp"

namespace asapi
{
	class RendererSystem;

	class Mesh
	{
		friend RendererSystem;
		
		tMeshHandle 			h_meshHandle = nullptr;


	public:
		Mesh(const char*);
		~Mesh();

		void* GetRawHandle(){ return (void*)h_meshHandle; }

		static void Compile(const char* dest, const char* source);
		
	};
}

#endif