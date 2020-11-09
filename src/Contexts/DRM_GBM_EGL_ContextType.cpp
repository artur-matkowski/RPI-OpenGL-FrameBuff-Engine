#include "DRM_GBM_EGL_ContextType.hpp"



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
	printf ("resolution: %ix%i\n", mode_info.hdisplay, mode_info.vdisplay);
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

static void setup_opengl () {

	std::cout<<__FILE__<<":"<<__LINE__<<std::endl<<std::flush;

	gbm_device = gbm_create_device (device);
	std::cout<<__FILE__<<":"<<__LINE__<<std::endl<<std::flush;
	display = eglGetDisplay (gbm_device);
	std::cout<<__FILE__<<":"<<__LINE__<<std::endl<<std::flush;
	eglInitialize (display, NULL, NULL);
	
	std::cout<<__FILE__<<":"<<__LINE__<<std::endl<<std::flush;
	// create an OpenGL context
	//eglBindAPI (EGL_OPENGL_API);
	EGLint attributes[] = {
    EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,

    // Uncomment the following to enable MSAA
    // EGL_SAMPLE_BUFFERS, 1, // <-- Must be set to 1 to enable multisampling!
    // EGL_SAMPLES, 4, // <-- Number of samples

    // Uncomment the following to enable stencil buffer
    // EGL_STENCIL_SIZE, 1,

    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE};
    /*{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
	EGL_NONE};*/
	EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE};
	EGLConfig config;
	EGLint num_config;
	std::cout<<__FILE__<<":"<<__LINE__<<std::endl<<std::flush;
	eglChooseConfig (display, attributes, &config, 1, &num_config);
	eglBindAPI(EGL_OPENGL_ES_API);
	context = eglCreateContext (display, config, EGL_NO_CONTEXT, contextAttribs);
	//context = eglCreateContext (display, config, EGL_NO_CONTEXT, 0);
	
	std::cout<<__FILE__<<":"<<__LINE__<<std::endl<<std::flush;
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

namespace asapgl
{

	DRM_GBM_EGL_ContextType::DRM_GBM_EGL_ContextType(DRM_GBM_EGL_ContextType::Args &f)
	{

		device = open ("/dev/dri/card0", O_RDWR|O_CLOEXEC);



		if(device == 0)
			log::error << "Can not open dri/card0, is Full KMS anabled on RPI?" << std::endl;


		find_display_configuration ();

		setup_opengl ();
	}
	
	DRM_GBM_EGL_ContextType::~DRM_GBM_EGL_ContextType()
	{
		clean_up ();
		close (device);
	}

	void DRM_GBM_EGL_ContextType::SwapBuffer()
	{
		swap_buffers();
	}
}