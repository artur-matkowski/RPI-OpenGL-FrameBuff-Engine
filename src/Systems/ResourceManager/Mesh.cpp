#include "Mesh.hpp"

namespace asapgl
{
	Mesh::Mesh(const char* filename)
	{
		static GLfloat vertices[] = {
			 0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
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
	}
}