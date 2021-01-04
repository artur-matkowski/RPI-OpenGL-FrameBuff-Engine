#ifndef _TARGET

#include "Xlib_EGL_ContextType.hpp"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <Systems.hpp>


namespace asapgl
{
	struct Xlib_EGL_ContextType::XlibData
	{
		Display *display;
		Window root;
	};

	struct Xlib_EGL_ContextType::EGLData
	{
		EGLDisplay egl;


		EGLContext context;
		EGLSurface surface;
		unsigned int width;
		unsigned int height;


		long unsigned int x11;
	};


	bool Xlib_EGL_ContextType::DisplayOpen(void)
	{
		EGLint major, minor;

		m_XlibData->display = XOpenDisplay(NULL);
		if (!m_XlibData->display) {
			return false;
		}

		m_eglData->egl = eglGetDisplay(m_XlibData->display);
		if (m_eglData->egl == EGL_NO_DISPLAY) {
			XCloseDisplay(m_XlibData->display);
			return false;
		}

		if (!eglInitialize(m_eglData->egl, &major, &minor)) {
			XCloseDisplay(m_XlibData->display);
			return false;
		}

		log::info << "EGL: " << major << "." << minor << std::endl;

		//printf("EGL: %d.%d\n", major, minor);

		return true;
	}




bool Xlib_EGL_ContextType::window_create(const char *name,
				    unsigned int x, unsigned int y,
				    unsigned int width, unsigned int height,
				    const int* attributes, const int* contextAttribs)
{
	XSetWindowAttributes attr;
	unsigned long mask;
	XVisualInfo visual;
	EGLint num_configs;
	XVisualInfo* _INFO;
	XSizeHints hints;
	EGLConfig config;
	int num_visuals;
	EGLint version;
	int screen;
	EGLint vid;


	screen = DefaultScreen(m_XlibData->display);
	m_XlibData->root = RootWindow(m_XlibData->display, screen);

	if (!eglChooseConfig(m_eglData->egl, attributes, &config, 1, &num_configs)) {
		return false;
	}

	if (!eglGetConfigAttrib(m_eglData->egl, config, EGL_NATIVE_VISUAL_ID, &vid)) {
		return false;
	}

	visual.visualid = vid;

	_INFO = XGetVisualInfo(m_XlibData->display, VisualIDMask, &visual, &num_visuals);
	if (!_INFO) {
		return false;
	}

	memset(&attr, 0, sizeof(attr));
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = XCreateColormap(m_XlibData->display, m_XlibData->root, _INFO->visual, AllocNone);
	attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	m_eglData->x11 = XCreateWindow(m_XlibData->display, m_XlibData->root, 0, 0, width, height,
				    0, _INFO->depth, InputOutput, _INFO->visual,
				    mask, &attr);
	if (!m_eglData->x11) {
		return false;
	}

	memset(&hints, 0, sizeof(hints));
	hints.x = x;
	hints.y = y;
	hints.width = width;
	hints.height = height;
	hints.flags = USSize | USPosition;

	XSetNormalHints(m_XlibData->display, m_eglData->x11, &hints);
	XSetStandardProperties(m_XlibData->display, m_eglData->x11, name, name, None,
			       NULL, 0, &hints);

	eglBindAPI(EGL_OPENGL_ES_API);

	m_eglData->context = eglCreateContext(m_eglData->egl, config,
					   EGL_NO_CONTEXT, contextAttribs);
	if (m_eglData->context == EGL_NO_CONTEXT) {
		return false;
	}

	eglQueryContext(m_eglData->egl, m_eglData->context, EGL_CONTEXT_CLIENT_VERSION, &version);
	log::info << "OpenGL ES: " << version << std::endl;
	
	//printf("OpenGL ES: %d\n", version);

	m_eglData->surface = eglCreateWindowSurface(m_eglData->egl, config,
						 m_eglData->x11, NULL);
	if (m_eglData->surface == EGL_NO_SURFACE) {
		return false;
	}

	XFree(_INFO);

	m_eglData->width = width;
	m_eglData->height = height;



   	m_mouseCodeMap[ 1 ]							= asapgl::mousecodes::snapi_left;
   	m_mouseCodeMap[ 3 ]							= asapgl::mousecodes::snapi_right;
   	m_mouseCodeMap[ 2 ]							= asapgl::mousecodes::snapi_middle;
   	//m_mouseCodeMap[ BTN_BACK ]				= asapgl::mousecodes::snapi_back;
   	//m_mouseCodeMap[ BTN_FORWARD ]				= asapgl::mousecodes::snapi_forward;

   	m_keyCodeMap[ 0 ]							= asapgl::keycodes::snapi_reserved;
	m_keyCodeMap[ 9 ]							= asapgl::keycodes::snapi_esc;
	m_keyCodeMap[ 10 ]							= asapgl::keycodes::snapi_1;
	m_keyCodeMap[ 11 ]							= asapgl::keycodes::snapi_2;
	m_keyCodeMap[ 12 ]							= asapgl::keycodes::snapi_3;
	m_keyCodeMap[ 13 ]							= asapgl::keycodes::snapi_4;
	m_keyCodeMap[ 14 ]							= asapgl::keycodes::snapi_5;
	m_keyCodeMap[ 15 ]							= asapgl::keycodes::snapi_6;
	m_keyCodeMap[ 16 ]							= asapgl::keycodes::snapi_7;
	m_keyCodeMap[ 17 ]							= asapgl::keycodes::snapi_8;
	m_keyCodeMap[ 18 ]							= asapgl::keycodes::snapi_9;
	m_keyCodeMap[ 19 ]							= asapgl::keycodes::snapi_0;
	m_keyCodeMap[ 20 ]							= asapgl::keycodes::snapi_minus;
	m_keyCodeMap[ 21 ]							= asapgl::keycodes::snapi_equal;
	m_keyCodeMap[ 22 ]							= asapgl::keycodes::snapi_backspace;
	m_keyCodeMap[ 23 ]							= asapgl::keycodes::snapi_tab;
	m_keyCodeMap[ 24 ]							= asapgl::keycodes::snapi_q;
	m_keyCodeMap[ 25 ]							= asapgl::keycodes::snapi_w;
	m_keyCodeMap[ 26 ]							= asapgl::keycodes::snapi_e;
	m_keyCodeMap[ 27 ]							= asapgl::keycodes::snapi_r;
	m_keyCodeMap[ 28 ]							= asapgl::keycodes::snapi_t;
	m_keyCodeMap[ 29 ]							= asapgl::keycodes::snapi_y;
	m_keyCodeMap[ 30 ]							= asapgl::keycodes::snapi_u;
	m_keyCodeMap[ 31 ]							= asapgl::keycodes::snapi_i;
	m_keyCodeMap[ 32 ]							= asapgl::keycodes::snapi_o;
	m_keyCodeMap[ 33 ]							= asapgl::keycodes::snapi_p;
	m_keyCodeMap[ 34 ]							= asapgl::keycodes::snapi_leftbrace;
	m_keyCodeMap[ 35 ]							= asapgl::keycodes::snapi_rightbrace;
	m_keyCodeMap[ 36 ]							= asapgl::keycodes::snapi_enter;
	m_keyCodeMap[ 37 ]							= asapgl::keycodes::snapi_leftctrl;
	m_keyCodeMap[ 38 ]							= asapgl::keycodes::snapi_a;
	m_keyCodeMap[ 39 ]							= asapgl::keycodes::snapi_s;
	m_keyCodeMap[ 40 ]							= asapgl::keycodes::snapi_d;
	m_keyCodeMap[ 41 ]							= asapgl::keycodes::snapi_f;
	m_keyCodeMap[ 42 ]							= asapgl::keycodes::snapi_g;
	m_keyCodeMap[ 43 ]							= asapgl::keycodes::snapi_h;
	m_keyCodeMap[ 44 ]							= asapgl::keycodes::snapi_j;
	m_keyCodeMap[ 45 ]							= asapgl::keycodes::snapi_k;
	m_keyCodeMap[ 46 ]							= asapgl::keycodes::snapi_l;
	m_keyCodeMap[ 47 ]							= asapgl::keycodes::snapi_semicolon;
	m_keyCodeMap[ 48 ]							= asapgl::keycodes::snapi_apostrophe;
	m_keyCodeMap[ 49 ]							= asapgl::keycodes::snapi_grave;
	m_keyCodeMap[ 50 ]							= asapgl::keycodes::snapi_leftshift;
	m_keyCodeMap[ 51 ]							= asapgl::keycodes::snapi_backslash;
	m_keyCodeMap[ 52 ]							= asapgl::keycodes::snapi_z;
	m_keyCodeMap[ 53 ]							= asapgl::keycodes::snapi_x;
	m_keyCodeMap[ 54 ]							= asapgl::keycodes::snapi_c;
	m_keyCodeMap[ 55 ]							= asapgl::keycodes::snapi_v;
	m_keyCodeMap[ 56 ]							= asapgl::keycodes::snapi_b;
	m_keyCodeMap[ 57 ]							= asapgl::keycodes::snapi_n;
	m_keyCodeMap[ 58 ]							= asapgl::keycodes::snapi_m;
	m_keyCodeMap[ 59 ]							= asapgl::keycodes::snapi_comma;
	m_keyCodeMap[ 60 ]							= asapgl::keycodes::snapi_dot;
	m_keyCodeMap[ 61 ]							= asapgl::keycodes::snapi_slash;
	m_keyCodeMap[ 62 ]							= asapgl::keycodes::snapi_rightshift;
	m_keyCodeMap[ 63 ]							= asapgl::keycodes::snapi_kpasterisk;
	m_keyCodeMap[ 64 ]							= asapgl::keycodes::snapi_leftalt;
	m_keyCodeMap[ 65 ]							= asapgl::keycodes::snapi_space;
	m_keyCodeMap[ 66 ]							= asapgl::keycodes::snapi_capslock;
	m_keyCodeMap[ 67 ]							= asapgl::keycodes::snapi_f1;
	m_keyCodeMap[ 68 ]							= asapgl::keycodes::snapi_f2;
	m_keyCodeMap[ 69 ]							= asapgl::keycodes::snapi_f3;
	m_keyCodeMap[ 70 ]							= asapgl::keycodes::snapi_f4;
	m_keyCodeMap[ 71 ]							= asapgl::keycodes::snapi_f5;
	m_keyCodeMap[ 72 ]							= asapgl::keycodes::snapi_f6;
	m_keyCodeMap[ 73 ]							= asapgl::keycodes::snapi_f7;
	m_keyCodeMap[ 74 ]							= asapgl::keycodes::snapi_f8;
	m_keyCodeMap[ 75 ]							= asapgl::keycodes::snapi_f9;
	m_keyCodeMap[ 76 ]							= asapgl::keycodes::snapi_f10;
	m_keyCodeMap[ 77 ]							= asapgl::keycodes::snapi_numlock;
	//m_keyCodeMap[ 88 ]						= asapgl::keycodes::snapi_scrolllock;
	m_keyCodeMap[ 79 ]							= asapgl::keycodes::snapi_kp7;
	m_keyCodeMap[ 80 ]							= asapgl::keycodes::snapi_kp8;
	m_keyCodeMap[ 81 ]							= asapgl::keycodes::snapi_kp9;
	m_keyCodeMap[ 82 ]							= asapgl::keycodes::snapi_kpminus;
	m_keyCodeMap[ 83 ]							= asapgl::keycodes::snapi_kp4;
	m_keyCodeMap[ 84 ]							= asapgl::keycodes::snapi_kp5;
	m_keyCodeMap[ 85 ]							= asapgl::keycodes::snapi_kp6;
	m_keyCodeMap[ 86 ]							= asapgl::keycodes::snapi_kpplus;
	m_keyCodeMap[ 87 ]							= asapgl::keycodes::snapi_kp1;
	m_keyCodeMap[ 88 ]							= asapgl::keycodes::snapi_kp2;
	m_keyCodeMap[ 89 ]							= asapgl::keycodes::snapi_kp3;
	m_keyCodeMap[ 90 ]							= asapgl::keycodes::snapi_kp0;
	m_keyCodeMap[ 91 ]							= asapgl::keycodes::snapi_kpdot;
	//m_keyCodeMap[ 92 ]		= asapgl::keycodes::snapi_zenkakuhankaku;
	//m_keyCodeMap[ 93 ]					= asapgl::keycodes::snapi_102nd;
	//m_keyCodeMap[ 94 ]					= asapgl::keycodes::snapi_f11;
	//m_keyCodeMap[ 95 ]					= asapgl::keycodes::snapi_f12;
	//m_keyCodeMap[ 96 ]					= asapgl::keycodes::snapi_ro;
	//m_keyCodeMap[ 97 ]				= asapgl::keycodes::snapi_katakana;
	//m_keyCodeMap[ 98 ]				= asapgl::keycodes::snapi_hiragana;
	//m_keyCodeMap[ 99 ]				= asapgl::keycodes::snapi_henkan;
	//m_keyCodeMap[ 100 ]		= asapgl::keycodes::snapi_katakanahiragana;
	//m_keyCodeMap[ 101 ]				= asapgl::keycodes::snapi_muhenkan;
	//m_keyCodeMap[ 102 ]				= asapgl::keycodes::snapi_kpjpcomma;
	m_keyCodeMap[ 104 ]							= asapgl::keycodes::snapi_kpenter;
	m_keyCodeMap[ 105 ]							= asapgl::keycodes::snapi_rightctrl;
	m_keyCodeMap[ 106 ]							= asapgl::keycodes::snapi_kpslash;
	//m_keyCodeMap[ 107 ]						= asapgl::keycodes::snapi_sysrq;
	m_keyCodeMap[ 108 ]							= asapgl::keycodes::snapi_rightalt;
	//m_keyCodeMap[ 109 ]						= asapgl::keycodes::snapi_linefeed;
	m_keyCodeMap[ 110 ]							= asapgl::keycodes::snapi_home;
	m_keyCodeMap[ 111 ]							= asapgl::keycodes::snapi_up;
	m_keyCodeMap[ 112 ]							= asapgl::keycodes::snapi_pageup;
	m_keyCodeMap[ 113 ]							= asapgl::keycodes::snapi_left;
	m_keyCodeMap[ 114 ]							= asapgl::keycodes::snapi_right;
	m_keyCodeMap[ 115 ]							= asapgl::keycodes::snapi_end;
	m_keyCodeMap[ 116 ]							= asapgl::keycodes::snapi_down;
	m_keyCodeMap[ 117 ]							= asapgl::keycodes::snapi_pagedown;
	m_keyCodeMap[ 118 ]							= asapgl::keycodes::snapi_insert;
	m_keyCodeMap[ 119 ]							= asapgl::keycodes::snapi_delete;
	//m_keyCodeMap[ KEY_MACRO ]					= asapgl::keycodes::snapi_macro;
	//m_keyCodeMap[ KEY_MUTE ]					= asapgl::keycodes::snapi_mute;
	//m_keyCodeMap[ KEY_VOLUMEDOWN ]				= asapgl::keycodes::snapi_volumedown;
	//m_keyCodeMap[ KEY_VOLUMEUP ]				= asapgl::keycodes::snapi_volumeup;
	//m_keyCodeMap[ KEY_POWER ]					= asapgl::keycodes::snapi_power;
	//m_keyCodeMap[ KEY_KPEQUAL ]					= asapgl::keycodes::snapi_kpequal;
	//m_keyCodeMap[ KEY_KPPLUSMINUS ]				= asapgl::keycodes::snapi_kpplusminus;
	//m_keyCodeMap[ KEY_PAUSE ]					= asapgl::keycodes::snapi_pause;
	//m_keyCodeMap[ KEY_SCALE ]					= asapgl::keycodes::snapi_scale;
	//m_keyCodeMap[ KEY_KPCOMMA ]					= asapgl::keycodes::snapi_kpcomma;
	//m_keyCodeMap[ KEY_HANGEUL ]					= asapgl::keycodes::snapi_hangeul;
	//m_keyCodeMap[ KEY_HANGUEL ]					= asapgl::keycodes::snapi_hanguel;
	//m_keyCodeMap[ KEY_HANJA ]					= asapgl::keycodes::snapi_hanja;
	//m_keyCodeMap[ KEY_YEN ]						= asapgl::keycodes::snapi_yen;
	//m_keyCodeMap[ KEY_LEFTMETA ]				= asapgl::keycodes::snapi_leftmeta;
	//m_keyCodeMap[ KEY_RIGHTMETA ]				= asapgl::keycodes::snapi_rightmeta;
	//m_keyCodeMap[ KEY_COMPOSE ]					= asapgl::keycodes::snapi_compose;
	//m_keyCodeMap[ KEY_STOP ]					= asapgl::keycodes::snapi_stop;
	//m_keyCodeMap[ KEY_AGAIN ]					= asapgl::keycodes::snapi_again;
	//m_keyCodeMap[ KEY_PROPS ]					= asapgl::keycodes::snapi_props;
	//m_keyCodeMap[ KEY_UNDO ]					= asapgl::keycodes::snapi_undo;
	//m_keyCodeMap[ KEY_FRONT ]					= asapgl::keycodes::snapi_front;
	//m_keyCodeMap[ KEY_COPY ]					= asapgl::keycodes::snapi_copy;
	//m_keyCodeMap[ KEY_OPEN ]					= asapgl::keycodes::snapi_open;
	//m_keyCodeMap[ KEY_PASTE ]					= asapgl::keycodes::snapi_paste;
	//m_keyCodeMap[ KEY_FIND ]					= asapgl::keycodes::snapi_find;
	//m_keyCodeMap[ KEY_CUT ]						= asapgl::keycodes::snapi_cut;
	//m_keyCodeMap[ KEY_HELP ]					= asapgl::keycodes::snapi_help;
	//m_keyCodeMap[ KEY_MENU ]					= asapgl::keycodes::snapi_menu;
	//m_keyCodeMap[ KEY_CALC ]					= asapgl::keycodes::snapi_calc;
	//m_keyCodeMap[ KEY_SETUP ]					= asapgl::keycodes::snapi_setup;
	//m_keyCodeMap[ KEY_SLEEP ]					= asapgl::keycodes::snapi_sleep;
	//m_keyCodeMap[ KEY_WAKEUP ]					= asapgl::keycodes::snapi_wakeup;
	//m_keyCodeMap[ KEY_FILE ]					= asapgl::keycodes::snapi_file;
	//m_keyCodeMap[ KEY_SENDFILE ]				= asapgl::keycodes::snapi_sendfile;
	//m_keyCodeMap[ KEY_DELETEFILE ]				= asapgl::keycodes::snapi_deletefile;
	//m_keyCodeMap[ KEY_XFER ]					= asapgl::keycodes::snapi_xfer;
	//m_keyCodeMap[ KEY_PROG1 ]					= asapgl::keycodes::snapi_prog1;
	//m_keyCodeMap[ KEY_PROG2 ]					= asapgl::keycodes::snapi_prog2;
	//m_keyCodeMap[ KEY_WWW ]						= asapgl::keycodes::snapi_www;
	//m_keyCodeMap[ KEY_MSDOS ]					= asapgl::keycodes::snapi_msdos;
	//m_keyCodeMap[ KEY_COFFEE ]					= asapgl::keycodes::snapi_coffee;
	//m_keyCodeMap[ KEY_SCREENLOCK ]				= asapgl::keycodes::snapi_screenlock;
	//m_keyCodeMap[ KEY_ROTATE_DISPLAY ]			= asapgl::keycodes::snapi_rotate_display;
	//m_keyCodeMap[ KEY_DIRECTION ]				= asapgl::keycodes::snapi_direction;
	//m_keyCodeMap[ KEY_CYCLEWINDOWS ]			= asapgl::keycodes::snapi_cyclewindows;
	//m_keyCodeMap[ KEY_MAIL ]					= asapgl::keycodes::snapi_mail;
	//m_keyCodeMap[ KEY_BOOKMARKS ]				= asapgl::keycodes::snapi_bookmarks;
	//m_keyCodeMap[ KEY_COMPUTER ]				= asapgl::keycodes::snapi_computer;
	//m_keyCodeMap[ KEY_BACK ]					= asapgl::keycodes::snapi_back;
	//m_keyCodeMap[ KEY_FORWARD ]					= asapgl::keycodes::snapi_forward;
	//m_keyCodeMap[ KEY_CLOSECD ]					= asapgl::keycodes::snapi_closecd;
	//m_keyCodeMap[ KEY_EJECTCD ]					= asapgl::keycodes::snapi_ejectcd;
	//m_keyCodeMap[ KEY_EJECTCLOSECD ]			= asapgl::keycodes::snapi_ejectclosecd;
	//m_keyCodeMap[ KEY_NEXTSONG ]				= asapgl::keycodes::snapi_nextsong;
	//m_keyCodeMap[ KEY_PLAYPAUSE ]				= asapgl::keycodes::snapi_playpause;
	//m_keyCodeMap[ KEY_PREVIOUSSONG ]			= asapgl::keycodes::snapi_previoussong;
	//m_keyCodeMap[ KEY_STOPCD ]					= asapgl::keycodes::snapi_stopcd;
	//m_keyCodeMap[ KEY_RECORD ]					= asapgl::keycodes::snapi_record;
	//m_keyCodeMap[ KEY_REWIND ]					= asapgl::keycodes::snapi_rewind;
	//m_keyCodeMap[ KEY_PHONE ]					= asapgl::keycodes::snapi_phone;
	//m_keyCodeMap[ KEY_ISO ]						= asapgl::keycodes::snapi_iso;
	//m_keyCodeMap[ KEY_CONFIG ]					= asapgl::keycodes::snapi_config;
	//m_keyCodeMap[ KEY_HOMEPAGE ]				= asapgl::keycodes::snapi_homepage;
	//m_keyCodeMap[ KEY_REFRESH ]					= asapgl::keycodes::snapi_refresh;
	//m_keyCodeMap[ KEY_EXIT ]					= asapgl::keycodes::snapi_exit;
	//m_keyCodeMap[ KEY_MOVE ]					= asapgl::keycodes::snapi_move;
	//m_keyCodeMap[ KEY_EDIT ]					= asapgl::keycodes::snapi_edit;
	//m_keyCodeMap[ KEY_SCROLLUP ]				= asapgl::keycodes::snapi_scrollup;
	//m_keyCodeMap[ KEY_SCROLLDOWN ]				= asapgl::keycodes::snapi_scrolldown;
	//m_keyCodeMap[ KEY_KPLEFTPAREN ]				= asapgl::keycodes::snapi_kpleftparen;
	//m_keyCodeMap[ KEY_KPRIGHTPAREN ]			= asapgl::keycodes::snapi_kprightparen;
	//m_keyCodeMap[ KEY_NEW ]						= asapgl::keycodes::snapi_new;
	//m_keyCodeMap[ KEY_REDO ]					= asapgl::keycodes::snapi_redo;
	//m_keyCodeMap[ KEY_F13 ]						= asapgl::keycodes::snapi_f13;
	//m_keyCodeMap[ KEY_F14 ]						= asapgl::keycodes::snapi_f14;
	//m_keyCodeMap[ KEY_F15 ]						= asapgl::keycodes::snapi_f15;
	//m_keyCodeMap[ KEY_F16 ]						= asapgl::keycodes::snapi_f16;
	//m_keyCodeMap[ KEY_F17 ]						= asapgl::keycodes::snapi_f17;
	//m_keyCodeMap[ KEY_F18 ]						= asapgl::keycodes::snapi_f18;
	//m_keyCodeMap[ KEY_F19 ]						= asapgl::keycodes::snapi_f19;
	//m_keyCodeMap[ KEY_F20 ]						= asapgl::keycodes::snapi_f20;
	//m_keyCodeMap[ KEY_F21 ]						= asapgl::keycodes::snapi_f21;
	//m_keyCodeMap[ KEY_F22 ]						= asapgl::keycodes::snapi_f22;
	//m_keyCodeMap[ KEY_F23 ]						= asapgl::keycodes::snapi_f23;
	//m_keyCodeMap[ KEY_F24 ]						= asapgl::keycodes::snapi_f24;
	//m_keyCodeMap[ KEY_PLAYCD ]					= asapgl::keycodes::snapi_playcd;
	//m_keyCodeMap[ KEY_PAUSECD ]					= asapgl::keycodes::snapi_pausecd;
	//m_keyCodeMap[ KEY_PROG3 ]					= asapgl::keycodes::snapi_prog3;
	//m_keyCodeMap[ KEY_PROG4 ]					= asapgl::keycodes::snapi_prog4;
	//m_keyCodeMap[ KEY_DASHBOARD ]				= asapgl::keycodes::snapi_dashboard;
	//m_keyCodeMap[ KEY_SUSPEND ]					= asapgl::keycodes::snapi_suspend;
	//m_keyCodeMap[ KEY_CLOSE ]					= asapgl::keycodes::snapi_close;
	//m_keyCodeMap[ KEY_PLAY ]					= asapgl::keycodes::snapi_play;
	//m_keyCodeMap[ KEY_FASTFORWARD ]				= asapgl::keycodes::snapi_fastforward;
	//m_keyCodeMap[ KEY_BASSBOOST ]				= asapgl::keycodes::snapi_bassboost;
	//m_keyCodeMap[ KEY_PRINT ]					= asapgl::keycodes::snapi_print;
	//m_keyCodeMap[ KEY_HP ]						= asapgl::keycodes::snapi_hp;
	//m_keyCodeMap[ KEY_CAMERA ]					= asapgl::keycodes::snapi_camera;
	//m_keyCodeMap[ KEY_SOUND ]					= asapgl::keycodes::snapi_sound;
	//m_keyCodeMap[ KEY_QUESTION ]				= asapgl::keycodes::snapi_question;
	//m_keyCodeMap[ KEY_EMAIL ]					= asapgl::keycodes::snapi_email;
	//m_keyCodeMap[ KEY_CHAT ]					= asapgl::keycodes::snapi_chat;
	//m_keyCodeMap[ KEY_SEARCH ]					= asapgl::keycodes::snapi_search;
	//m_keyCodeMap[ KEY_CONNECT ]					= asapgl::keycodes::snapi_connect;
	//m_keyCodeMap[ KEY_FINANCE ]					= asapgl::keycodes::snapi_finance;
	//m_keyCodeMap[ KEY_SPORT ]					= asapgl::keycodes::snapi_sport;
	//m_keyCodeMap[ KEY_SHOP ]					= asapgl::keycodes::snapi_shop;
	//m_keyCodeMap[ KEY_ALTERASE ]				= asapgl::keycodes::snapi_alterase;
	//m_keyCodeMap[ KEY_CANCEL ]					= asapgl::keycodes::snapi_cancel;
	//m_keyCodeMap[ KEY_BRIGHTNESSDOWN ]			= asapgl::keycodes::snapi_brightnessdown;
	//m_keyCodeMap[ KEY_BRIGHTNESSUP ]			= asapgl::keycodes::snapi_brightnessup;
	//m_keyCodeMap[ KEY_MEDIA ]					= asapgl::keycodes::snapi_media;
	//m_keyCodeMap[ KEY_SWITCHVIDEOMODE ]			= asapgl::keycodes::snapi_switchvideomode;
	//m_keyCodeMap[ KEY_KBDILLUMTOGGLE ]			= asapgl::keycodes::snapi_kbdillumtoggle;
	//m_keyCodeMap[ KEY_KBDILLUMDOWN ]			= asapgl::keycodes::snapi_kbdillumdown;
	//m_keyCodeMap[ KEY_KBDILLUMUP ]				= asapgl::keycodes::snapi_kbdillumup;
	//m_keyCodeMap[ KEY_SEND ]					= asapgl::keycodes::snapi_send;
	//m_keyCodeMap[ KEY_REPLY ]					= asapgl::keycodes::snapi_reply;
	//m_keyCodeMap[ KEY_FORWARDMAIL ]				= asapgl::keycodes::snapi_forwardmail;
	//m_keyCodeMap[ KEY_SAVE ]					= asapgl::keycodes::snapi_save;
	//m_keyCodeMap[ KEY_DOCUMENTS ]				= asapgl::keycodes::snapi_documents;
	//m_keyCodeMap[ KEY_BATTERY ]					= asapgl::keycodes::snapi_battery;
	//m_keyCodeMap[ KEY_BLUETOOTH ]				= asapgl::keycodes::snapi_bluetooth;
	//m_keyCodeMap[ KEY_WLAN ]					= asapgl::keycodes::snapi_wlan;
	//m_keyCodeMap[ KEY_UWB ]						= asapgl::keycodes::snapi_uwb;
	//m_keyCodeMap[ KEY_UNKNOWN ]					= asapgl::keycodes::snapi_unknown;
	//m_keyCodeMap[ KEY_VIDEO_NEXT ]				= asapgl::keycodes::snapi_video_next;
	//m_keyCodeMap[ KEY_VIDEO_PREV ]				= asapgl::keycodes::snapi_video_prev;
	//m_keyCodeMap[ KEY_BRIGHTNESS_CYCLE ]		= asapgl::keycodes::snapi_brightness_cycle;
	//m_keyCodeMap[ KEY_BRIGHTNESS_AUTO ]			= asapgl::keycodes::snapi_brightness_auto;
	//m_keyCodeMap[ KEY_BRIGHTNESS_ZERO ]			= asapgl::keycodes::snapi_brightness_zero;
	//m_keyCodeMap[ KEY_DISPLAY_OFF ]				= asapgl::keycodes::snapi_display_off;
	//m_keyCodeMap[ KEY_WWAN ]					= asapgl::keycodes::snapi_wwan;
	//m_keyCodeMap[ KEY_WIMAX ]					= asapgl::keycodes::snapi_wimax;
	//m_keyCodeMap[ KEY_RFKILL ]					= asapgl::keycodes::snapi_rfkill;
	//m_keyCodeMap[ KEY_MICMUTE ]					= asapgl::keycodes::snapi_micmute;



	return true;
}


void Xlib_EGL_ContextType::window_show()
{
    /* select kind of events we are interested in */
    XSelectInput(m_XlibData->display, m_eglData->x11, ExposureMask
			| StructureNotifyMask 
			| SubstructureNotifyMask 
			| KeyPressMask
			| KeyReleaseMask
			//| PointerMotionHintMask
			| ButtonPressMask
			| ButtonReleaseMask
			| PointerMotionMask
			| EnterWindowMask
			| FocusChangeMask);


	XMapWindow(m_XlibData->display, m_eglData->x11);

	if (!eglMakeCurrent(m_eglData->egl, m_eglData->surface, m_eglData->surface, m_eglData->context))
		fprintf(stderr, "eglMakeCurrent():\n");

	XFlush(m_XlibData->display);

}


