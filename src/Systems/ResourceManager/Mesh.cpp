#include "Mesh.hpp"

namespace asapgl
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

		m_vertices = vertices;
		m_texCoords = texCoords;
		m_colors = colors;
		m_size = 3;
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

		for(int i=0; i<sizeof(vertices); i+=3)
		{
			m_vertices[i] = verticesO[i] * resolution.y/resolution.x;
		}

		for(int i=0; i<sizeof(vertices); ++i)
		{
			m_vertices[i] = m_vertices[i] * 50.f/resolution.x;
		}
	}
}