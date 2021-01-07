#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <png.h>

#include "RendererSystem.hpp"
#include "ContextBase.hpp"
#include "Systems.hpp"



namespace asapgl{
	void RendererSystem::SetupEvents()
	{
    	bfu::CallbackId id;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

		events.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
	    {
		    ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
	    	m_resolution.x = args->m_width; 
	    	m_resolution.y = args->m_height; 
			log::debug << "resolution update invoked on RendererSystem: " << m_resolution.x << "x" << m_resolution.y  << std::endl;
	    });
	}

	void RendererSystem::Init()
	{
		m_rendererComponent = new RendererComponent();
	}

	RendererSystem::RendererSystem()
	{

	}
	RendererSystem::~RendererSystem()
	{

	}

	


	void RendererSystem::Render()
	{
		glViewport(0, 0, m_resolution.x, m_resolution.y);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		m_rendererComponent->Render();
	}
			
	

}