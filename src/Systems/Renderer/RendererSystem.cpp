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
	}

	void RendererSystem::Init()
	{
		texture = new Texture("debug.png");
		shader = new Shader("---");
		mesh = new Mesh("---");
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




		shader->UseProgram();

		GLint texUnitLoc = glGetUniformLocation(shader->Get(), "texUnit");
		GLint blendLoc = glGetUniformLocation(shader->Get(), "blend");


		texture->BindTexture();
		glUniform1i(texUnitLoc , 0);
		glUniform1f(blendLoc , 1.0);


		mesh->Render();

	}
			
	

}