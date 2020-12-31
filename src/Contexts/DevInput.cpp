#include "DevInput.hpp"
#include <stdio.h>
#include <fstream>
#include <fcntl.h>
#include <linux/input.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <bitforge/utils/bfu.hpp>

namespace snapi
{

	void devinput::poolEvents()
	{
		#define toPrint(x) #x": %d\n", x
		struct input_event *evp;
		bool mouseMoved = false;
		struct input_event ev[64];
		int result;

		for(int i=0; i<m_kbdFdsSize; ++i)
		{
		    m_kbdPollStruct[i].revents = 0;
		}

		for(int i=0; i<m_mouseFdsSize; ++i)
		{
		    m_mousePollStruct[i].revents = 0;
		}



		result = poll(m_kbdPollStruct, m_kbdFdsSize, 0);
		if(result>0)
		{
			//printf("keyboard event %d\n", result);
			for(int i=0; i<m_kbdFdsSize; ++i)
			{
			    if(m_kbdPollStruct[i].revents != 0)
			    {
			    	//TODO
			    	int readed = read(m_kbdPollStruct[i].fd, ev, sizeof(ev) );
			    	int readedEvents = readed/sizeof(struct input_event);

			    	for(int i = 0; i<readedEvents; ++i)
			    	{
			    		evp = &ev[i];
			    		if( evp->type == EV_KEY )
			    		{
				    		//printf(toPrint(EV_KEY));
				    		//printf("EVENT: type:%d code:%d value:%d\n", evp->type, evp->code, evp->value);

				    		snapi::keystates state = (evp->value == 1) ? snapi::keystates::snapi_up : snapi::keystates::snapi_down;
			        		if(evp->code < 255)
			        			m_keyboardCallback(m_keyCodeMap[evp->code], state);
			        		else
			        			m_keyboardCallback(snapi::keycodes::unknown, state);
			    		}
			    	}
			    }
			}
		}


		result = poll(m_mousePollStruct, m_mouseFdsSize, 0);
		if(result>0)
		{
			//printf("mouse event %d\n", result);
			for(int i=0; i<m_mouseFdsSize; ++i)
			{
				if(m_mousePollStruct[i].revents != 0)
			    {
			    	int readed = read(m_mousePollStruct[i].fd, ev, sizeof(ev) );
			    	int readedEvents = readed/sizeof(struct input_event);

			    	for(int i = 0; i<readedEvents; ++i)
			    	{
			    		evp = &ev[i];

			    		//printf("EVENT: type:%d code:%d value:%d\n", evp->type, evp->code, evp->value);

			    		
			    		if(evp->type == EV_REL) //mouse movement
				        {
				        	//printf(toPrint(EV_REL));
				        	
					        if(evp->code == REL_X) // Mouse Left/Right
					        {
				        		mouseMoved = true;
					            m_mouse_posX += evp->value;
					            //printf("Mouse moved left/right %d\n",evp->value);
					        }
					        
					        else if(evp->code == REL_Y) // Mouse Up/Down
					        {
				        		mouseMoved = true;
					            m_mouse_posY += evp->value;
					            //printf("Mouse moved up/down %d\n",evp->value);
					        }
				        	else if(evp->code == REL_WHEEL) //touchscreen coords Y
				        	{
				    			snapi::keystates state = (evp->value == 1) ? snapi::keystates::snapi_up : snapi::keystates::snapi_down;
				        		m_mouseClickCallback(m_mouse_posX, m_mouse_posY, snapi::mousecodes::snapi_wheelY, state);
				        	}
				        }
				        else if(evp->type == EV_ABS)
				        {
				        	//printf(toPrint(EV_ABS));
				        	if(evp->code == ABS_X) //touchscreen coords X
				        	{
				        		mouseMoved = true;
				        		m_mouse_posX = evp->value;
				        		//printf("EVENT: type:%d code:%d value:%d\n", evp->type, evp->code, evp->value);
				        	}
				        	else if(evp->code == ABS_Y) //touchscreen coords Y
				        	{
				        		mouseMoved = true;
				        		m_mouse_posY = evp->value;
				        		//printf("EVENT: type:%d code:%d value:%d\n", evp->type, evp->code, evp->value);
				        	}
				        }
				        else if(evp->type == EV_KEY)
				        {
				        	if(evp->code == 330) // touchscreen click
				        	{
				    			snapi::keystates state = (evp->value == 1) ? snapi::keystates::snapi_up : snapi::keystates::snapi_down;
				        		m_mouseClickCallback(m_mouse_posX, m_mouse_posY, snapi::mousecodes::snapi_left, state);
				        	}
				        	else 
				        	{
				        		auto it = m_mouseCodeMap.find( evp->code );
				    			snapi::keystates state = (evp->value == 1) ? snapi::keystates::snapi_up : snapi::keystates::snapi_down;

				        		if(it == m_mouseCodeMap.end() )
				        		{
				        			m_mouseClickCallback(m_mouse_posX, m_mouse_posY, snapi::mousecodes::unknown, state);
				        		}
				        		else
				        		{
				        			m_mouseClickCallback(m_mouse_posX, m_mouse_posY, it->second, state);
				        		}

				        		//m_mouseClickCallback(m_mouse_posX, m_mouse_posY, evp->code, evp->value);
				        	}
				        	//printf(toPrint(EV_KEY));
				        	//printf("EVENT: type:%d code:%d value:%d\n", evp->type, evp->code, evp->value);
				        }
			    	}
			    }
			}
		}

		if(mouseMoved)
		{
			if(m_mouse_posX < 0)
				m_mouse_posX = 0;
			else if(m_mouse_posX > m_mouseXmax)
				m_mouse_posX = m_mouseXmax;

			if(m_mouse_posY < 0)
				m_mouse_posY = 0;
			else if(m_mouse_posY > m_mouseYmax)
				m_mouse_posY = m_mouseYmax;

			m_mouseMoveCallback(m_mouse_posX, m_mouse_posY);
		}
		//printf("mouse x:%d\t y:%d\n", m_mouse_posX, m_mouse_posY);

	}

