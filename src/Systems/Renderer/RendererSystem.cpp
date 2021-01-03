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
	    	m_width = args->m_width; 
	    	m_height = args->m_height; 
			log::debug << "resolution update invoked on RendererSystem: " << m_width << "x" << m_height  << std::endl;
	    });

	    events.RegisterCallback<KeyboardEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    KeyboardEvent* args = (KeyboardEvent*)&a;
			log::debug << "KeyboardEvent: " << (int)args->m_key << "x" << (int)args->m_state  << std::endl;
	    });

	    events.RegisterCallback<MouseClickEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    MouseClickEvent* args = (MouseClickEvent*)&a;
			log::debug << "MouseClickEvent: " << (int)args->m_key << "x" << (int)args->m_state  << "  " <<  (int)args->m_Xpos << "x" << (int)args->m_Ypos  << std::endl;
	    });

	    events.RegisterCallback<MouseMoveEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    MouseMoveEvent* args = (MouseMoveEvent*)&a;
			log::debug << "MouseMoveEvent: " <<  (int)args->m_Xpos << "x" << (int)args->m_Ypos  << std::endl;
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
		glViewport(0, 0, m_width, m_height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		m_rendererComponent->Render();
	}
			
	

}