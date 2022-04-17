#ifndef _H_Texture
#define _H_Texture
#include <GLES2/gl2.h>
#ifdef IS_EDITOR
#include <cstring>
#endif

namespace asapi
{
	struct TextureData
	{
		int* m_width = nullptr;
		int* m_height = nullptr;
		uint8_t* m_encoding = nullptr;

		void* m_data = nullptr;
	};

	class Texture
	{
		uint32_t m_textureID = -1;
		TextureData m_textureData;


		#ifdef IS_EDITOR
		char name[256];
		#endif

		void LoadPNG(const char*);

	public:
		Texture(const char* fileName);
		~Texture();

		void SendTextureToGPU();

		inline const void BindTexture() const
		{
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, m_textureID );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		}


		#ifdef IS_EDITOR
		const char* GetName(){ return this!=nullptr ? name : "nullptr"; }
		inline void SetName(const char* in){ strncpy(name, in, 256); }
		#endif
		
		uint32_t GetTextureID() {return m_textureID; } 
		uint8_t GetTextureEncoding() { return *m_textureData.m_encoding; }

		static void Compile(const char* dest, const char* source);
	};
}

#endif