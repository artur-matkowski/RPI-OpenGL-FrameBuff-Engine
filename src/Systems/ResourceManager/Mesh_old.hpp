#ifndef _H_Mesh
#define _H_Mesh
#include <vector>
#include "RendererSystem.hpp"
#include "glm.hpp"

namespace asapi
{
	class RendererSystem;

	class Mesh_old
	{
		friend RendererSystem;
		
		tMeshHandle 			h_meshHandle = nullptr;


	public:
		Mesh_old(const char*);
		~Mesh_old();

		void* GetRawHandle(){ return (void*)h_meshHandle; }

		static void Compile(const char* dest, const char* source);
		
	};
}

#endif