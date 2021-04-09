
#include <cstdio>

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <gbm.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <thread>

#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#include <bitforge/utils/bfu.hpp>

#include "DRM_GBM_EGL_ContextType.hpp"
#include "Systems.hpp"
#ifdef IS_EDITOR
#include "ImguiDRM.hpp"
#endif

#define EXIT(msg) { fputs (msg, stderr); exit (EXIT_FAILURE); }

int device = -1;

static drmModeConnector *find_connector (drmModeRes *resources) {
	// iterate the connectors
	int i;
	for (i=0; i<resources->count_connectors; i++) {
		drmModeConnector *connector = drmModeGetConnector (device, resources->connectors[i]);
		// pick the first connected connector
		if (connector->connection == DRM_MODE_CONNECTED) {
			return connector;
		}
		drmModeFreeConnector (connector);
	}
	// no connector found
	return NULL;
}

static drmModeEncoder *find_encoder (drmModeRes *resources, drmModeConnector *connector) {
	if (connector->encoder_id) {
		return drmModeGetEncoder (device, connector->encoder_id);
	}
	// no encoder found
	return NULL;
}

static uint32_t connector_id;
static drmModeModeInfo mode_info;
static drmModeCrtc *crtc;

static void find_display_configuration () {
	drmModeRes *resources = drmModeGetResources (device);
	// find a connector
	drmModeConnector *connector = find_connector (resources);
	if (!connector) EXIT ("no connector found\n");
	// save the connector_id
	connector_id = connector->connector_id;
	// save the first mode
	mode_info = connector->modes[0];

	// find an encoder
	drmModeEncoder *encoder = find_encoder (resources, connector);
	if (!encoder) EXIT ("no encoder found\n");
	// find a CRTC
	if (encoder->crtc_id) {
		crtc = drmModeGetCrtc (device, encoder->crtc_id);
	}
	drmModeFreeEncoder (encoder);
	drmModeFreeConnector (connector);
	drmModeFreeResources (resources);
}

static struct gbm_device *gbm_device;
static EGLDisplay display;
static EGLContext context;
static struct gbm_surface *gbm_surface;
static EGLSurface egl_surface;

static void setup_opengl (const int* attributes, const int* contextAttribs) 
{
	EGLint major, minor;
	EGLint version;
	EGLConfig config;
	EGLint num_config;


	gbm_device = gbm_create_device (device);
	display = eglGetDisplay (gbm_device);
	eglInitialize (display, &major, &minor);

	log::info << "EGL: " << major << "." << minor << std::endl;
	
	// create an OpenGL context
	eglChooseConfig (display, attributes, &config, 1, &num_config);
	eglBindAPI(EGL_OPENGL_ES_API);
	context = eglCreateContext (display, config, EGL_NO_CONTEXT, contextAttribs);

	eglQueryContext(display, context, EGL_CONTEXT_CLIENT_VERSION, &version);
	log::info << "OpenGL ES: " << version << std::endl;
	
	// create the GBM and EGL surface
	gbm_surface = gbm_surface_create (gbm_device, mode_info.hdisplay, mode_info.vdisplay, GBM_BO_FORMAT_XRGB8888, GBM_BO_USE_SCANOUT|GBM_BO_USE_RENDERING);
	egl_surface = eglCreateWindowSurface (display, config, gbm_surface, NULL);
	eglMakeCurrent (display, egl_surface, egl_surface, context);
}

static struct gbm_bo *previous_bo = NULL;
static uint32_t previous_fb;

static void swap_buffers () {
	eglSwapBuffers (display, egl_surface);
	glFlush();

	struct gbm_bo *bo = gbm_surface_lock_front_buffer (gbm_surface);
	uint32_t handle = gbm_bo_get_handle (bo).u32;
	uint32_t pitch = gbm_bo_get_stride (bo);
	uint32_t fb;
	drmModeAddFB (device, mode_info.hdisplay, mode_info.vdisplay, 24, 32, pitch, handle, &fb);
	drmModeSetCrtc (device, crtc->crtc_id, fb, 0, 0, &connector_id, 1, &mode_info);
	
	if (previous_bo) {
		drmModeRmFB (device, previous_fb);
		gbm_surface_release_buffer (gbm_surface, previous_bo);
	}
	previous_bo = bo;
	previous_fb = fb;
}


static void clean_up () {
	// set the previous crtc
	drmModeSetCrtc (device, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y, &connector_id, 1, &crtc->mode);
	drmModeFreeCrtc (crtc);
	
	if (previous_bo) {
		drmModeRmFB (device, previous_fb);
		gbm_surface_release_buffer (gbm_surface, previous_bo);
	}
	
	eglDestroySurface (display, egl_surface);
	gbm_surface_destroy (gbm_surface);
	eglDestroyContext (display, context);
	eglTerminate (display);
	gbm_device_destroy (gbm_device);
}

namespace asapi
{

