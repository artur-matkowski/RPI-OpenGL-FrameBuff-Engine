#ifndef _H_Mesh
#define _H_Mesh
#include <GLES2/gl2.h>
#include "glm.hpp"

namespace asapi
{
	class Mesh
	{
		GLfloat* 	m_vertices = 0;
		GLfloat* 	m_texCoords = 0;
		GLfloat* 	m_colors = 0;
		GLint 		m_size = 0;

		GLuint 		vertex_buffer;
		GLuint 		indice_array;


	public:
		Mesh(glm::vec2 resolution);
		Mesh(const char*);
		~Mesh(){};

		inline void Render()
		{
			// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
			// glEnableVertexAttribArray(0);

			// glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, m_colors);
			// glEnableVertexAttribArray(1);

			// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords);
			// glEnableVertexAttribArray(2);

			// glDrawArrays(GL_TRIANGLES, 0, m_size);


			//Render
			// Step 1
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);


			// Step 3
		    glEnableVertexAttribArray(0);
		    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, nullptr);

		    glEnableVertexAttribArray(1);
		    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*5) );

		    glEnableVertexAttribArray(2);
		    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT)*9, (void*) (sizeof(GL_FLOAT)*3) );


			// Step 4
			glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, NULL);

		}
		
	};
}

#endif