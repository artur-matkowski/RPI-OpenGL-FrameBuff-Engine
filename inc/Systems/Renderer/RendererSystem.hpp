#ifndef _H_RENDERERSYSTEM
#define _H_RENDERERSYSTEM
#include "object.hpp"
#include "Texture.hpp"

namespace asapgl{

	class RendererSystem: public object
	{
		int 				m_width;
		int 				m_height;





		bool m_hasAlpha;
		bool m_hasCompletedWithNoErrors = true;
		int m_sizeOfImage;

		int mi_width, mi_height;
		void *m_textureImage = 0;

		uint m_textureID = -1;

		Texture* texture = 0;
	
		void SendTextureToGPU(void *textureImage);
		void BindTexture();

	public:
		RendererSystem();
		~RendererSystem();

		void Init();
		void SetupEvents();
		void Render();
	};

}


#endif