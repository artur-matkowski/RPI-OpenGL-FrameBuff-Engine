#ifndef _H_Mesh
#define _H_Mesh
#include <GLES2/gl2.h>
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
		Mesh(glm::vec2 resolution);
		Mesh(const char*);
		~Mesh();

		void* GetRawHandle(){ return (void*)h_meshHandle; }

		inline void Render()
		{
        	uint32_t* config = (uint32_t*)h_meshHandle;

        	if(config==nullptr)
        		return;

			// //Render
			glBindBuffer(GL_ARRAY_BUFFER, config[0]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config[1]);

			const uint32_t attributes = config[2];
			for(uint32_t i = 0; i<attributes; ++i)
			{
				const uint32_t index = i*5+3;
				glEnableVertexAttribArray(config[index+0]);
				glVertexAttribPointer(config[index+1]
									, config[index+2]
									, GL_FLOAT
									, GL_FALSE
									, sizeof(GL_FLOAT)*(config[index+3])
									, (void*) (sizeof(GL_FLOAT)*(config[index+4])) );
			}


			const uint32_t size = config[3+attributes*5];
			glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, NULL);

			// glDisableVertexAttribArray(0);
			// glDisableVertexAttribArray(2);

		}

		static void Compile(const char* dest, const char* source);
		
	};
}

#endif