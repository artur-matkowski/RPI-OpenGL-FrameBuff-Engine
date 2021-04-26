#include "GLFW_egl_Context.hpp"
#include <thread>
#include <bitforge/utils/bfu.hpp>
#include "Systems.hpp"
//#include "imgui_impl_opengl2.hpp"
#ifdef IS_EDITOR
#include "backends/imgui_impl_opengl3.h"
#endif

static void glfw_error_callback(int err, const char* description)
{
    log::error << "Glfw Error " << err << ": " << description << std::endl;
}

namespace asapi
{
	constexpr struct KeyMapsDRM
	{
		asapi::keycodes			m_keyCodeMap[ GLFW_KEY_LAST+1 ] = {asapi::keycodes::unknown};
		const int				m_mouseButtons = GLFW_MOUSE_BUTTON_LAST+1 ;
		asapi::mousecodes		m_mouseCodeMap[GLFW_MOUSE_BUTTON_LAST+1] = {asapi::mousecodes::unknown};
		char keycodes2char[255] = {0};
		
		constexpr KeyMapsDRM()
		{
		   	m_mouseCodeMap[ GLFW_MOUSE_BUTTON_LEFT 		]	= asapi::mousecodes::snapi_left;
		   	m_mouseCodeMap[ GLFW_MOUSE_BUTTON_RIGHT 	]	= asapi::mousecodes::snapi_right;
		   	m_mouseCodeMap[ GLFW_MOUSE_BUTTON_MIDDLE 	]	= asapi::mousecodes::snapi_middle;

		   	//+1 is becouse [ GLFW_KEY_UNKNOWN = -1 ]
			//m_keyCodeMap[ GLFW_KEY_RESERVED+1 ]				= asapi::keycodes::snapi_reserved;
			m_keyCodeMap[ GLFW_KEY_ESCAPE+1 ]					= asapi::keycodes::snapi_esc;
			m_keyCodeMap[ GLFW_KEY_1+1 ]						= asapi::keycodes::snapi_1;
			m_keyCodeMap[ GLFW_KEY_2+1 ]						= asapi::keycodes::snapi_2;
			m_keyCodeMap[ GLFW_KEY_3+1 ]						= asapi::keycodes::snapi_3;
			m_keyCodeMap[ GLFW_KEY_4+1 ]						= asapi::keycodes::snapi_4;
			m_keyCodeMap[ GLFW_KEY_5+1 ]						= asapi::keycodes::snapi_5;
			m_keyCodeMap[ GLFW_KEY_6+1 ]						= asapi::keycodes::snapi_6;
			m_keyCodeMap[ GLFW_KEY_7+1 ]						= asapi::keycodes::snapi_7;
			m_keyCodeMap[ GLFW_KEY_8+1 ]						= asapi::keycodes::snapi_8;
			m_keyCodeMap[ GLFW_KEY_9+1 ]						= asapi::keycodes::snapi_9;
			m_keyCodeMap[ GLFW_KEY_0+1 ]						= asapi::keycodes::snapi_0;
			m_keyCodeMap[ GLFW_KEY_MINUS+1 ]					= asapi::keycodes::snapi_minus;
			m_keyCodeMap[ GLFW_KEY_EQUAL+1 ]					= asapi::keycodes::snapi_equal;
			m_keyCodeMap[ GLFW_KEY_BACKSPACE+1 ]				= asapi::keycodes::snapi_backspace;
			m_keyCodeMap[ GLFW_KEY_TAB+1 ]						= asapi::keycodes::snapi_tab;
			m_keyCodeMap[ GLFW_KEY_Q+1 ]						= asapi::keycodes::snapi_q;
			m_keyCodeMap[ GLFW_KEY_W+1 ]						= asapi::keycodes::snapi_w;
			m_keyCodeMap[ GLFW_KEY_E+1 ]						= asapi::keycodes::snapi_e;
			m_keyCodeMap[ GLFW_KEY_R+1 ]						= asapi::keycodes::snapi_r;
			m_keyCodeMap[ GLFW_KEY_T+1 ]						= asapi::keycodes::snapi_t;
			m_keyCodeMap[ GLFW_KEY_Y+1 ]						= asapi::keycodes::snapi_y;
			m_keyCodeMap[ GLFW_KEY_U+1 ]						= asapi::keycodes::snapi_u;
			m_keyCodeMap[ GLFW_KEY_I+1 ]						= asapi::keycodes::snapi_i;
			m_keyCodeMap[ GLFW_KEY_O+1 ]						= asapi::keycodes::snapi_o;
			m_keyCodeMap[ GLFW_KEY_P+1 ]						= asapi::keycodes::snapi_p;
			m_keyCodeMap[ GLFW_KEY_LEFT_BRACKET+1 ]				= asapi::keycodes::snapi_leftbrace;
			m_keyCodeMap[ GLFW_KEY_RIGHT_BRACKET+1 ]			= asapi::keycodes::snapi_rightbrace;
			m_keyCodeMap[ GLFW_KEY_ENTER+1 ]					= asapi::keycodes::snapi_enter;
			m_keyCodeMap[ GLFW_KEY_LEFT_CONTROL+1 ]				= asapi::keycodes::snapi_leftctrl;
			m_keyCodeMap[ GLFW_KEY_A+1 ]						= asapi::keycodes::snapi_a;
			m_keyCodeMap[ GLFW_KEY_S+1 ]						= asapi::keycodes::snapi_s;
			m_keyCodeMap[ GLFW_KEY_D+1 ]						= asapi::keycodes::snapi_d;
			m_keyCodeMap[ GLFW_KEY_F+1 ]						= asapi::keycodes::snapi_f;
			m_keyCodeMap[ GLFW_KEY_G+1 ]						= asapi::keycodes::snapi_g;
			m_keyCodeMap[ GLFW_KEY_H+1 ]						= asapi::keycodes::snapi_h;
			m_keyCodeMap[ GLFW_KEY_J+1 ]						= asapi::keycodes::snapi_j;
			m_keyCodeMap[ GLFW_KEY_K+1 ]						= asapi::keycodes::snapi_k;
			m_keyCodeMap[ GLFW_KEY_L+1 ]						= asapi::keycodes::snapi_l;
			m_keyCodeMap[ GLFW_KEY_SEMICOLON+1 ]				= asapi::keycodes::snapi_semicolon;
			m_keyCodeMap[ GLFW_KEY_APOSTROPHE+1 ]				= asapi::keycodes::snapi_apostrophe;
			m_keyCodeMap[ GLFW_KEY_BACKSLASH+1 ]				= asapi::keycodes::snapi_grave;
			m_keyCodeMap[ GLFW_KEY_LEFT_SHIFT+1 ]				= asapi::keycodes::snapi_leftshift;
			m_keyCodeMap[ GLFW_KEY_BACKSLASH+1 ]				= asapi::keycodes::snapi_backslash;
			m_keyCodeMap[ GLFW_KEY_Z+1 ]						= asapi::keycodes::snapi_z;
			m_keyCodeMap[ GLFW_KEY_X+1 ]						= asapi::keycodes::snapi_x;
			m_keyCodeMap[ GLFW_KEY_C+1 ]						= asapi::keycodes::snapi_c;
			m_keyCodeMap[ GLFW_KEY_V+1 ]						= asapi::keycodes::snapi_v;
			m_keyCodeMap[ GLFW_KEY_B+1 ]						= asapi::keycodes::snapi_b;
			m_keyCodeMap[ GLFW_KEY_N+1 ]						= asapi::keycodes::snapi_n;
			m_keyCodeMap[ GLFW_KEY_M+1 ]						= asapi::keycodes::snapi_m;
			m_keyCodeMap[ GLFW_KEY_COMMA+1 ]					= asapi::keycodes::snapi_comma;
			//m_keyCodeMap[ GLFW_KEY_KP_DECIMAL+1 ]				= asapi::keycodes::snapi_dot;
			m_keyCodeMap[ GLFW_KEY_SLASH+1 ]					= asapi::keycodes::snapi_slash;
			m_keyCodeMap[ GLFW_KEY_LEFT_SHIFT+1 ]				= asapi::keycodes::snapi_rightshift;
			m_keyCodeMap[ GLFW_KEY_CAPS_LOCK+1 ]				= asapi::keycodes::snapi_kpasterisk;
			m_keyCodeMap[ GLFW_KEY_LEFT_ALT+1 ]					= asapi::keycodes::snapi_leftalt;
			m_keyCodeMap[ GLFW_KEY_SPACE+1 ]					= asapi::keycodes::snapi_space;
			m_keyCodeMap[ GLFW_KEY_CAPS_LOCK+1 ]				= asapi::keycodes::snapi_capslock;
			m_keyCodeMap[ GLFW_KEY_F1+1 ]						= asapi::keycodes::snapi_f1;
			m_keyCodeMap[ GLFW_KEY_F2+1 ]						= asapi::keycodes::snapi_f2;
			m_keyCodeMap[ GLFW_KEY_F3+1 ]						= asapi::keycodes::snapi_f3;
			m_keyCodeMap[ GLFW_KEY_F4+1 ]						= asapi::keycodes::snapi_f4;
			m_keyCodeMap[ GLFW_KEY_F5+1 ]						= asapi::keycodes::snapi_f5;
			m_keyCodeMap[ GLFW_KEY_F6+1 ]						= asapi::keycodes::snapi_f6;
			m_keyCodeMap[ GLFW_KEY_F7+1 ]						= asapi::keycodes::snapi_f7;
			m_keyCodeMap[ GLFW_KEY_F8+1 ]						= asapi::keycodes::snapi_f8;
			m_keyCodeMap[ GLFW_KEY_F9+1 ]						= asapi::keycodes::snapi_f9;
			m_keyCodeMap[ GLFW_KEY_F10+1 ]					= asapi::keycodes::snapi_f10;
			m_keyCodeMap[ GLFW_KEY_NUM_LOCK+1 ]				= asapi::keycodes::snapi_numlock;
			m_keyCodeMap[ GLFW_KEY_SCROLL_LOCK+1 ]				= asapi::keycodes::snapi_scrolllock;
			m_keyCodeMap[ GLFW_KEY_KP_7+1 ]					= asapi::keycodes::snapi_kp7;
			m_keyCodeMap[ GLFW_KEY_KP_8+1 ]					= asapi::keycodes::snapi_kp8;
			m_keyCodeMap[ GLFW_KEY_KP_9+1 ]					= asapi::keycodes::snapi_kp9;
			m_keyCodeMap[ GLFW_KEY_KP_SUBTRACT+1 ]				= asapi::keycodes::snapi_kpminus;
			m_keyCodeMap[ GLFW_KEY_KP_4+1 ]					= asapi::keycodes::snapi_kp4;
			m_keyCodeMap[ GLFW_KEY_KP_5+1 ]					= asapi::keycodes::snapi_kp5;
			m_keyCodeMap[ GLFW_KEY_KP_6+1 ]					= asapi::keycodes::snapi_kp6;
			//m_keyCodeMap[ GLFW_KEY_PLUS+1 ]					= asapi::keycodes::snapi_plus;
			m_keyCodeMap[ GLFW_KEY_KP_1+1 ]						= asapi::keycodes::snapi_kp1;
			m_keyCodeMap[ GLFW_KEY_KP_2+1 ]						= asapi::keycodes::snapi_kp2;
			m_keyCodeMap[ GLFW_KEY_KP_3+1 ]						= asapi::keycodes::snapi_kp3;
			m_keyCodeMap[ GLFW_KEY_KP_0+1 ]						= asapi::keycodes::snapi_kp0;
			m_keyCodeMap[ GLFW_KEY_PERIOD+1 ]					= asapi::keycodes::snapi_dot;
			//m_keyCodeMap[ GLFW_KEY_ZENKAKUHANKAKU+1 ]			= asapi::keycodes::snapi_zenkakuhankaku;
			//m_keyCodeMap[ GLFW_KEY_102ND+1 ]					= asapi::keycodes::snapi_102nd;
			m_keyCodeMap[ GLFW_KEY_F11+1 ]						= asapi::keycodes::snapi_f11;
			m_keyCodeMap[ GLFW_KEY_F12+1 ]						= asapi::keycodes::snapi_f12;
			//m_keyCodeMap[ GLFW_KEY_RO+1 ]						= asapi::keycodes::snapi_ro;
			//m_keyCodeMap[ GLFW_KEY_KATAKANA+1 ]				= asapi::keycodes::snapi_katakana;
			//m_keyCodeMap[ GLFW_KEY_HIRAGANA+1 ]				= asapi::keycodes::snapi_hiragana;
			//m_keyCodeMap[ GLFW_KEY_HENKAN+1 ]					= asapi::keycodes::snapi_henkan;
			//m_keyCodeMap[ GLFW_KEY_KATAKANAHIRAGANA+1 ]		= asapi::keycodes::snapi_katakanahiragana;
			//m_keyCodeMap[ GLFW_KEY_MUHENKAN+1 ]				= asapi::keycodes::snapi_muhenkan;
			//m_keyCodeMap[ GLFW_KEY_KP_JPCOMMA+1 ]				= asapi::keycodes::snapi_kpjpcomma;
			m_keyCodeMap[ GLFW_KEY_KP_ENTER+1 ]					= asapi::keycodes::snapi_kpenter;
			m_keyCodeMap[ GLFW_KEY_RIGHT_CONTROL+1 ]				= asapi::keycodes::snapi_rightctrl;
			//m_keyCodeMap[ GLFW_KEY_SLASH+1 ]					= asapi::keycodes::snapi_kpslash;
			//m_keyCodeMap[ GLFW_KEY_SYSRQ+1 ]					= asapi::keycodes::snapi_sysrq;
			m_keyCodeMap[ GLFW_KEY_RIGHT_ALT+1 ]				= asapi::keycodes::snapi_rightalt;
			//m_keyCodeMap[ GLFW_KEY_LINEFEED+1 ]				= asapi::keycodes::snapi_linefeed;
			m_keyCodeMap[ GLFW_KEY_HOME+1 ]					= asapi::keycodes::snapi_home;
			m_keyCodeMap[ GLFW_KEY_UP+1 ]						= asapi::keycodes::snapi_up;
			m_keyCodeMap[ GLFW_KEY_PAGE_UP+1 ]					= asapi::keycodes::snapi_pageup;
			m_keyCodeMap[ GLFW_KEY_LEFT+1 ]					= asapi::keycodes::snapi_left;
			m_keyCodeMap[ GLFW_KEY_RIGHT+1 ]					= asapi::keycodes::snapi_right;
			m_keyCodeMap[ GLFW_KEY_END+1 ]						= asapi::keycodes::snapi_end;
			m_keyCodeMap[ GLFW_KEY_DOWN+1 ]					= asapi::keycodes::snapi_down;
			m_keyCodeMap[ GLFW_KEY_PAGE_DOWN+1 ]				= asapi::keycodes::snapi_pagedown;
			m_keyCodeMap[ GLFW_KEY_INSERT+1 ]					= asapi::keycodes::snapi_insert;
			m_keyCodeMap[ GLFW_KEY_DELETE+1 ]					= asapi::keycodes::snapi_delete;
			//m_keyCodeMap[ GLFW_KEY_MACRO+1 ]					= asapi::keycodes::snapi_macro;
			//m_keyCodeMap[ GLFW_KEY_MUTE+1 ]					= asapi::keycodes::snapi_mute;
			//m_keyCodeMap[ GLFW_KEY_VOLUMEDOWN+1 ]				= asapi::keycodes::snapi_volumedown;
			//m_keyCodeMap[ GLFW_KEY_VOLUMEUP+1 ]				= asapi::keycodes::snapi_volumeup;
			//m_keyCodeMap[ GLFW_KEY_POWER+1 ]					= asapi::keycodes::snapi_power;
			//m_keyCodeMap[ GLFW_KEY_KP_EQUAL+1 ]					= asapi::keycodes::snapi_kpequal;
			m_keyCodeMap[ GLFW_KEY_KP_ADD+1 ]				= asapi::keycodes::snapi_kpplusminus;
			//m_keyCodeMap[ GLFW_KEY_PAUSE+1 ]					= asapi::keycodes::snapi_pause;
			//m_keyCodeMap[ GLFW_KEY_SCALE+1 ]					= asapi::keycodes::snapi_scale;
			m_keyCodeMap[ GLFW_KEY_COMMA+1 ]					= asapi::keycodes::snapi_kpcomma;
			//m_keyCodeMap[ GLFW_KEY_HANGEUL+1 ]					= asapi::keycodes::snapi_hangeul;
			//m_keyCodeMap[ GLFW_KEY_HANGUEL+1 ]					= asapi::keycodes::snapi_hanguel;
			//m_keyCodeMap[ GLFW_KEY_HANJA+1 ]					= asapi::keycodes::snapi_hanja;
			//m_keyCodeMap[ GLFW_KEY_YEN+1 ]						= asapi::keycodes::snapi_yen;
			//m_keyCodeMap[ GLFW_KEY_LEFTMETA+1 ]				= asapi::keycodes::snapi_leftmeta;
			//m_keyCodeMap[ GLFW_KEY_RIGHTMETA+1 ]				= asapi::keycodes::snapi_rightmeta;
			//m_keyCodeMap[ GLFW_KEY_COMPOSE+1 ]					= asapi::keycodes::snapi_compose;
			//m_keyCodeMap[ GLFW_KEY_STOP+1 ]					= asapi::keycodes::snapi_stop;
			//m_keyCodeMap[ GLFW_KEY_AGAIN+1 ]					= asapi::keycodes::snapi_again;
			//m_keyCodeMap[ GLFW_KEY_PROPS+1 ]					= asapi::keycodes::snapi_props;
			//m_keyCodeMap[ GLFW_KEY_UNDO+1 ]					= asapi::keycodes::snapi_undo;
			//m_keyCodeMap[ GLFW_KEY_FRONT+1 ]					= asapi::keycodes::snapi_front;
			//m_keyCodeMap[ GLFW_KEY_COPY+1 ]					= asapi::keycodes::snapi_copy;
			//m_keyCodeMap[ GLFW_KEY_OPEN+1 ]					= asapi::keycodes::snapi_open;
			//m_keyCodeMap[ GLFW_KEY_PASTE+1 ]					= asapi::keycodes::snapi_paste;
			//m_keyCodeMap[ GLFW_KEY_FIND+1 ]					= asapi::keycodes::snapi_find;
			//m_keyCodeMap[ GLFW_KEY_CUT+1 ]						= asapi::keycodes::snapi_cut;
			//m_keyCodeMap[ GLFW_KEY_HELP+1 ]					= asapi::keycodes::snapi_help;
			//m_keyCodeMap[ GLFW_KEY_MENU+1 ]					= asapi::keycodes::snapi_menu;
			//m_keyCodeMap[ GLFW_KEY_CALC+1 ]					= asapi::keycodes::snapi_calc;
			//m_keyCodeMap[ GLFW_KEY_SETUP+1 ]					= asapi::keycodes::snapi_setup;
			//m_keyCodeMap[ GLFW_KEY_SLEEP+1 ]					= asapi::keycodes::snapi_sleep;
			//m_keyCodeMap[ GLFW_KEY_WAKEUP+1 ]					= asapi::keycodes::snapi_wakeup;
			//m_keyCodeMap[ GLFW_KEY_FILE+1 ]					= asapi::keycodes::snapi_file;
			//m_keyCodeMap[ GLFW_KEY_SENDFILE+1 ]				= asapi::keycodes::snapi_sendfile;
			//m_keyCodeMap[ GLFW_KEY_DELETEFILE+1 ]				= asapi::keycodes::snapi_deletefile;
			//m_keyCodeMap[ GLFW_KEY_XFER+1 ]					= asapi::keycodes::snapi_xfer;
			//m_keyCodeMap[ GLFW_KEY_PROG1+1 ]					= asapi::keycodes::snapi_prog1;
			//m_keyCodeMap[ GLFW_KEY_PROG2+1 ]					= asapi::keycodes::snapi_prog2;
			//m_keyCodeMap[ GLFW_KEY_WWW+1 ]						= asapi::keycodes::snapi_www;
			//m_keyCodeMap[ GLFW_KEY_MSDOS+1 ]					= asapi::keycodes::snapi_msdos;
			//m_keyCodeMap[ GLFW_KEY_COFFEE+1 ]					= asapi::keycodes::snapi_coffee;
			//m_keyCodeMap[ GLFW_KEY_SCREENLOCK+1 ]				= asapi::keycodes::snapi_screenlock;
			//m_keyCodeMap[ GLFW_KEY_ROTATE_DISPLAY+1 ]			= asapi::keycodes::snapi_rotate_display;
			//m_keyCodeMap[ GLFW_KEY_DIRECTION+1 ]				= asapi::keycodes::snapi_direction;
			//m_keyCodeMap[ GLFW_KEY_CYCLEWINDOWS+1 ]			= asapi::keycodes::snapi_cyclewindows;
			//m_keyCodeMap[ GLFW_KEY_MAIL+1 ]					= asapi::keycodes::snapi_mail;
			//m_keyCodeMap[ GLFW_KEY_BOOKMARKS+1 ]				= asapi::keycodes::snapi_bookmarks;
			//m_keyCodeMap[ GLFW_KEY_COMPUTER+1 ]				= asapi::keycodes::snapi_computer;
			//m_keyCodeMap[ GLFW_KEY_BACK+1 ]					= asapi::keycodes::snapi_back;
			//m_keyCodeMap[ GLFW_KEY_FORWARD+1 ]					= asapi::keycodes::snapi_forward;
			//m_keyCodeMap[ GLFW_KEY_CLOSECD+1 ]					= asapi::keycodes::snapi_closecd;
			//m_keyCodeMap[ GLFW_KEY_EJECTCD+1 ]					= asapi::keycodes::snapi_ejectcd;
			//m_keyCodeMap[ GLFW_KEY_EJECTCLOSECD+1 ]			= asapi::keycodes::snapi_ejectclosecd;
			//m_keyCodeMap[ GLFW_KEY_NEXTSONG+1 ]				= asapi::keycodes::snapi_nextsong;
			//m_keyCodeMap[ GLFW_KEY_PLAYPAUSE+1 ]				= asapi::keycodes::snapi_playpause;
			//m_keyCodeMap[ GLFW_KEY_PREVIOUSSONG+1 ]			= asapi::keycodes::snapi_previoussong;
			//m_keyCodeMap[ GLFW_KEY_STOPCD+1 ]					= asapi::keycodes::snapi_stopcd;
			//m_keyCodeMap[ GLFW_KEY_RECORD+1 ]					= asapi::keycodes::snapi_record;
			//m_keyCodeMap[ GLFW_KEY_REWIND+1 ]					= asapi::keycodes::snapi_rewind;
			//m_keyCodeMap[ GLFW_KEY_PHONE+1 ]					= asapi::keycodes::snapi_phone;
			//m_keyCodeMap[ GLFW_KEY_ISO+1 ]						= asapi::keycodes::snapi_iso;
			//m_keyCodeMap[ GLFW_KEY_CONFIG+1 ]					= asapi::keycodes::snapi_config;
			//m_keyCodeMap[ GLFW_KEY_HOMEPAGE+1 ]				= asapi::keycodes::snapi_homepage;
			//m_keyCodeMap[ GLFW_KEY_REFRESH+1 ]					= asapi::keycodes::snapi_refresh;
			//m_keyCodeMap[ GLFW_KEY_EXIT+1 ]					= asapi::keycodes::snapi_exit;
			//m_keyCodeMap[ GLFW_KEY_MOVE+1 ]					= asapi::keycodes::snapi_move;
			//m_keyCodeMap[ GLFW_KEY_EDIT+1 ]					= asapi::keycodes::snapi_edit;
			//m_keyCodeMap[ GLFW_KEY_SCROLLUP+1 ]				= asapi::keycodes::snapi_scrollup;
			//m_keyCodeMap[ GLFW_KEY_SCROLLDOWN+1 ]				= asapi::keycodes::snapi_scrolldown;
			//m_keyCodeMap[ GLFW_KEY_KP_LEFTPAREN+1 ]				= asapi::keycodes::snapi_kpleftparen;
			//m_keyCodeMap[ GLFW_KEY_KP_RIGHTPAREN+1 ]			= asapi::keycodes::snapi_kprightparen;
			//m_keyCodeMap[ GLFW_KEY_NEW+1 ]						= asapi::keycodes::snapi_new;
			//m_keyCodeMap[ GLFW_KEY_REDO+1 ]					= asapi::keycodes::snapi_redo;
			m_keyCodeMap[ GLFW_KEY_F13+1 ]						= asapi::keycodes::snapi_f13;
			m_keyCodeMap[ GLFW_KEY_F14+1 ]						= asapi::keycodes::snapi_f14;
			m_keyCodeMap[ GLFW_KEY_F15+1 ]						= asapi::keycodes::snapi_f15;
			m_keyCodeMap[ GLFW_KEY_F16+1 ]						= asapi::keycodes::snapi_f16;
			m_keyCodeMap[ GLFW_KEY_F17+1 ]						= asapi::keycodes::snapi_f17;
			m_keyCodeMap[ GLFW_KEY_F18+1 ]						= asapi::keycodes::snapi_f18;
			m_keyCodeMap[ GLFW_KEY_F19+1 ]						= asapi::keycodes::snapi_f19;
			m_keyCodeMap[ GLFW_KEY_F20+1 ]						= asapi::keycodes::snapi_f20;
			m_keyCodeMap[ GLFW_KEY_F21+1 ]						= asapi::keycodes::snapi_f21;
			m_keyCodeMap[ GLFW_KEY_F22+1 ]						= asapi::keycodes::snapi_f22;
			m_keyCodeMap[ GLFW_KEY_F23+1 ]						= asapi::keycodes::snapi_f23;
			m_keyCodeMap[ GLFW_KEY_F24+1 ]						= asapi::keycodes::snapi_f24;
			//m_keyCodeMap[ GLFW_KEY_PLAYCD+1 ]					= asapi::keycodes::snapi_playcd;
			//m_keyCodeMap[ GLFW_KEY_PAUSECD+1 ]					= asapi::keycodes::snapi_pausecd;
			//m_keyCodeMap[ GLFW_KEY_PROG3+1 ]					= asapi::keycodes::snapi_prog3;
			//m_keyCodeMap[ GLFW_KEY_PROG4+1 ]					= asapi::keycodes::snapi_prog4;
			//m_keyCodeMap[ GLFW_KEY_DASHBOARD+1 ]				= asapi::keycodes::snapi_dashboard;
			//m_keyCodeMap[ GLFW_KEY_SUSPEND+1 ]					= asapi::keycodes::snapi_suspend;
			//m_keyCodeMap[ GLFW_KEY_CLOSE+1 ]					= asapi::keycodes::snapi_close;
			//m_keyCodeMap[ GLFW_KEY_PLAY+1 ]					= asapi::keycodes::snapi_play;
			//m_keyCodeMap[ GLFW_KEY_FASTFORWARD+1 ]				= asapi::keycodes::snapi_fastforward;
			//m_keyCodeMap[ GLFW_KEY_BASSBOOST+1 ]				= asapi::keycodes::snapi_bassboost;
			//m_keyCodeMap[ GLFW_KEY_PRINT+1 ]					= asapi::keycodes::snapi_print;
			//m_keyCodeMap[ GLFW_KEY_HP+1 ]						= asapi::keycodes::snapi_hp;
			//m_keyCodeMap[ GLFW_KEY_CAMERA+1 ]					= asapi::keycodes::snapi_camera;
			//m_keyCodeMap[ GLFW_KEY_SOUND+1 ]					= asapi::keycodes::snapi_sound;
			//m_keyCodeMap[ GLFW_KEY_QUESTION+1 ]				= asapi::keycodes::snapi_question;
			//m_keyCodeMap[ GLFW_KEY_EMAIL+1 ]					= asapi::keycodes::snapi_email;
			//m_keyCodeMap[ GLFW_KEY_CHAT+1 ]					= asapi::keycodes::snapi_chat;
			//m_keyCodeMap[ GLFW_KEY_SEARCH+1 ]					= asapi::keycodes::snapi_search;
			//m_keyCodeMap[ GLFW_KEY_CONNECT+1 ]					= asapi::keycodes::snapi_connect;
			//m_keyCodeMap[ GLFW_KEY_FINANCE+1 ]					= asapi::keycodes::snapi_finance;
			//m_keyCodeMap[ GLFW_KEY_SPORT+1 ]					= asapi::keycodes::snapi_sport;
			//m_keyCodeMap[ GLFW_KEY_SHOP+1 ]					= asapi::keycodes::snapi_shop;
			//m_keyCodeMap[ GLFW_KEY_ALTERASE+1 ]				= asapi::keycodes::snapi_alterase;
			//m_keyCodeMap[ GLFW_KEY_CANCEL+1 ]					= asapi::keycodes::snapi_cancel;
			//m_keyCodeMap[ GLFW_KEY_BRIGHTNESSDOWN+1 ]			= asapi::keycodes::snapi_brightnessdown;
			//m_keyCodeMap[ GLFW_KEY_BRIGHTNESSUP+1 ]			= asapi::keycodes::snapi_brightnessup;
			//m_keyCodeMap[ GLFW_KEY_MEDIA+1 ]					= asapi::keycodes::snapi_media;
			//m_keyCodeMap[ GLFW_KEY_SWITCHVIDEOMODE+1 ]			= asapi::keycodes::snapi_switchvideomode;
			//m_keyCodeMap[ GLFW_KEY_KBDILLUMTOGGLE+1 ]			= asapi::keycodes::snapi_kbdillumtoggle;
			//m_keyCodeMap[ GLFW_KEY_KBDILLUMDOWN+1 ]			= asapi::keycodes::snapi_kbdillumdown;
			//m_keyCodeMap[ GLFW_KEY_KBDILLUMUP+1 ]				= asapi::keycodes::snapi_kbdillumup;
			//m_keyCodeMap[ GLFW_KEY_SEND+1 ]					= asapi::keycodes::snapi_send;
			//m_keyCodeMap[ GLFW_KEY_REPLY+1 ]					= asapi::keycodes::snapi_reply;
			//m_keyCodeMap[ GLFW_KEY_FORWARDMAIL+1 ]				= asapi::keycodes::snapi_forwardmail;
			//m_keyCodeMap[ GLFW_KEY_SAVE+1 ]					= asapi::keycodes::snapi_save;
			//m_keyCodeMap[ GLFW_KEY_DOCUMENTS+1 ]				= asapi::keycodes::snapi_documents;
			//m_keyCodeMap[ GLFW_KEY_BATTERY+1 ]					= asapi::keycodes::snapi_battery;
			//m_keyCodeMap[ GLFW_KEY_BLUETOOTH+1 ]				= asapi::keycodes::snapi_bluetooth;
			//m_keyCodeMap[ GLFW_KEY_WLAN+1 ]					= asapi::keycodes::snapi_wlan;
			//m_keyCodeMap[ GLFW_KEY_UWB+1 ]						= asapi::keycodes::snapi_uwb;
			m_keyCodeMap[ GLFW_KEY_UNKNOWN+1 ]					= asapi::keycodes::snapi_unknown;
			//m_keyCodeMap[ GLFW_KEY_VIDEO_NEXT+1 ]				= asapi::keycodes::snapi_video_next;
			//m_keyCodeMap[ GLFW_KEY_VIDEO_PREV+1 ]				= asapi::keycodes::snapi_video_prev;
			//m_keyCodeMap[ GLFW_KEY_BRIGHTNESS_CYCLE+1 ]		= asapi::keycodes::snapi_brightness_cycle;
			//m_keyCodeMap[ GLFW_KEY_BRIGHTNESS_AUTO+1 ]			= asapi::keycodes::snapi_brightness_auto;
			//m_keyCodeMap[ GLFW_KEY_BRIGHTNESS_ZERO+1 ]			= asapi::keycodes::snapi_brightness_zero;
			//m_keyCodeMap[ GLFW_KEY_DISPLAY_OFF+1 ]				= asapi::keycodes::snapi_display_off;
			//m_keyCodeMap[ GLFW_KEY_WWAN+1 ]					= asapi::keycodes::snapi_wwan;
			//m_keyCodeMap[ GLFW_KEY_WIMAX+1 ]					= asapi::keycodes::snapi_wimax;
			//m_keyCodeMap[ GLFW_KEY_RFKILL+1 ]					= asapi::keycodes::snapi_rfkill;
			//m_keyCodeMap[ GLFW_KEY_MICMUTE+1 ]					= asapi::keycodes::snapi_micmute;


			keycodes2char[ (int) asapi::keycodes::snapi_esc ]					= (char)27; //escape
			keycodes2char[ (int) asapi::keycodes::snapi_1 ]						= '1';
			keycodes2char[ (int) asapi::keycodes::snapi_2 ]						= '2';
			keycodes2char[ (int) asapi::keycodes::snapi_3 ]						= '3';
			keycodes2char[ (int) asapi::keycodes::snapi_4 ]						= '4';
			keycodes2char[ (int) asapi::keycodes::snapi_5 ]						= '5';
			keycodes2char[ (int) asapi::keycodes::snapi_6 ]						= '6';
			keycodes2char[ (int) asapi::keycodes::snapi_7 ]						= '7';
			keycodes2char[ (int) asapi::keycodes::snapi_8 ]						= '8';
			keycodes2char[ (int) asapi::keycodes::snapi_9 ]						= '9';
			keycodes2char[ (int) asapi::keycodes::snapi_0 ]						= '0';
			keycodes2char[ (int) asapi::keycodes::snapi_minus ]					= '-';
			keycodes2char[ (int) asapi::keycodes::snapi_equal ]					= '=';
			keycodes2char[ (int) asapi::keycodes::snapi_backspace ]				= (char)8; //backspace
			keycodes2char[ (int) asapi::keycodes::snapi_q ]						= 'q';
			keycodes2char[ (int) asapi::keycodes::snapi_w ]						= 'w';
			keycodes2char[ (int) asapi::keycodes::snapi_e ]						= 'e';
			keycodes2char[ (int) asapi::keycodes::snapi_r ]						= 'r';
			keycodes2char[ (int) asapi::keycodes::snapi_t ]						= 't';
			keycodes2char[ (int) asapi::keycodes::snapi_y ]						= 'y';
			keycodes2char[ (int) asapi::keycodes::snapi_u ]						= 'u';
			keycodes2char[ (int) asapi::keycodes::snapi_i ]						= 'i';
			keycodes2char[ (int) asapi::keycodes::snapi_o ]						= 'o';
			keycodes2char[ (int) asapi::keycodes::snapi_p ]						= 'p';
			keycodes2char[ (int) asapi::keycodes::snapi_leftbrace ]				= '{';
			keycodes2char[ (int) asapi::keycodes::snapi_rightbrace ]			= '}';
			keycodes2char[ (int) asapi::keycodes::snapi_enter ]					= '\n';
			keycodes2char[ (int) asapi::keycodes::snapi_a ]						= 'a';
			keycodes2char[ (int) asapi::keycodes::snapi_s ]						= 's';
			keycodes2char[ (int) asapi::keycodes::snapi_d ]						= 'd';
			keycodes2char[ (int) asapi::keycodes::snapi_f ]						= 'f';
			keycodes2char[ (int) asapi::keycodes::snapi_g ]						= 'g';
			keycodes2char[ (int) asapi::keycodes::snapi_h ]						= 'h';
			keycodes2char[ (int) asapi::keycodes::snapi_j ]						= 'j';
			keycodes2char[ (int) asapi::keycodes::snapi_k ]						= 'k';
			keycodes2char[ (int) asapi::keycodes::snapi_l ]						= 'l';
			keycodes2char[ (int) asapi::keycodes::snapi_semicolon ]				= ';';
			keycodes2char[ (int) asapi::keycodes::snapi_apostrophe ]			= '\'';
			keycodes2char[ (int) asapi::keycodes::snapi_grave ]					= '`';
			keycodes2char[ (int) asapi::keycodes::snapi_backslash ]				= '\\';
			keycodes2char[ (int) asapi::keycodes::snapi_z ]						= 'z';
			keycodes2char[ (int) asapi::keycodes::snapi_x ]						= 'x';
			keycodes2char[ (int) asapi::keycodes::snapi_c ]						= 'c';
			keycodes2char[ (int) asapi::keycodes::snapi_v ]						= 'v';
			keycodes2char[ (int) asapi::keycodes::snapi_b ]						= 'b';
			keycodes2char[ (int) asapi::keycodes::snapi_n ]						= 'n';
			keycodes2char[ (int) asapi::keycodes::snapi_m ]						= 'm';
			keycodes2char[ (int) asapi::keycodes::snapi_comma ]					= ',';
			keycodes2char[ (int) asapi::keycodes::snapi_dot ]					= '.';
			keycodes2char[ (int) asapi::keycodes::snapi_slash ]					= '/';
			keycodes2char[ (int) asapi::keycodes::snapi_kpasterisk ]			= '*';
			keycodes2char[ (int) asapi::keycodes::snapi_space ]					= ' ';
			keycodes2char[ (int) asapi::keycodes::snapi_kp7 ]					= '7';
			keycodes2char[ (int) asapi::keycodes::snapi_kp8 ]					= '8';
			keycodes2char[ (int) asapi::keycodes::snapi_kp9 ]					= '9';
			keycodes2char[ (int) asapi::keycodes::snapi_kpminus ]				= '-';
			keycodes2char[ (int) asapi::keycodes::snapi_kp4 ]					= '4';
			keycodes2char[ (int) asapi::keycodes::snapi_kp5 ]					= '5';
			keycodes2char[ (int) asapi::keycodes::snapi_kp6 ]					= '6';
			keycodes2char[ (int) asapi::keycodes::snapi_kpplus ]				= '+';
			keycodes2char[ (int) asapi::keycodes::snapi_kp1 ]					= '1';
			keycodes2char[ (int) asapi::keycodes::snapi_kp2 ]					= '2';
			keycodes2char[ (int) asapi::keycodes::snapi_kp3 ]					= '3';
			keycodes2char[ (int) asapi::keycodes::snapi_kp0 ]					= '0';
			keycodes2char[ (int) asapi::keycodes::snapi_kpdot ]					= ',';

		}
	}keyMapsDRM;


