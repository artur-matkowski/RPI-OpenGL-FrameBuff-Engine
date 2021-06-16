#ifndef _H_Texture
#define _H_Texture
#include <GLES2/gl2.h>
#ifdef IS_EDITOR
#include <cstring>
#endif

namespace asapi
{
	class Texture
	{
		int m_width, m_height;

		uint32_t m_textureID = -1;

		#ifdef IS_EDITOR
		char name[256];
		#endif

		void* LoadPNG(const char*);

	public:
		Texture(const char* fileName);
		~Texture();

		void SendTextureToGPU(void *textureImage);

		inline const void BindTexture() const
		{
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_textureID );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}


		#ifdef IS_EDITOR
		const char* GetName(){ return this!=nullptr ? name : "nullptr"; }
		inline void SetName(const char* in){ strncpy(name, in, 256); }
		#endif
		
		uint32_t GetTextureID() {return m_textureID; } 

		static void Compile(const char* dest, const char* source);
	};
}

#endif