	Xlib_EGL_ContextType::Xlib_EGL_ContextType(const int* attributes, const int* contextAttribs, const int argc, const char** argv)
		:m_XlibData(new struct XlibData)
		,m_eglData(new struct EGLData)
	{
		const unsigned int width = 1024;
		const unsigned int height = 600;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

		DisplayOpen();
		/*
		if (!display) {
			fprintf(stderr, "failed to open display\n");
			return;
		}*/

		window_create( "argv[0]", 0, 0, width, height, attributes, contextAttribs);
		/*if (!window) {
			fprintf(stderr, "failed to create window\n");
			return;
		}*/

		window_show();

		events.Invoke<ResizeWindowArgs>([&](ResizeWindowArgs& args) 
	    {
	    	args.m_width = width; 
	    	args.m_height = height; 
	    });


	}
	
	Xlib_EGL_ContextType::~Xlib_EGL_ContextType()
	{
		XAutoRepeatOn(m_XlibData->display);

		eglDestroySurface(m_eglData->egl, m_eglData->surface);
		eglDestroyContext(m_eglData->egl, m_eglData->context);
		XDestroyWindow(m_XlibData->display, m_eglData->x11);


		eglTerminate(m_eglData->egl);
		XCloseDisplay(m_XlibData->display);
	}