	void DRM_GBM_EGL_ContextType::Init(const int argc, const char** argv)
	{

		PRIFILE( device = open ("/dev/dri/card0", O_RDWR|O_CLOEXEC); );
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;



		if(device == 0)
			log::error << "Can not open dri/card0, is Full KMS anabled on RPI?" << std::endl;


		PRIFILE( find_display_configuration (); );

		PRIFILE( setup_opengl(attributes, contextAttribs); );

		resolution.x = mode_info.hdisplay; 
	    resolution.y = mode_info.vdisplay; 

	    ResizeWindowArgs resizeWindowArgs;

    	resizeWindowArgs.m_width = mode_info.hdisplay; 
    	resizeWindowArgs.m_height = mode_info.vdisplay; 

	    p_ev_ResizeWindow->Invoke( &resizeWindowArgs );


		p_postRenderCallback = &DRM_GBM_EGL_ContextType::SwapBuffer;

		#ifdef IS_EDITOR
	    // Setup Dear ImGui context
	    IMGUI_CHECKVERSION();
	    ImGui::CreateContext();
	    ImGuiIO& io = ImGui::GetIO(); (void)io;
	    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	    //io.ConfigViewportsNoAutoMerge = true;
	    //io.ConfigViewportsNoTaskBarIcon = true;

	    // Setup Dear ImGui style
	    ImGui::StyleColorsDark();
	    //ImGui::StyleColorsClassic();

	    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	    ImGuiStyle& style = ImGui::GetStyle();
	    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	    {
	        style.WindowRounding = 0.0f;
	        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	    }

	    // Setup Platform/Renderer backends
	    ImGui_ImplDRM_InitForOpenGL(&display, &context, &egl_surface, resolution);
	    ImGui_ImplOpenGL3_Init();

		p_postRenderCallback = &DRM_GBM_EGL_ContextType::RenderGUIAndSwapBuffer;

	    #endif
	}
	
	DRM_GBM_EGL_ContextType::~DRM_GBM_EGL_ContextType()
	{
		#ifdef IS_EDITOR
	    ImGui_ImplDRM_Shutdown();
	    #endif

		clean_up ();
		close (device);
	}

	void DRM_GBM_EGL_ContextType::SwapBuffer()
	{
		swap_buffers();
	}


	void DRM_GBM_EGL_ContextType::CleanUp()
	{
		m_isRunning = false;
	}


	void DRM_GBM_EGL_ContextType::HandleContextEvents()
	{
		m_devinput.poolEvents();
	}
	void DRM_GBM_EGL_ContextType::GetResolution(uint16_t* X, uint16_t* Y)
	{
		*X = (uint16_t)resolution.x;
		*Y = (uint16_t)resolution.y;
	}

	void DRM_GBM_EGL_ContextType::RenderGUIAndSwapBuffer()
	{
     	static ImGuiIO& io = ImGui::GetIO(); (void)io;
     	static Mesh 			cursorMesh( glm::vec2(resolution.x, resolution.y) );
		static MaterialType 	cursorMaterial("debug");
		static Uniform<glm::mat4>* uCursorPos = (Uniform<glm::mat4>*)cursorMaterial.GetUniformPtr("modelViewMat");
		static SYSTEMS& system = SYSTEMS::GetObject();

		io.DeltaTime = deltaTime;

		// glViewport(0, 0, m_mainEglWindow->resolution.x, m_mainEglWindow->resolution.y);
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);
		
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplDRM_NewFrame();


		system.EDITOR.OnGUI();  

		// Rendering
        ImGui::Render();

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore current shader using the commented lines below.
        //GLint last_program;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        //glUseProgram(0);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //glUseProgram(last_program);		   

		
		cursorMaterial.BindMaterial();
		int x,y;
		m_devinput.GetCursorPos(x, y);
		glm::mat4 cursorModelView = glm::mat4(1.0);
		cursorModelView[3] = glm::vec4( x/(float)resolution.x * 2.0f - 1.0f
										,1.0f - y/(float)resolution.y * 2.0f
										, 0.0f, 1.0f);
		uCursorPos->SetUniform(cursorModelView);
		cursorMesh.Render();

		SwapBuffer();
	}

	void DRM_GBM_EGL_ContextType::MainLoop()
	{
		SYSTEMS& system = SYSTEMS::GetObject();
		bfu::EventSystem& events = system.EVENTS;
		RendererSystem& rendererSystem = system.RENDERER;
		auto frameEnd =  std::chrono::system_clock::now();
		auto frameStart = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> frameDeltaTime( m_frameDelay );


		std::chrono::duration<double> elapsed;

		while(m_isRunning)
		{
			frameStart = std::chrono::high_resolution_clock::now();

			HandleContextEvents();

			//TODO frame stuff
			{
				glViewport(0, 0, resolution.x, resolution.y);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				rendererSystem.Render();

				(this->*p_postRenderCallback)();
			}



			std::chrono::duration<double> calculationTime = std::chrono::high_resolution_clock::now() - frameStart;
			std::chrono::duration<double> diffToFrameEnd = m_frameDelay - calculationTime;

			std::this_thread::sleep_for(diffToFrameEnd);

			frameEnd = std::chrono::high_resolution_clock::now();
			frameDeltaTime = frameEnd - frameStart;
			deltaTime = (float)frameDeltaTime.count();
		}
		
	}
}