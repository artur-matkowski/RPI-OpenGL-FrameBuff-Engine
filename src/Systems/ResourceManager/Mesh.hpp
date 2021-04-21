#ifndef _H_Mesh
#define _H_Mesh
#include <GLES2/gl2.h>
#include <vector>
#include "glm.hpp"

namespace asapi
{
	class Mesh
	{
		GLuint 		vertex_buffer;
		GLuint 		indice_array;
		GLint 		m_size = -1;

		bool 		m_hasPosition;
		bool		m_hasNormals;
		uint32_t	m_numUvChannels;


		std::vector<uint32_t>	v_VBOconfig;
		std::vector<uint32_t>	v_VBOindices;
		std::vector<float> 		v_VBOdata;


		#ifdef IS_EDITOR
		char name[256];
		#endif

	public:
		Mesh(glm::vec2 resolution);
		Mesh(const char*);
		~Mesh();

		inline void Render()
		{
			//Render
			// Step 1
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);


			// Step 2
		    glEnableVertexAttribArray(0);
		    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, nullptr);

		    glEnableVertexAttribArray(1);
		    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*5) );

		    glEnableVertexAttribArray(2);
		    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*3) );


			// Step 3
			glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, NULL);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

		}

		static void Compile(const char* dest, const char* source);
		
	};
}

#endif