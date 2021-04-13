#include "Mesh.hpp"

namespace asapi
{
	Mesh::Mesh(const char* filename)
	{
		static GLfloat vertices[] = {
			 0.0f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
		};

		static GLfloat texCoords[] = {
			 0.0f,  0.0f,
			 0.0f,  1.0f,
			 1.0f,  1.0f, 
		};

		static GLfloat colors[] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
		};

		static GLfloat vertexbuff[] = {
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 
		};
		static GLuint indices[3] = {0,1,2};

		m_vertices = vertices;
		m_texCoords = texCoords;
		m_colors = colors;
		m_size = 3;


 		glGenBuffers(1, &vertex_buffer);
 		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 9, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * 9, vertexbuff);


        glGenBuffers(1, &indice_array);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indice_array);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*3, indices);




	}


	Mesh::Mesh(glm::vec2 resolution)
	{
		static GLfloat verticesO[] = {
			 0.5f, -0.1f, 0.0f,
			 0.1f, -0.5f, 0.0f,
			 0.0f,  0.0f, 0.0f,
			 0.0f,  0.0f, 0.0f,
			 1.0f, -0.9f, 0.0f,
			 0.9f, -1.0f, 0.0f,
		};
		static GLfloat vertices[] = {
			 0.5f, -0.1f, 0.0f,
			 0.1f, -0.5f, 0.0f,
			 0.0f,  0.0f, 0.0f,
			 0.0f,  0.0f, 0.0f,
			 1.0f, -0.9f, 0.0f,
			 0.9f, -1.0f, 0.0f,
		};

		static GLfloat texCoords[] = {
			 0.0f,  0.0f,
			 0.0f,  1.0f,
			 1.0f,  1.0f, 
			 0.0f,  1.0f, 
			 1.0f,  1.0f, 
			 1.0f,  1.0f, 
		};

		static GLfloat colors[] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
		};

		m_vertices = vertices;
		m_texCoords = texCoords;
		m_colors = colors;
		m_size = 6;

		for(int i=0; i<18; i+=3)
		{
			m_vertices[i] = verticesO[i] * resolution.y/resolution.x;
		}

		for(int i=0; i<18; ++i)
		{
			m_vertices[i] = m_vertices[i] * 50.f/resolution.x;
		}
	}
}