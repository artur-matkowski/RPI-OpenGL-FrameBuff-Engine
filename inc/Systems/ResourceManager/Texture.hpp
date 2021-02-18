#ifndef _H_Texture
#define _H_Texture
#include <GLES2/gl2.h>

namespace asapi
{
	class Texture
	{
		int m_width, m_height;

		uint32_t m_textureID = -1;

		void* LoadPNG(const char*);

	public:
		Texture(const char* fileName);
		~Texture();

		void SendTextureToGPU(void *textureImage);

		inline void BindTexture()
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		
	};
}

#endif