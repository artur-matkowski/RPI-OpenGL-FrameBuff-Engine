#ifndef _H_Mesh
#define _H_Mesh
#include <GLES2/gl2.h>


namespace asapgl
{
	class Mesh
	{
		GLfloat* m_vertices = 0;
		GLfloat* m_texCoords = 0;
		GLfloat* m_colors = 0;

	public:
		Mesh(const char*);
		~Mesh(){};

		inline void Render()
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, m_vertices);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, m_colors);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, m_texCoords);
			glEnableVertexAttribArray(2);

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		
	};
}

#endif