#include "Mesh.hpp"
#include <cstring>

namespace asapi
{
	Mesh::Mesh(const char* path)
	{
		#ifdef IS_EDITOR
		int size = strlen(path);
		const char* tmp = &path[size-1];
		for(; *tmp!='/'; tmp-=1); // < find where file name is starting
		strncpy(name, tmp+1, 255);
		name[255] = '\0';
		#endif

		static GLfloat vertexbuff[] = {
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		//	^ verts 			^ UVs 		^ colors

		static GLuint indices[3] = {0, 1, 2};
		m_size = 3;


 		glGenBuffers(1, &vertex_buffer);
 		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_size * 9, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_size * 9, vertexbuff);


        glGenBuffers(1, &indice_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*m_size, indices);

	}


	Mesh::Mesh(glm::vec2 resolution)
	{
		static GLfloat vertexbuff[] = {
			 0.5f, -0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.1f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, -0.9f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.9f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};
		static GLfloat vertexbuff0[] = {
			 0.5f, -0.1f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.1f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			 1.0f, -0.9f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.9f, -1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};
		//	^ verts 			^ UVs 		^ colors

		static GLuint indices[6] = {0, 1, 2, 4, 5, 6};

		m_size = 6;

		for(int i=0; i<9*6; i+=9)
		{
			vertexbuff[i] = vertexbuff0[i] * resolution.y/resolution.x;
			vertexbuff[i+1] = vertexbuff0[i+1] * resolution.y/resolution.x;
		}

		for(int i=0; i<9*6; i+=9)
		{
			vertexbuff[i] = vertexbuff[i] * 50.f/resolution.x;
			vertexbuff[i+1] = vertexbuff[i+1] * 50.f/resolution.y;
		}


 		glGenBuffers(1, &vertex_buffer);
 		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_size * 9, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_size * 9, vertexbuff);


        glGenBuffers(1, &indice_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_size, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*m_size, indices);
	}

	void Mesh::Compile(const char* dest, const char* source)
	{
		
	}
}