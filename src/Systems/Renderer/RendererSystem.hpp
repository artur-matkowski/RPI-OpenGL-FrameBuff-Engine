#ifndef _H_RENDERERSYSTEM
#define _H_RENDERERSYSTEM
#include "object.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "RendererComponent.hpp"


namespace asapi{

	class RendererSystem: public object
	{
		glm::ivec2			m_resolution;

		std::vector<RendererComponent*> 
							v_rendererComponents;


		//RendererComponent* 	m_rendererComponent = 0;

	static void ResizeWidowCallback(void* _this, void* data);

	public:
		RendererSystem();
		~RendererSystem();

		void Init();
		void Render();

		void RegisterRenderer(RendererComponent*);
		void UnRegisterRenderer(RendererComponent*);
	};

}


#endif