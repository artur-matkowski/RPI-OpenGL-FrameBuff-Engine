#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png.h>

#include "RendererSystem.hpp"
#include "ContextBase.hpp"
#include "Systems.hpp"



namespace asapi{

	void RendererSystem::Init()
	{
		//m_rendererComponent = new RendererComponent( SYSTEMS::SYSTEMS_ALLOCATOR );
		bfu::EventSystem& es = SYSTEMS::GetObject().EVENTS;
		es.GetFastEvent("ResizeWindow")->RegisterCallback(this, RendererSystem::ResizeWidowCallback);
	}

	RendererSystem::RendererSystem()
	{
		v_rendererComponents.reserve(1024);
	}
	RendererSystem::~RendererSystem()
	{

	}

	


	void RendererSystem::Render()
	{
		//m_rendererComponent->Render();

		for(auto it = v_rendererComponents.begin(); it!=v_rendererComponents.end(); ++it)
		{
			(*it)->Render();
		}
	}
			
	
	void RendererSystem::RegisterRenderer(RendererComponent* element)
	{
		v_rendererComponents.push_back(element);
	}
	void RendererSystem::UnRegisterRenderer(RendererComponent* element)
	{
		for(auto it = v_rendererComponents.begin(); it!=v_rendererComponents.end(); ++it)
		{
			if(element==*it)
			{
				v_rendererComponents.erase(it);
				break;
			}
		}
	}

	void RendererSystem::ResizeWidowCallback(void* receiver, void* data)
	{
		RendererSystem* _this = (RendererSystem*)receiver;
		ResizeWindowArgs* args = (ResizeWindowArgs*)data;

    	_this->m_resolution.x = args->m_width; 
    	_this->m_resolution.y = args->m_height; 
	}
}