	void Xlib_EGL_ContextType::SwapBuffer()
	{
		eglSwapBuffers(m_eglData->egl, m_eglData->surface);
		glFlush();
	}

	void Xlib_EGL_ContextType::HandleContextEvents()
	{
		XEvent event;
		static bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;




		while( XPending(m_XlibData->display) )
		{
			XNextEvent(m_XlibData->display, &event);
			int key;


			switch (event.type) 
			{
			case Expose:
				//redraw = true;
				break;

			case DestroyNotify:
				XAutoRepeatOn(m_XlibData->display);
				log::debug << "-------------------Window killed!" << std::endl;
				break;

			case MotionNotify:
				//log::debug << "MotionNotify: " << event.xmotion.x << " " << event.xmotion.y << std::endl;
				events.Invoke<MouseMoveEvent>([&](MouseMoveEvent& args) 
			    {
					args.m_Xpos = (int)event.xmotion.x;
					args.m_Ypos = (int)event.xmotion.y;
			    });

				break;	

			case ButtonPress:
				events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
			    {
					args.m_Xpos = (int)event.xbutton.x;
					args.m_Ypos = (int)event.xbutton.y;
					args.m_key = (int)m_mouseCodeMap[event.xbutton.button];
					args.m_state = (int)asapgl::keystates::snapi_down;
			    });
				break;

			case ButtonRelease:
			/*
				log::debug << "Button: " << event.xbutton.x << " " << event.xbutton.y 
					<< " " << event.xbutton.state << " " << event.xbutton.button << std::endl;*/
				events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
			    {
					args.m_Xpos = (int)event.xbutton.x;
					args.m_Ypos = (int)event.xbutton.y;
					args.m_key = (int)m_mouseCodeMap[event.xbutton.button];
					args.m_state = (int)asapgl::keystates::snapi_up;
			    });
				break;

			case ConfigureNotify:
			    events.Invoke<ResizeWindowArgs>([&](ResizeWindowArgs& args) 
			    {
			    	args.m_width = (GLint)event.xconfigure.width; 
			    	args.m_height = (GLint)event.xconfigure.height; 
			    });
				break;

			case KeyPress:
				log::debug << "inputed key keycode: " << event.xkey.keycode << std::endl;
				key = (int)asapgl::keycodes::unknown;

				if(event.xkey.keycode < (int)asapgl::keycodes::unknown){
					key = (int)m_keyCodeMap[(int)event.xkey.keycode];
				}

				events.Invoke<KeyboardEvent>([&](KeyboardEvent& args) 
			    {
			    	args.m_key = (int)key; 
			    	args.m_state = (int)asapgl::keystates::snapi_down; 
			    });
				break;

			case KeyRelease:
				key = (int)asapgl::keycodes::unknown;

				if(event.xkey.keycode < (int)asapgl::keycodes::unknown){
					key = (int)m_keyCodeMap[(int)event.xkey.keycode];
				}

				events.Invoke<KeyboardEvent>([&](KeyboardEvent& args) 
			    {
			    	args.m_key = (int)key; 
			    	args.m_state = (int)asapgl::keystates::snapi_up; 
			    });
			    break;
			
			case FocusIn:
				XAutoRepeatOff(m_XlibData->display);
			    break;

			case FocusOut:
				XAutoRepeatOn(m_XlibData->display);
			    break;

			default:
				break;
			}
		
		}
		
	}
}

#endif