	int m_mouse_posX, m_mouse_posY;

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		MouseClickEvent mouseClickEvent;
		static bfu::Event* p_ev_MouseClickEvent = SYSTEMS::GetObject().EVENTS.GetFastEvent("MouseClickEvent");

		asapi::keystates state = (action != GLFW_PRESS) ? asapi::keystates::snapi_up : asapi::keystates::snapi_down;
				        		
		mouseClickEvent.m_Xpos = (int)m_mouse_posX;
		mouseClickEvent.m_Ypos = (int)m_mouse_posY;
		mouseClickEvent.m_key = (int)asapi::mousecodes::snapi_wheelY;
		mouseClickEvent.m_state = (int)state;
	}

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		MouseClickEvent mouseClickEvent;
		static bfu::Event* p_ev_MouseClickEvent = SYSTEMS::GetObject().EVENTS.GetFastEvent("MouseClickEvent");
				        		
		mouseClickEvent.m_Xpos = (int)xoffset;
		mouseClickEvent.m_Ypos = (int)yoffset;
		mouseClickEvent.m_key = (int)asapi::mousecodes::snapi_wheelY;
		mouseClickEvent.m_state = (int)0;
	}

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KeyboardEvent keyboardEvent;
		static bfu::Event* p_ev_KeyboardEvent = SYSTEMS::GetObject().EVENTS.GetFastEvent("KeyboardEvent");

		asapi::keystates state = (action != GLFW_PRESS) ? asapi::keystates::snapi_up : asapi::keystates::snapi_down;

		keyboardEvent.m_key = (int)keyMapsDRM.m_keyCodeMap[key]; 
		keyboardEvent.m_state = (int)state; 
		keyboardEvent.m_char = (char)keyMapsDRM.keycodes2char[ keyboardEvent.m_key ];

		p_ev_KeyboardEvent->Invoke( &keyboardEvent );
		
	}
	void WindowFocusCallback(GLFWwindow* window, int focused)
	{
		#ifdef IS_EDITOR
		if(focused)
		{
			SYSTEMS::GetObject().RESOURCES.RefreshResources();
		}
		#endif
	}
	void WindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		ResizeWindowArgs args;
		args.m_width = width; 
    	args.m_height = height;

		static bfu::Event* p_ev_WindowSizeEvent = SYSTEMS::GetObject().EVENTS.GetFastEvent("ResizeWindow");
		p_ev_WindowSizeEvent->Invoke( &args );
	}


	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		MouseMoveEvent mouseMoveEvent;
		static bfu::Event* p_ev_MouseMoveEvent = SYSTEMS::GetObject().EVENTS.GetFastEvent("MouseMoveEvent");

		m_mouse_posX = mouseMoveEvent.m_Xpos = (int)xpos;
		m_mouse_posY = mouseMoveEvent.m_Ypos = (int)ypos;

		p_ev_MouseMoveEvent->Invoke( &mouseMoveEvent );
	}

	void CharCallback(GLFWwindow* window, unsigned int c)
	{
		// emtpy
	}

	void GLFW_egl_Context::Init(const int argc, const char** argv) 
	{
		#ifndef USE_XLIB
		// Setup window
		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			return;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL2 example", NULL, NULL);
		if (window == NULL)
			return;
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enable vsync

		
		glfwGetWindowSize(window, &resolution.x, &resolution.y);


		glfwSetMouseButtonCallback(window, MouseButtonCallback);
		glfwSetScrollCallback(window, ScrollCallback);
		glfwSetKeyCallback(window, KeyCallback);
		glfwSetCharCallback(window, CharCallback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetWindowFocusCallback(window, WindowFocusCallback);
		glfwSetWindowSizeCallback(window, WindowSizeCallback);


		p_postRenderCallback = &GLFW_egl_Context::SwapBuffer;

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
	    ImGui_ImplGlfw_InitForOpenGL(window, true);
	    ImGui_ImplOpenGL3_Init();


		p_postRenderCallback = &GLFW_egl_Context::RenderGUIAndSwapBuffer;

	    #endif
	    #endif
	}

	void GLFW_egl_Context::RenderGUIAndSwapBuffer()
	{
		#ifndef USE_XLIB
		#ifdef IS_EDITOR
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
        ImGui_ImplGlfw_NewFrame();

		system.EDITOR.OnGUI();

		// Rendering
        //ImGui::EndFrame();
		ImGui::Render();

		// If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
		// you may need to backup/reset/restore current shader using the commented lines below.
		//GLint last_program;
		//glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
		//glUseProgram(0);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//glUseProgram(last_program);

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
		    GLFWwindow* backup_current_context = glfwGetCurrentContext();
		    ImGui::UpdatePlatformWindows();
		    ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
		}


		// glm::vec2 mousePos(m_mainEglWindow->cursorPos.x / (float)m_mainEglWindow->resolution.x*2.0f - 1.0f
		// 	, 1.0f - m_mainEglWindow->cursorPos.y / (float)m_mainEglWindow->resolution.y*2.0f );



		// cursorMaterial.BindMaterial();
		// glm::mat4 cursorModelView = glm::mat4(1.0);
		// cursorModelView[3] = glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
		// uCursorPos->SetUniform(cursorModelView);
		// cursorMesh.Render();
		
		//auto e = glGetError();
		//log::debug << "cursorMesh.Render(); "  << mousePos.x << " " << mousePos.y << std::endl;

		glfwSwapBuffers(window);
		#endif			
		#endif			
	}
	void GLFW_egl_Context::SwapBuffer() 
	{
		#ifndef USE_XLIB
        glfwSwapBuffers(window);
		#endif			
	}
	void GLFW_egl_Context::MainLoop() 
	{
		SYSTEMS& system = SYSTEMS::GetObject();
		bfu::EventSystem& events = system.EVENTS;
		RendererSystem& rendererSystem = system.RENDERER;

		auto frameEnd =  std::chrono::system_clock::now();
		auto frameStart = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> frameDeltaTime( m_frameDelay );

		std::chrono::duration<double> elapsed;


		while(m_isRunning && !glfwWindowShouldClose(window))
		{
			frameStart = std::chrono::high_resolution_clock::now();

			glfwPollEvents();

			//TODO frame stuff
			{
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

	void GLFW_egl_Context::HandleContextEvents() 
	{
		#ifndef USE_XLIB
		glfwPollEvents();
		#endif			
	}
	void GLFW_egl_Context::CleanUp() 
	{
		m_isRunning = false;
	}
	void GLFW_egl_Context::GetResolution(uint16_t* X, uint16_t* Y) 
	{
		#ifndef USE_XLIB
		int x, y;
		glfwGetWindowSize(window, &x, &y);
		*X = x;
		*Y = y;
		#endif			
	}
}