	void devinput::SetMouseMoveCallback(void (*mouseMoveCallback)(int posX, int posY) )
	{
		m_mouseMoveCallback = mouseMoveCallback;
	}
	void devinput::SetMuseClickCallback(void (*mouseClickCallback)(int posX, int posY, snapi::mousecodes key, snapi::keystates state) )
	{
		m_mouseClickCallback = mouseClickCallback;
	}
	void devinput::SetKeyboardCallback(void (*keyboardCallback)(snapi::keycodes key, snapi::keystates state) )
	{
		m_keyboardCallback = keyboardCallback;
	}

	void devinput::openDevice(const char* path, std::vector<int>& devices)
	{
		int fd = -1;
		int result;

	    //printf("Opening input device: %s\n", path);

	    fd = open(path, O_RDONLY);
	    result = ioctl(fd, EVIOCGRAB, 1);
	    //printf("Getting exclusive access: %s\n", (result == 0) ? "SUCCESS" : "FAILURE");

	    if(fd<0)
	    {
	    	log::error << "Opening input event: " << path << std::endl;
	    	log::error << "Exiting now, otherwise keyboard interrupts might not work " << std::endl;
	    	exit(0);
	    }
	    else
	    {
	    	devices.push_back(fd);
	    }
	}

	void devinput::closeDevice()
	{
		for(auto it = m_keyboards.begin(); it!=m_keyboards.end(); ++it)
		{
			close(*it);
		}
		m_keyboards.clear();
		
		if( m_kbdPollStruct != 0 ){
			delete m_kbdPollStruct;
			m_kbdPollStruct = 0;
		}


		for(auto it = m_mouses.begin(); it!=m_mouses.end(); ++it)
		{
			close(*it);
		}
		m_mouses.clear();

		if( m_mousePollStruct != 0 ){
			delete m_mousePollStruct;
			m_mousePollStruct = 0;
		}
	}

	void devinput::scanForDevices()
	{
		closeDevice();
		//openDevice("/dev/input/event0", m_keyboards);

		std::ifstream infile("/proc/bus/input/devices");
		char buff[512];

		std::string line;
		while (std::getline(infile, line))
		{
			std::size_t found = line.find("Handlers");
			if ( found == std::string::npos )
			{
				continue;
			}

			found = line.find("kbd");
			if( found != std::string::npos )
			{
				found = line.find("event");
				int result = sprintf(buff, "/dev/input/%s", line.c_str()+found);
				buff[result-1] = 0;

				openDevice(buff, m_keyboards);
				continue;
			}

			found = line.find("mouse");
			if( found != std::string::npos )
			{
				found = line.find("event");
				int result = sprintf(buff, "/dev/input/%s", line.c_str()+found);
				buff[result-1] = 0;

				openDevice(buff, m_mouses);
				continue;
			}
		}

		m_kbdFdsSize = m_keyboards.size();
		m_kbdPollStruct = new struct pollfd[ m_kbdFdsSize ];
		for(int i=0; i<m_kbdFdsSize; ++i)
		{
		    m_kbdPollStruct[i].fd = m_keyboards[i];
		    m_kbdPollStruct[i].events = POLLIN;
		    m_kbdPollStruct[i].revents = 0;
		}

		m_mouseFdsSize = m_mouses.size();
		m_mousePollStruct = new struct pollfd[ m_mouseFdsSize ];
		for(int i=0; i<m_mouseFdsSize; ++i)
		{
		    m_mousePollStruct[i].fd = m_mouses[i];
		    m_mousePollStruct[i].events = POLLIN;
		    m_mousePollStruct[i].revents = 0;
		}
	}


