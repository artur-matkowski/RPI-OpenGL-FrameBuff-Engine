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
	void RendererSystem::SetupEvents()
	{
    	bfu::CallbackId id;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

		events.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
	    {
		    ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
	    	m_resolution.x = args->m_width; 
	    	m_resolution.y = args->m_height; 
			//log::debug << "resolution update invoked on RendererSystem: " << m_resolution.x << "x" << m_resolution.y  << std::endl;
	    });
	}

	void RendererSystem::Init()
	{
		//m_rendererComponent = new RendererComponent( SYSTEMS::SYSTEMS_ALLOCATOR );
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

}