	void devinput::SetDisplaySize(int mouseXmax, int mouseYmax)
	{
		m_mouseXmax = mouseXmax;
		m_mouseYmax = mouseYmax;
	}

	devinput::devinput()
	{
	   	scanForDevices();

	   	m_mouseCodeMap[ BTN_LEFT ]					= snapi::mousecodes::snapi_left;
	   	m_mouseCodeMap[ BTN_RIGHT ]					= snapi::mousecodes::snapi_right;
	   	m_mouseCodeMap[ BTN_MIDDLE ]				= snapi::mousecodes::snapi_middle;
	   	m_mouseCodeMap[ BTN_BACK ]					= snapi::mousecodes::snapi_back;
	   	m_mouseCodeMap[ BTN_FORWARD ]				= snapi::mousecodes::snapi_forward;

	   	m_keyCodeMap[ KEY_RESERVED ]				= snapi::keycodes::snapi_reserved;
		m_keyCodeMap[ KEY_ESC ]						= snapi::keycodes::snapi_esc;
		m_keyCodeMap[ KEY_1 ]						= snapi::keycodes::snapi_1;
		m_keyCodeMap[ KEY_2 ]						= snapi::keycodes::snapi_2;
		m_keyCodeMap[ KEY_3 ]						= snapi::keycodes::snapi_3;
		m_keyCodeMap[ KEY_4 ]						= snapi::keycodes::snapi_4;
		m_keyCodeMap[ KEY_5 ]						= snapi::keycodes::snapi_5;
		m_keyCodeMap[ KEY_6 ]						= snapi::keycodes::snapi_6;
		m_keyCodeMap[ KEY_7 ]						= snapi::keycodes::snapi_7;
		m_keyCodeMap[ KEY_8 ]						= snapi::keycodes::snapi_8;
		m_keyCodeMap[ KEY_9 ]						= snapi::keycodes::snapi_9;
		m_keyCodeMap[ KEY_0 ]						= snapi::keycodes::snapi_0;
		m_keyCodeMap[ KEY_MINUS ]					= snapi::keycodes::snapi_minus;
		m_keyCodeMap[ KEY_EQUAL ]					= snapi::keycodes::snapi_equal;
		m_keyCodeMap[ KEY_BACKSPACE ]				= snapi::keycodes::snapi_backspace;
		m_keyCodeMap[ KEY_TAB ]						= snapi::keycodes::snapi_tab;
		m_keyCodeMap[ KEY_Q ]						= snapi::keycodes::snapi_q;
		m_keyCodeMap[ KEY_W ]						= snapi::keycodes::snapi_w;
		m_keyCodeMap[ KEY_E ]						= snapi::keycodes::snapi_e;
		m_keyCodeMap[ KEY_R ]						= snapi::keycodes::snapi_r;
		m_keyCodeMap[ KEY_T ]						= snapi::keycodes::snapi_t;
		m_keyCodeMap[ KEY_Y ]						= snapi::keycodes::snapi_y;
		m_keyCodeMap[ KEY_U ]						= snapi::keycodes::snapi_u;
		m_keyCodeMap[ KEY_I ]						= snapi::keycodes::snapi_i;
		m_keyCodeMap[ KEY_O ]						= snapi::keycodes::snapi_o;
		m_keyCodeMap[ KEY_P ]						= snapi::keycodes::snapi_p;
		m_keyCodeMap[ KEY_LEFTBRACE ]				= snapi::keycodes::snapi_leftbrace;
		m_keyCodeMap[ KEY_RIGHTBRACE ]				= snapi::keycodes::snapi_rightbrace;
		m_keyCodeMap[ KEY_ENTER ]					= snapi::keycodes::snapi_enter;
		m_keyCodeMap[ KEY_LEFTCTRL ]				= snapi::keycodes::snapi_leftctrl;
		m_keyCodeMap[ KEY_A ]						= snapi::keycodes::snapi_a;
		m_keyCodeMap[ KEY_S ]						= snapi::keycodes::snapi_s;
		m_keyCodeMap[ KEY_D ]						= snapi::keycodes::snapi_d;
		m_keyCodeMap[ KEY_F ]						= snapi::keycodes::snapi_f;
		m_keyCodeMap[ KEY_G ]						= snapi::keycodes::snapi_g;
		m_keyCodeMap[ KEY_H ]						= snapi::keycodes::snapi_h;
		m_keyCodeMap[ KEY_J ]						= snapi::keycodes::snapi_j;
		m_keyCodeMap[ KEY_K ]						= snapi::keycodes::snapi_k;
		m_keyCodeMap[ KEY_L ]						= snapi::keycodes::snapi_l;
		m_keyCodeMap[ KEY_SEMICOLON ]				= snapi::keycodes::snapi_semicolon;
		m_keyCodeMap[ KEY_APOSTROPHE ]				= snapi::keycodes::snapi_apostrophe;
		m_keyCodeMap[ KEY_GRAVE ]					= snapi::keycodes::snapi_grave;
		m_keyCodeMap[ KEY_LEFTSHIFT ]				= snapi::keycodes::snapi_leftshift;
		m_keyCodeMap[ KEY_BACKSLASH ]				= snapi::keycodes::snapi_backslash;
		m_keyCodeMap[ KEY_Z ]						= snapi::keycodes::snapi_z;
		m_keyCodeMap[ KEY_X ]						= snapi::keycodes::snapi_x;
		m_keyCodeMap[ KEY_C ]						= snapi::keycodes::snapi_c;
		m_keyCodeMap[ KEY_V ]						= snapi::keycodes::snapi_v;
		m_keyCodeMap[ KEY_B ]						= snapi::keycodes::snapi_b;
		m_keyCodeMap[ KEY_N ]						= snapi::keycodes::snapi_n;
		m_keyCodeMap[ KEY_M ]						= snapi::keycodes::snapi_m;
		m_keyCodeMap[ KEY_COMMA ]					= snapi::keycodes::snapi_comma;
		m_keyCodeMap[ KEY_DOT ]						= snapi::keycodes::snapi_dot;
		m_keyCodeMap[ KEY_SLASH ]					= snapi::keycodes::snapi_slash;
		m_keyCodeMap[ KEY_RIGHTSHIFT ]				= snapi::keycodes::snapi_rightshift;
		m_keyCodeMap[ KEY_KPASTERISK ]				= snapi::keycodes::snapi_kpasterisk;
		m_keyCodeMap[ KEY_LEFTALT ]					= snapi::keycodes::snapi_leftalt;
		m_keyCodeMap[ KEY_SPACE ]					= snapi::keycodes::snapi_space;
		m_keyCodeMap[ KEY_CAPSLOCK ]				= snapi::keycodes::snapi_capslock;
		m_keyCodeMap[ KEY_F1 ]						= snapi::keycodes::snapi_f1;
		m_keyCodeMap[ KEY_F2 ]						= snapi::keycodes::snapi_f2;
		m_keyCodeMap[ KEY_F3 ]						= snapi::keycodes::snapi_f3;
		m_keyCodeMap[ KEY_F4 ]						= snapi::keycodes::snapi_f4;
		m_keyCodeMap[ KEY_F5 ]						= snapi::keycodes::snapi_f5;
		m_keyCodeMap[ KEY_F6 ]						= snapi::keycodes::snapi_f6;
		m_keyCodeMap[ KEY_F7 ]						= snapi::keycodes::snapi_f7;
		m_keyCodeMap[ KEY_F8 ]						= snapi::keycodes::snapi_f8;
		m_keyCodeMap[ KEY_F9 ]						= snapi::keycodes::snapi_f9;
		m_keyCodeMap[ KEY_F10 ]						= snapi::keycodes::snapi_f10;
		m_keyCodeMap[ KEY_NUMLOCK ]					= snapi::keycodes::snapi_numlock;
		m_keyCodeMap[ KEY_SCROLLLOCK ]				= snapi::keycodes::snapi_scrolllock;
		m_keyCodeMap[ KEY_KP7 ]						= snapi::keycodes::snapi_kp7;
		m_keyCodeMap[ KEY_KP8 ]						= snapi::keycodes::snapi_kp8;
		m_keyCodeMap[ KEY_KP9 ]						= snapi::keycodes::snapi_kp9;
		m_keyCodeMap[ KEY_KPMINUS ]					= snapi::keycodes::snapi_kpminus;
		m_keyCodeMap[ KEY_KP4 ]						= snapi::keycodes::snapi_kp4;
		m_keyCodeMap[ KEY_KP5 ]						= snapi::keycodes::snapi_kp5;
		m_keyCodeMap[ KEY_KP6 ]						= snapi::keycodes::snapi_kp6;
		m_keyCodeMap[ KEY_KPPLUS ]					= snapi::keycodes::snapi_kpplus;
		m_keyCodeMap[ KEY_KP1 ]						= snapi::keycodes::snapi_kp1;
		m_keyCodeMap[ KEY_KP2 ]						= snapi::keycodes::snapi_kp2;
		m_keyCodeMap[ KEY_KP3 ]						= snapi::keycodes::snapi_kp3;
		m_keyCodeMap[ KEY_KP0 ]						= snapi::keycodes::snapi_kp0;
		m_keyCodeMap[ KEY_KPDOT ]					= snapi::keycodes::snapi_kpdot;
		m_keyCodeMap[ KEY_ZENKAKUHANKAKU ]			= snapi::keycodes::snapi_zenkakuhankaku;
		m_keyCodeMap[ KEY_102ND ]					= snapi::keycodes::snapi_102nd;
		m_keyCodeMap[ KEY_F11 ]						= snapi::keycodes::snapi_f11;
		m_keyCodeMap[ KEY_F12 ]						= snapi::keycodes::snapi_f12;
		m_keyCodeMap[ KEY_RO ]						= snapi::keycodes::snapi_ro;
		m_keyCodeMap[ KEY_KATAKANA ]				= snapi::keycodes::snapi_katakana;
		m_keyCodeMap[ KEY_HIRAGANA ]				= snapi::keycodes::snapi_hiragana;
		m_keyCodeMap[ KEY_HENKAN ]					= snapi::keycodes::snapi_henkan;
		m_keyCodeMap[ KEY_KATAKANAHIRAGANA ]		= snapi::keycodes::snapi_katakanahiragana;
		m_keyCodeMap[ KEY_MUHENKAN ]				= snapi::keycodes::snapi_muhenkan;
		m_keyCodeMap[ KEY_KPJPCOMMA ]				= snapi::keycodes::snapi_kpjpcomma;
		m_keyCodeMap[ KEY_KPENTER ]					= snapi::keycodes::snapi_kpenter;
		m_keyCodeMap[ KEY_RIGHTCTRL ]				= snapi::keycodes::snapi_rightctrl;
		m_keyCodeMap[ KEY_KPSLASH ]					= snapi::keycodes::snapi_kpslash;
		m_keyCodeMap[ KEY_SYSRQ ]					= snapi::keycodes::snapi_sysrq;
		m_keyCodeMap[ KEY_RIGHTALT ]				= snapi::keycodes::snapi_rightalt;
		m_keyCodeMap[ KEY_LINEFEED ]				= snapi::keycodes::snapi_linefeed;
		m_keyCodeMap[ KEY_HOME ]					= snapi::keycodes::snapi_home;
		m_keyCodeMap[ KEY_UP ]						= snapi::keycodes::snapi_up;
		m_keyCodeMap[ KEY_PAGEUP ]					= snapi::keycodes::snapi_pageup;
		m_keyCodeMap[ KEY_LEFT ]					= snapi::keycodes::snapi_left;
		m_keyCodeMap[ KEY_RIGHT ]					= snapi::keycodes::snapi_right;
		m_keyCodeMap[ KEY_END ]						= snapi::keycodes::snapi_end;
		m_keyCodeMap[ KEY_DOWN ]					= snapi::keycodes::snapi_down;
		m_keyCodeMap[ KEY_PAGEDOWN ]				= snapi::keycodes::snapi_pagedown;
		m_keyCodeMap[ KEY_INSERT ]					= snapi::keycodes::snapi_insert;
		m_keyCodeMap[ KEY_DELETE ]					= snapi::keycodes::snapi_delete;
		m_keyCodeMap[ KEY_MACRO ]					= snapi::keycodes::snapi_macro;
		m_keyCodeMap[ KEY_MUTE ]					= snapi::keycodes::snapi_mute;
		m_keyCodeMap[ KEY_VOLUMEDOWN ]				= snapi::keycodes::snapi_volumedown;
		m_keyCodeMap[ KEY_VOLUMEUP ]				= snapi::keycodes::snapi_volumeup;
		m_keyCodeMap[ KEY_POWER ]					= snapi::keycodes::snapi_power;
		m_keyCodeMap[ KEY_KPEQUAL ]					= snapi::keycodes::snapi_kpequal;
		m_keyCodeMap[ KEY_KPPLUSMINUS ]				= snapi::keycodes::snapi_kpplusminus;
		m_keyCodeMap[ KEY_PAUSE ]					= snapi::keycodes::snapi_pause;
		m_keyCodeMap[ KEY_SCALE ]					= snapi::keycodes::snapi_scale;
		m_keyCodeMap[ KEY_KPCOMMA ]					= snapi::keycodes::snapi_kpcomma;
		m_keyCodeMap[ KEY_HANGEUL ]					= snapi::keycodes::snapi_hangeul;
		m_keyCodeMap[ KEY_HANGUEL ]					= snapi::keycodes::snapi_hanguel;
		m_keyCodeMap[ KEY_HANJA ]					= snapi::keycodes::snapi_hanja;
		m_keyCodeMap[ KEY_YEN ]						= snapi::keycodes::snapi_yen;
		m_keyCodeMap[ KEY_LEFTMETA ]				= snapi::keycodes::snapi_leftmeta;
		m_keyCodeMap[ KEY_RIGHTMETA ]				= snapi::keycodes::snapi_rightmeta;
		m_keyCodeMap[ KEY_COMPOSE ]					= snapi::keycodes::snapi_compose;
		m_keyCodeMap[ KEY_STOP ]					= snapi::keycodes::snapi_stop;
		m_keyCodeMap[ KEY_AGAIN ]					= snapi::keycodes::snapi_again;
		m_keyCodeMap[ KEY_PROPS ]					= snapi::keycodes::snapi_props;
		m_keyCodeMap[ KEY_UNDO ]					= snapi::keycodes::snapi_undo;
		m_keyCodeMap[ KEY_FRONT ]					= snapi::keycodes::snapi_front;
		m_keyCodeMap[ KEY_COPY ]					= snapi::keycodes::snapi_copy;
		m_keyCodeMap[ KEY_OPEN ]					= snapi::keycodes::snapi_open;
		m_keyCodeMap[ KEY_PASTE ]					= snapi::keycodes::snapi_paste;
		m_keyCodeMap[ KEY_FIND ]					= snapi::keycodes::snapi_find;
		m_keyCodeMap[ KEY_CUT ]						= snapi::keycodes::snapi_cut;
		m_keyCodeMap[ KEY_HELP ]					= snapi::keycodes::snapi_help;
		m_keyCodeMap[ KEY_MENU ]					= snapi::keycodes::snapi_menu;
		m_keyCodeMap[ KEY_CALC ]					= snapi::keycodes::snapi_calc;
		m_keyCodeMap[ KEY_SETUP ]					= snapi::keycodes::snapi_setup;
		m_keyCodeMap[ KEY_SLEEP ]					= snapi::keycodes::snapi_sleep;
		m_keyCodeMap[ KEY_WAKEUP ]					= snapi::keycodes::snapi_wakeup;
		m_keyCodeMap[ KEY_FILE ]					= snapi::keycodes::snapi_file;
		m_keyCodeMap[ KEY_SENDFILE ]				= snapi::keycodes::snapi_sendfile;
		m_keyCodeMap[ KEY_DELETEFILE ]				= snapi::keycodes::snapi_deletefile;
		m_keyCodeMap[ KEY_XFER ]					= snapi::keycodes::snapi_xfer;
		m_keyCodeMap[ KEY_PROG1 ]					= snapi::keycodes::snapi_prog1;
		m_keyCodeMap[ KEY_PROG2 ]					= snapi::keycodes::snapi_prog2;
		m_keyCodeMap[ KEY_WWW ]						= snapi::keycodes::snapi_www;
		m_keyCodeMap[ KEY_MSDOS ]					= snapi::keycodes::snapi_msdos;
		m_keyCodeMap[ KEY_COFFEE ]					= snapi::keycodes::snapi_coffee;
		m_keyCodeMap[ KEY_SCREENLOCK ]				= snapi::keycodes::snapi_screenlock;
		m_keyCodeMap[ KEY_ROTATE_DISPLAY ]			= snapi::keycodes::snapi_rotate_display;
		m_keyCodeMap[ KEY_DIRECTION ]				= snapi::keycodes::snapi_direction;
		m_keyCodeMap[ KEY_CYCLEWINDOWS ]			= snapi::keycodes::snapi_cyclewindows;
		m_keyCodeMap[ KEY_MAIL ]					= snapi::keycodes::snapi_mail;
		m_keyCodeMap[ KEY_BOOKMARKS ]				= snapi::keycodes::snapi_bookmarks;
		m_keyCodeMap[ KEY_COMPUTER ]				= snapi::keycodes::snapi_computer;
		m_keyCodeMap[ KEY_BACK ]					= snapi::keycodes::snapi_back;
		m_keyCodeMap[ KEY_FORWARD ]					= snapi::keycodes::snapi_forward;
		m_keyCodeMap[ KEY_CLOSECD ]					= snapi::keycodes::snapi_closecd;
		m_keyCodeMap[ KEY_EJECTCD ]					= snapi::keycodes::snapi_ejectcd;
		m_keyCodeMap[ KEY_EJECTCLOSECD ]			= snapi::keycodes::snapi_ejectclosecd;
		m_keyCodeMap[ KEY_NEXTSONG ]				= snapi::keycodes::snapi_nextsong;
		m_keyCodeMap[ KEY_PLAYPAUSE ]				= snapi::keycodes::snapi_playpause;
		m_keyCodeMap[ KEY_PREVIOUSSONG ]			= snapi::keycodes::snapi_previoussong;
		m_keyCodeMap[ KEY_STOPCD ]					= snapi::keycodes::snapi_stopcd;
		m_keyCodeMap[ KEY_RECORD ]					= snapi::keycodes::snapi_record;
		m_keyCodeMap[ KEY_REWIND ]					= snapi::keycodes::snapi_rewind;
		m_keyCodeMap[ KEY_PHONE ]					= snapi::keycodes::snapi_phone;
		m_keyCodeMap[ KEY_ISO ]						= snapi::keycodes::snapi_iso;
		m_keyCodeMap[ KEY_CONFIG ]					= snapi::keycodes::snapi_config;
		m_keyCodeMap[ KEY_HOMEPAGE ]				= snapi::keycodes::snapi_homepage;
		m_keyCodeMap[ KEY_REFRESH ]					= snapi::keycodes::snapi_refresh;
		m_keyCodeMap[ KEY_EXIT ]					= snapi::keycodes::snapi_exit;
		m_keyCodeMap[ KEY_MOVE ]					= snapi::keycodes::snapi_move;
		m_keyCodeMap[ KEY_EDIT ]					= snapi::keycodes::snapi_edit;
		m_keyCodeMap[ KEY_SCROLLUP ]				= snapi::keycodes::snapi_scrollup;
		m_keyCodeMap[ KEY_SCROLLDOWN ]				= snapi::keycodes::snapi_scrolldown;
		m_keyCodeMap[ KEY_KPLEFTPAREN ]				= snapi::keycodes::snapi_kpleftparen;
		m_keyCodeMap[ KEY_KPRIGHTPAREN ]			= snapi::keycodes::snapi_kprightparen;
		m_keyCodeMap[ KEY_NEW ]						= snapi::keycodes::snapi_new;
		m_keyCodeMap[ KEY_REDO ]					= snapi::keycodes::snapi_redo;
		m_keyCodeMap[ KEY_F13 ]						= snapi::keycodes::snapi_f13;
		m_keyCodeMap[ KEY_F14 ]						= snapi::keycodes::snapi_f14;
		m_keyCodeMap[ KEY_F15 ]						= snapi::keycodes::snapi_f15;
		m_keyCodeMap[ KEY_F16 ]						= snapi::keycodes::snapi_f16;
		m_keyCodeMap[ KEY_F17 ]						= snapi::keycodes::snapi_f17;
		m_keyCodeMap[ KEY_F18 ]						= snapi::keycodes::snapi_f18;
		m_keyCodeMap[ KEY_F19 ]						= snapi::keycodes::snapi_f19;
		m_keyCodeMap[ KEY_F20 ]						= snapi::keycodes::snapi_f20;
		m_keyCodeMap[ KEY_F21 ]						= snapi::keycodes::snapi_f21;
		m_keyCodeMap[ KEY_F22 ]						= snapi::keycodes::snapi_f22;
		m_keyCodeMap[ KEY_F23 ]						= snapi::keycodes::snapi_f23;
		m_keyCodeMap[ KEY_F24 ]						= snapi::keycodes::snapi_f24;
		m_keyCodeMap[ KEY_PLAYCD ]					= snapi::keycodes::snapi_playcd;
		m_keyCodeMap[ KEY_PAUSECD ]					= snapi::keycodes::snapi_pausecd;
		m_keyCodeMap[ KEY_PROG3 ]					= snapi::keycodes::snapi_prog3;
		m_keyCodeMap[ KEY_PROG4 ]					= snapi::keycodes::snapi_prog4;
		m_keyCodeMap[ KEY_DASHBOARD ]				= snapi::keycodes::snapi_dashboard;
		m_keyCodeMap[ KEY_SUSPEND ]					= snapi::keycodes::snapi_suspend;
		m_keyCodeMap[ KEY_CLOSE ]					= snapi::keycodes::snapi_close;
		m_keyCodeMap[ KEY_PLAY ]					= snapi::keycodes::snapi_play;
		m_keyCodeMap[ KEY_FASTFORWARD ]				= snapi::keycodes::snapi_fastforward;
		m_keyCodeMap[ KEY_BASSBOOST ]				= snapi::keycodes::snapi_bassboost;
		m_keyCodeMap[ KEY_PRINT ]					= snapi::keycodes::snapi_print;
		m_keyCodeMap[ KEY_HP ]						= snapi::keycodes::snapi_hp;
		m_keyCodeMap[ KEY_CAMERA ]					= snapi::keycodes::snapi_camera;
		m_keyCodeMap[ KEY_SOUND ]					= snapi::keycodes::snapi_sound;
		m_keyCodeMap[ KEY_QUESTION ]				= snapi::keycodes::snapi_question;
		m_keyCodeMap[ KEY_EMAIL ]					= snapi::keycodes::snapi_email;
		m_keyCodeMap[ KEY_CHAT ]					= snapi::keycodes::snapi_chat;
		m_keyCodeMap[ KEY_SEARCH ]					= snapi::keycodes::snapi_search;
		m_keyCodeMap[ KEY_CONNECT ]					= snapi::keycodes::snapi_connect;
		m_keyCodeMap[ KEY_FINANCE ]					= snapi::keycodes::snapi_finance;
		m_keyCodeMap[ KEY_SPORT ]					= snapi::keycodes::snapi_sport;
		m_keyCodeMap[ KEY_SHOP ]					= snapi::keycodes::snapi_shop;
		m_keyCodeMap[ KEY_ALTERASE ]				= snapi::keycodes::snapi_alterase;
		m_keyCodeMap[ KEY_CANCEL ]					= snapi::keycodes::snapi_cancel;
		m_keyCodeMap[ KEY_BRIGHTNESSDOWN ]			= snapi::keycodes::snapi_brightnessdown;
		m_keyCodeMap[ KEY_BRIGHTNESSUP ]			= snapi::keycodes::snapi_brightnessup;
		m_keyCodeMap[ KEY_MEDIA ]					= snapi::keycodes::snapi_media;
		m_keyCodeMap[ KEY_SWITCHVIDEOMODE ]			= snapi::keycodes::snapi_switchvideomode;
		m_keyCodeMap[ KEY_KBDILLUMTOGGLE ]			= snapi::keycodes::snapi_kbdillumtoggle;
		m_keyCodeMap[ KEY_KBDILLUMDOWN ]			= snapi::keycodes::snapi_kbdillumdown;
		m_keyCodeMap[ KEY_KBDILLUMUP ]				= snapi::keycodes::snapi_kbdillumup;
		m_keyCodeMap[ KEY_SEND ]					= snapi::keycodes::snapi_send;
		m_keyCodeMap[ KEY_REPLY ]					= snapi::keycodes::snapi_reply;
		m_keyCodeMap[ KEY_FORWARDMAIL ]				= snapi::keycodes::snapi_forwardmail;
		m_keyCodeMap[ KEY_SAVE ]					= snapi::keycodes::snapi_save;
		m_keyCodeMap[ KEY_DOCUMENTS ]				= snapi::keycodes::snapi_documents;
		m_keyCodeMap[ KEY_BATTERY ]					= snapi::keycodes::snapi_battery;
		m_keyCodeMap[ KEY_BLUETOOTH ]				= snapi::keycodes::snapi_bluetooth;
		m_keyCodeMap[ KEY_WLAN ]					= snapi::keycodes::snapi_wlan;
		m_keyCodeMap[ KEY_UWB ]						= snapi::keycodes::snapi_uwb;
		m_keyCodeMap[ KEY_UNKNOWN ]					= snapi::keycodes::snapi_unknown;
		m_keyCodeMap[ KEY_VIDEO_NEXT ]				= snapi::keycodes::snapi_video_next;
		m_keyCodeMap[ KEY_VIDEO_PREV ]				= snapi::keycodes::snapi_video_prev;
		m_keyCodeMap[ KEY_BRIGHTNESS_CYCLE ]		= snapi::keycodes::snapi_brightness_cycle;
		m_keyCodeMap[ KEY_BRIGHTNESS_AUTO ]			= snapi::keycodes::snapi_brightness_auto;
		m_keyCodeMap[ KEY_BRIGHTNESS_ZERO ]			= snapi::keycodes::snapi_brightness_zero;
		m_keyCodeMap[ KEY_DISPLAY_OFF ]				= snapi::keycodes::snapi_display_off;
		m_keyCodeMap[ KEY_WWAN ]					= snapi::keycodes::snapi_wwan;
		m_keyCodeMap[ KEY_WIMAX ]					= snapi::keycodes::snapi_wimax;
		m_keyCodeMap[ KEY_RFKILL ]					= snapi::keycodes::snapi_rfkill;
		m_keyCodeMap[ KEY_MICMUTE ]					= snapi::keycodes::snapi_micmute;
	}

}