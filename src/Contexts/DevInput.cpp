#include "DevInput.hpp"
#include <stdio.h>
#include <fstream>
#include <fcntl.h>
#include <linux/input.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <bitforge/utils/bfu.hpp>
#include "Systems.hpp"

namespace asapgl
{

	void devinput::poolEvents()
	{
		#define toPrint(x) #x": %d\n", x
		struct input_event *evp;
		bool mouseMoved = false;
		struct input_event ev[64];
		int result;
		static bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;

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

				    		asapgl::keystates state = (evp->value == 1) ? asapgl::keystates::snapi_up : asapgl::keystates::snapi_down;
			        		if(evp->code < 255)
			        		{
							    events.Invoke<KeyboardEvent>([&](KeyboardEvent& args) 
							    {
							    	args.m_key = (int)m_keyCodeMap[evp->code]; 
							    	args.m_state = (int)state; 
							    });
			        		}
			        		else
			        		{
			        			events.Invoke<KeyboardEvent>([&](KeyboardEvent& args) 
							    {
							    	args.m_key = (int)asapgl::keycodes::unknown; 
							    	args.m_state = (int)state; 
							    });
			        		}
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
				    			asapgl::keystates state = (evp->value == 1) ? asapgl::keystates::snapi_up : asapgl::keystates::snapi_down;
				        		events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
							    {
									args.m_Xpos = (int)m_mouse_posX;
									args.m_Ypos = (int)m_mouse_posY;
									args.m_key = (int)asapgl::mousecodes::snapi_wheelY;
									args.m_state = (int)state;
							    });
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
				    			asapgl::keystates state = (evp->value == 1) ? asapgl::keystates::snapi_up : asapgl::keystates::snapi_down;
				        		events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
							    {
									args.m_Xpos = (int)m_mouse_posX;
									args.m_Ypos = (int)m_mouse_posY;
									args.m_key = (int)asapgl::mousecodes::snapi_left;
									args.m_state = (int)state;
							    });
				        	}
				        	else 
				        	{
				        		auto it = m_mouseCodeMap.find( evp->code );
				    			asapgl::keystates state = (evp->value == 1) ? asapgl::keystates::snapi_up : asapgl::keystates::snapi_down;

				        		if(it == m_mouseCodeMap.end() )
				        		{
				        			events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
								    {
										args.m_Xpos = (int)m_mouse_posX;
										args.m_Ypos = (int)m_mouse_posY;
										args.m_key = (int)asapgl::mousecodes::unknown;
										args.m_state = (int)state;
								    });
				        		}
				        		else
				        		{
				        			events.Invoke<MouseClickEvent>([&](MouseClickEvent& args) 
								    {
										args.m_Xpos = (int)m_mouse_posX;
										args.m_Ypos = (int)m_mouse_posY;
										args.m_key = (int)it->second;
										args.m_state = (int)state;
								    });
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

			events.Invoke<MouseMoveEvent>([&](MouseMoveEvent& args) 
		    {
				args.m_Xpos = (int)m_mouse_posX;
				args.m_Ypos = (int)m_mouse_posY;
		    });
		}
		//printf("mouse x:%d\t y:%d\n", m_mouse_posX, m_mouse_posY);

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

	   	m_mouseCodeMap[ BTN_LEFT ]					= asapgl::mousecodes::snapi_left;
	   	m_mouseCodeMap[ BTN_RIGHT ]					= asapgl::mousecodes::snapi_right;
	   	m_mouseCodeMap[ BTN_MIDDLE ]				= asapgl::mousecodes::snapi_middle;
	   	m_mouseCodeMap[ BTN_BACK ]					= asapgl::mousecodes::snapi_back;
	   	m_mouseCodeMap[ BTN_FORWARD ]				= asapgl::mousecodes::snapi_forward;

	   	m_keyCodeMap[ KEY_RESERVED ]				= asapgl::keycodes::snapi_reserved;
		m_keyCodeMap[ KEY_ESC ]						= asapgl::keycodes::snapi_esc;
		m_keyCodeMap[ KEY_1 ]						= asapgl::keycodes::snapi_1;
		m_keyCodeMap[ KEY_2 ]						= asapgl::keycodes::snapi_2;
		m_keyCodeMap[ KEY_3 ]						= asapgl::keycodes::snapi_3;
		m_keyCodeMap[ KEY_4 ]						= asapgl::keycodes::snapi_4;
		m_keyCodeMap[ KEY_5 ]						= asapgl::keycodes::snapi_5;
		m_keyCodeMap[ KEY_6 ]						= asapgl::keycodes::snapi_6;
		m_keyCodeMap[ KEY_7 ]						= asapgl::keycodes::snapi_7;
		m_keyCodeMap[ KEY_8 ]						= asapgl::keycodes::snapi_8;
		m_keyCodeMap[ KEY_9 ]						= asapgl::keycodes::snapi_9;
		m_keyCodeMap[ KEY_0 ]						= asapgl::keycodes::snapi_0;
		m_keyCodeMap[ KEY_MINUS ]					= asapgl::keycodes::snapi_minus;
		m_keyCodeMap[ KEY_EQUAL ]					= asapgl::keycodes::snapi_equal;
		m_keyCodeMap[ KEY_BACKSPACE ]				= asapgl::keycodes::snapi_backspace;
		m_keyCodeMap[ KEY_TAB ]						= asapgl::keycodes::snapi_tab;
		m_keyCodeMap[ KEY_Q ]						= asapgl::keycodes::snapi_q;
		m_keyCodeMap[ KEY_W ]						= asapgl::keycodes::snapi_w;
		m_keyCodeMap[ KEY_E ]						= asapgl::keycodes::snapi_e;
		m_keyCodeMap[ KEY_R ]						= asapgl::keycodes::snapi_r;
		m_keyCodeMap[ KEY_T ]						= asapgl::keycodes::snapi_t;
		m_keyCodeMap[ KEY_Y ]						= asapgl::keycodes::snapi_y;
		m_keyCodeMap[ KEY_U ]						= asapgl::keycodes::snapi_u;
		m_keyCodeMap[ KEY_I ]						= asapgl::keycodes::snapi_i;
		m_keyCodeMap[ KEY_O ]						= asapgl::keycodes::snapi_o;
		m_keyCodeMap[ KEY_P ]						= asapgl::keycodes::snapi_p;
		m_keyCodeMap[ KEY_LEFTBRACE ]				= asapgl::keycodes::snapi_leftbrace;
		m_keyCodeMap[ KEY_RIGHTBRACE ]				= asapgl::keycodes::snapi_rightbrace;
		m_keyCodeMap[ KEY_ENTER ]					= asapgl::keycodes::snapi_enter;
		m_keyCodeMap[ KEY_LEFTCTRL ]				= asapgl::keycodes::snapi_leftctrl;
		m_keyCodeMap[ KEY_A ]						= asapgl::keycodes::snapi_a;
		m_keyCodeMap[ KEY_S ]						= asapgl::keycodes::snapi_s;
		m_keyCodeMap[ KEY_D ]						= asapgl::keycodes::snapi_d;
		m_keyCodeMap[ KEY_F ]						= asapgl::keycodes::snapi_f;
		m_keyCodeMap[ KEY_G ]						= asapgl::keycodes::snapi_g;
		m_keyCodeMap[ KEY_H ]						= asapgl::keycodes::snapi_h;
		m_keyCodeMap[ KEY_J ]						= asapgl::keycodes::snapi_j;
		m_keyCodeMap[ KEY_K ]						= asapgl::keycodes::snapi_k;
		m_keyCodeMap[ KEY_L ]						= asapgl::keycodes::snapi_l;
		m_keyCodeMap[ KEY_SEMICOLON ]				= asapgl::keycodes::snapi_semicolon;
		m_keyCodeMap[ KEY_APOSTROPHE ]				= asapgl::keycodes::snapi_apostrophe;
		m_keyCodeMap[ KEY_GRAVE ]					= asapgl::keycodes::snapi_grave;
		m_keyCodeMap[ KEY_LEFTSHIFT ]				= asapgl::keycodes::snapi_leftshift;
		m_keyCodeMap[ KEY_BACKSLASH ]				= asapgl::keycodes::snapi_backslash;
		m_keyCodeMap[ KEY_Z ]						= asapgl::keycodes::snapi_z;
		m_keyCodeMap[ KEY_X ]						= asapgl::keycodes::snapi_x;
		m_keyCodeMap[ KEY_C ]						= asapgl::keycodes::snapi_c;
		m_keyCodeMap[ KEY_V ]						= asapgl::keycodes::snapi_v;
		m_keyCodeMap[ KEY_B ]						= asapgl::keycodes::snapi_b;
		m_keyCodeMap[ KEY_N ]						= asapgl::keycodes::snapi_n;
		m_keyCodeMap[ KEY_M ]						= asapgl::keycodes::snapi_m;
		m_keyCodeMap[ KEY_COMMA ]					= asapgl::keycodes::snapi_comma;
		m_keyCodeMap[ KEY_DOT ]						= asapgl::keycodes::snapi_dot;
		m_keyCodeMap[ KEY_SLASH ]					= asapgl::keycodes::snapi_slash;
		m_keyCodeMap[ KEY_RIGHTSHIFT ]				= asapgl::keycodes::snapi_rightshift;
		m_keyCodeMap[ KEY_KPASTERISK ]				= asapgl::keycodes::snapi_kpasterisk;
		m_keyCodeMap[ KEY_LEFTALT ]					= asapgl::keycodes::snapi_leftalt;
		m_keyCodeMap[ KEY_SPACE ]					= asapgl::keycodes::snapi_space;
		m_keyCodeMap[ KEY_CAPSLOCK ]				= asapgl::keycodes::snapi_capslock;
		m_keyCodeMap[ KEY_F1 ]						= asapgl::keycodes::snapi_f1;
		m_keyCodeMap[ KEY_F2 ]						= asapgl::keycodes::snapi_f2;
		m_keyCodeMap[ KEY_F3 ]						= asapgl::keycodes::snapi_f3;
		m_keyCodeMap[ KEY_F4 ]						= asapgl::keycodes::snapi_f4;
		m_keyCodeMap[ KEY_F5 ]						= asapgl::keycodes::snapi_f5;
		m_keyCodeMap[ KEY_F6 ]						= asapgl::keycodes::snapi_f6;
		m_keyCodeMap[ KEY_F7 ]						= asapgl::keycodes::snapi_f7;
		m_keyCodeMap[ KEY_F8 ]						= asapgl::keycodes::snapi_f8;
		m_keyCodeMap[ KEY_F9 ]						= asapgl::keycodes::snapi_f9;
		m_keyCodeMap[ KEY_F10 ]						= asapgl::keycodes::snapi_f10;
		m_keyCodeMap[ KEY_NUMLOCK ]					= asapgl::keycodes::snapi_numlock;
		m_keyCodeMap[ KEY_SCROLLLOCK ]				= asapgl::keycodes::snapi_scrolllock;
		m_keyCodeMap[ KEY_KP7 ]						= asapgl::keycodes::snapi_kp7;
		m_keyCodeMap[ KEY_KP8 ]						= asapgl::keycodes::snapi_kp8;
		m_keyCodeMap[ KEY_KP9 ]						= asapgl::keycodes::snapi_kp9;
		m_keyCodeMap[ KEY_KPMINUS ]					= asapgl::keycodes::snapi_kpminus;
		m_keyCodeMap[ KEY_KP4 ]						= asapgl::keycodes::snapi_kp4;
		m_keyCodeMap[ KEY_KP5 ]						= asapgl::keycodes::snapi_kp5;
		m_keyCodeMap[ KEY_KP6 ]						= asapgl::keycodes::snapi_kp6;
		m_keyCodeMap[ KEY_KPPLUS ]					= asapgl::keycodes::snapi_kpplus;
		m_keyCodeMap[ KEY_KP1 ]						= asapgl::keycodes::snapi_kp1;
		m_keyCodeMap[ KEY_KP2 ]						= asapgl::keycodes::snapi_kp2;
		m_keyCodeMap[ KEY_KP3 ]						= asapgl::keycodes::snapi_kp3;
		m_keyCodeMap[ KEY_KP0 ]						= asapgl::keycodes::snapi_kp0;
		m_keyCodeMap[ KEY_KPDOT ]					= asapgl::keycodes::snapi_kpdot;
		m_keyCodeMap[ KEY_ZENKAKUHANKAKU ]			= asapgl::keycodes::snapi_zenkakuhankaku;
		m_keyCodeMap[ KEY_102ND ]					= asapgl::keycodes::snapi_102nd;
		m_keyCodeMap[ KEY_F11 ]						= asapgl::keycodes::snapi_f11;
		m_keyCodeMap[ KEY_F12 ]						= asapgl::keycodes::snapi_f12;
		m_keyCodeMap[ KEY_RO ]						= asapgl::keycodes::snapi_ro;
		m_keyCodeMap[ KEY_KATAKANA ]				= asapgl::keycodes::snapi_katakana;
		m_keyCodeMap[ KEY_HIRAGANA ]				= asapgl::keycodes::snapi_hiragana;
		m_keyCodeMap[ KEY_HENKAN ]					= asapgl::keycodes::snapi_henkan;
		m_keyCodeMap[ KEY_KATAKANAHIRAGANA ]		= asapgl::keycodes::snapi_katakanahiragana;
		m_keyCodeMap[ KEY_MUHENKAN ]				= asapgl::keycodes::snapi_muhenkan;
		m_keyCodeMap[ KEY_KPJPCOMMA ]				= asapgl::keycodes::snapi_kpjpcomma;
		m_keyCodeMap[ KEY_KPENTER ]					= asapgl::keycodes::snapi_kpenter;
		m_keyCodeMap[ KEY_RIGHTCTRL ]				= asapgl::keycodes::snapi_rightctrl;
		m_keyCodeMap[ KEY_KPSLASH ]					= asapgl::keycodes::snapi_kpslash;
		m_keyCodeMap[ KEY_SYSRQ ]					= asapgl::keycodes::snapi_sysrq;
		m_keyCodeMap[ KEY_RIGHTALT ]				= asapgl::keycodes::snapi_rightalt;
		m_keyCodeMap[ KEY_LINEFEED ]				= asapgl::keycodes::snapi_linefeed;
		m_keyCodeMap[ KEY_HOME ]					= asapgl::keycodes::snapi_home;
		m_keyCodeMap[ KEY_UP ]						= asapgl::keycodes::snapi_up;
		m_keyCodeMap[ KEY_PAGEUP ]					= asapgl::keycodes::snapi_pageup;
		m_keyCodeMap[ KEY_LEFT ]					= asapgl::keycodes::snapi_left;
		m_keyCodeMap[ KEY_RIGHT ]					= asapgl::keycodes::snapi_right;
		m_keyCodeMap[ KEY_END ]						= asapgl::keycodes::snapi_end;
		m_keyCodeMap[ KEY_DOWN ]					= asapgl::keycodes::snapi_down;
		m_keyCodeMap[ KEY_PAGEDOWN ]				= asapgl::keycodes::snapi_pagedown;
		m_keyCodeMap[ KEY_INSERT ]					= asapgl::keycodes::snapi_insert;
		m_keyCodeMap[ KEY_DELETE ]					= asapgl::keycodes::snapi_delete;
		m_keyCodeMap[ KEY_MACRO ]					= asapgl::keycodes::snapi_macro;
		m_keyCodeMap[ KEY_MUTE ]					= asapgl::keycodes::snapi_mute;
		m_keyCodeMap[ KEY_VOLUMEDOWN ]				= asapgl::keycodes::snapi_volumedown;
		m_keyCodeMap[ KEY_VOLUMEUP ]				= asapgl::keycodes::snapi_volumeup;
		m_keyCodeMap[ KEY_POWER ]					= asapgl::keycodes::snapi_power;
		m_keyCodeMap[ KEY_KPEQUAL ]					= asapgl::keycodes::snapi_kpequal;
		m_keyCodeMap[ KEY_KPPLUSMINUS ]				= asapgl::keycodes::snapi_kpplusminus;
		m_keyCodeMap[ KEY_PAUSE ]					= asapgl::keycodes::snapi_pause;
		m_keyCodeMap[ KEY_SCALE ]					= asapgl::keycodes::snapi_scale;
		m_keyCodeMap[ KEY_KPCOMMA ]					= asapgl::keycodes::snapi_kpcomma;
		m_keyCodeMap[ KEY_HANGEUL ]					= asapgl::keycodes::snapi_hangeul;
		m_keyCodeMap[ KEY_HANGUEL ]					= asapgl::keycodes::snapi_hanguel;
		m_keyCodeMap[ KEY_HANJA ]					= asapgl::keycodes::snapi_hanja;
		m_keyCodeMap[ KEY_YEN ]						= asapgl::keycodes::snapi_yen;
		m_keyCodeMap[ KEY_LEFTMETA ]				= asapgl::keycodes::snapi_leftmeta;
		m_keyCodeMap[ KEY_RIGHTMETA ]				= asapgl::keycodes::snapi_rightmeta;
		m_keyCodeMap[ KEY_COMPOSE ]					= asapgl::keycodes::snapi_compose;
		m_keyCodeMap[ KEY_STOP ]					= asapgl::keycodes::snapi_stop;
		m_keyCodeMap[ KEY_AGAIN ]					= asapgl::keycodes::snapi_again;
		m_keyCodeMap[ KEY_PROPS ]					= asapgl::keycodes::snapi_props;
		m_keyCodeMap[ KEY_UNDO ]					= asapgl::keycodes::snapi_undo;
		m_keyCodeMap[ KEY_FRONT ]					= asapgl::keycodes::snapi_front;
		m_keyCodeMap[ KEY_COPY ]					= asapgl::keycodes::snapi_copy;
		m_keyCodeMap[ KEY_OPEN ]					= asapgl::keycodes::snapi_open;
		m_keyCodeMap[ KEY_PASTE ]					= asapgl::keycodes::snapi_paste;
		m_keyCodeMap[ KEY_FIND ]					= asapgl::keycodes::snapi_find;
		m_keyCodeMap[ KEY_CUT ]						= asapgl::keycodes::snapi_cut;
		m_keyCodeMap[ KEY_HELP ]					= asapgl::keycodes::snapi_help;
		m_keyCodeMap[ KEY_MENU ]					= asapgl::keycodes::snapi_menu;
		m_keyCodeMap[ KEY_CALC ]					= asapgl::keycodes::snapi_calc;
		m_keyCodeMap[ KEY_SETUP ]					= asapgl::keycodes::snapi_setup;
		m_keyCodeMap[ KEY_SLEEP ]					= asapgl::keycodes::snapi_sleep;
		m_keyCodeMap[ KEY_WAKEUP ]					= asapgl::keycodes::snapi_wakeup;
		m_keyCodeMap[ KEY_FILE ]					= asapgl::keycodes::snapi_file;
		m_keyCodeMap[ KEY_SENDFILE ]				= asapgl::keycodes::snapi_sendfile;
		m_keyCodeMap[ KEY_DELETEFILE ]				= asapgl::keycodes::snapi_deletefile;
		m_keyCodeMap[ KEY_XFER ]					= asapgl::keycodes::snapi_xfer;
		m_keyCodeMap[ KEY_PROG1 ]					= asapgl::keycodes::snapi_prog1;
		m_keyCodeMap[ KEY_PROG2 ]					= asapgl::keycodes::snapi_prog2;
		m_keyCodeMap[ KEY_WWW ]						= asapgl::keycodes::snapi_www;
		m_keyCodeMap[ KEY_MSDOS ]					= asapgl::keycodes::snapi_msdos;
		m_keyCodeMap[ KEY_COFFEE ]					= asapgl::keycodes::snapi_coffee;
		m_keyCodeMap[ KEY_SCREENLOCK ]				= asapgl::keycodes::snapi_screenlock;
		m_keyCodeMap[ KEY_ROTATE_DISPLAY ]			= asapgl::keycodes::snapi_rotate_display;
		m_keyCodeMap[ KEY_DIRECTION ]				= asapgl::keycodes::snapi_direction;
		m_keyCodeMap[ KEY_CYCLEWINDOWS ]			= asapgl::keycodes::snapi_cyclewindows;
		m_keyCodeMap[ KEY_MAIL ]					= asapgl::keycodes::snapi_mail;
		m_keyCodeMap[ KEY_BOOKMARKS ]				= asapgl::keycodes::snapi_bookmarks;
		m_keyCodeMap[ KEY_COMPUTER ]				= asapgl::keycodes::snapi_computer;
		m_keyCodeMap[ KEY_BACK ]					= asapgl::keycodes::snapi_back;
		m_keyCodeMap[ KEY_FORWARD ]					= asapgl::keycodes::snapi_forward;
		m_keyCodeMap[ KEY_CLOSECD ]					= asapgl::keycodes::snapi_closecd;
		m_keyCodeMap[ KEY_EJECTCD ]					= asapgl::keycodes::snapi_ejectcd;
		m_keyCodeMap[ KEY_EJECTCLOSECD ]			= asapgl::keycodes::snapi_ejectclosecd;
		m_keyCodeMap[ KEY_NEXTSONG ]				= asapgl::keycodes::snapi_nextsong;
		m_keyCodeMap[ KEY_PLAYPAUSE ]				= asapgl::keycodes::snapi_playpause;
		m_keyCodeMap[ KEY_PREVIOUSSONG ]			= asapgl::keycodes::snapi_previoussong;
		m_keyCodeMap[ KEY_STOPCD ]					= asapgl::keycodes::snapi_stopcd;
		m_keyCodeMap[ KEY_RECORD ]					= asapgl::keycodes::snapi_record;
		m_keyCodeMap[ KEY_REWIND ]					= asapgl::keycodes::snapi_rewind;
		m_keyCodeMap[ KEY_PHONE ]					= asapgl::keycodes::snapi_phone;
		m_keyCodeMap[ KEY_ISO ]						= asapgl::keycodes::snapi_iso;
		m_keyCodeMap[ KEY_CONFIG ]					= asapgl::keycodes::snapi_config;
		m_keyCodeMap[ KEY_HOMEPAGE ]				= asapgl::keycodes::snapi_homepage;
		m_keyCodeMap[ KEY_REFRESH ]					= asapgl::keycodes::snapi_refresh;
		m_keyCodeMap[ KEY_EXIT ]					= asapgl::keycodes::snapi_exit;
		m_keyCodeMap[ KEY_MOVE ]					= asapgl::keycodes::snapi_move;
		m_keyCodeMap[ KEY_EDIT ]					= asapgl::keycodes::snapi_edit;
		m_keyCodeMap[ KEY_SCROLLUP ]				= asapgl::keycodes::snapi_scrollup;
		m_keyCodeMap[ KEY_SCROLLDOWN ]				= asapgl::keycodes::snapi_scrolldown;
		m_keyCodeMap[ KEY_KPLEFTPAREN ]				= asapgl::keycodes::snapi_kpleftparen;
		m_keyCodeMap[ KEY_KPRIGHTPAREN ]			= asapgl::keycodes::snapi_kprightparen;
		m_keyCodeMap[ KEY_NEW ]						= asapgl::keycodes::snapi_new;
		m_keyCodeMap[ KEY_REDO ]					= asapgl::keycodes::snapi_redo;
		m_keyCodeMap[ KEY_F13 ]						= asapgl::keycodes::snapi_f13;
		m_keyCodeMap[ KEY_F14 ]						= asapgl::keycodes::snapi_f14;
		m_keyCodeMap[ KEY_F15 ]						= asapgl::keycodes::snapi_f15;
		m_keyCodeMap[ KEY_F16 ]						= asapgl::keycodes::snapi_f16;
		m_keyCodeMap[ KEY_F17 ]						= asapgl::keycodes::snapi_f17;
		m_keyCodeMap[ KEY_F18 ]						= asapgl::keycodes::snapi_f18;
		m_keyCodeMap[ KEY_F19 ]						= asapgl::keycodes::snapi_f19;
		m_keyCodeMap[ KEY_F20 ]						= asapgl::keycodes::snapi_f20;
		m_keyCodeMap[ KEY_F21 ]						= asapgl::keycodes::snapi_f21;
		m_keyCodeMap[ KEY_F22 ]						= asapgl::keycodes::snapi_f22;
		m_keyCodeMap[ KEY_F23 ]						= asapgl::keycodes::snapi_f23;
		m_keyCodeMap[ KEY_F24 ]						= asapgl::keycodes::snapi_f24;
		m_keyCodeMap[ KEY_PLAYCD ]					= asapgl::keycodes::snapi_playcd;
		m_keyCodeMap[ KEY_PAUSECD ]					= asapgl::keycodes::snapi_pausecd;
		m_keyCodeMap[ KEY_PROG3 ]					= asapgl::keycodes::snapi_prog3;
		m_keyCodeMap[ KEY_PROG4 ]					= asapgl::keycodes::snapi_prog4;
		m_keyCodeMap[ KEY_DASHBOARD ]				= asapgl::keycodes::snapi_dashboard;
		m_keyCodeMap[ KEY_SUSPEND ]					= asapgl::keycodes::snapi_suspend;
		m_keyCodeMap[ KEY_CLOSE ]					= asapgl::keycodes::snapi_close;
		m_keyCodeMap[ KEY_PLAY ]					= asapgl::keycodes::snapi_play;
		m_keyCodeMap[ KEY_FASTFORWARD ]				= asapgl::keycodes::snapi_fastforward;
		m_keyCodeMap[ KEY_BASSBOOST ]				= asapgl::keycodes::snapi_bassboost;
		m_keyCodeMap[ KEY_PRINT ]					= asapgl::keycodes::snapi_print;
		m_keyCodeMap[ KEY_HP ]						= asapgl::keycodes::snapi_hp;
		m_keyCodeMap[ KEY_CAMERA ]					= asapgl::keycodes::snapi_camera;
		m_keyCodeMap[ KEY_SOUND ]					= asapgl::keycodes::snapi_sound;
		m_keyCodeMap[ KEY_QUESTION ]				= asapgl::keycodes::snapi_question;
		m_keyCodeMap[ KEY_EMAIL ]					= asapgl::keycodes::snapi_email;
		m_keyCodeMap[ KEY_CHAT ]					= asapgl::keycodes::snapi_chat;
		m_keyCodeMap[ KEY_SEARCH ]					= asapgl::keycodes::snapi_search;
		m_keyCodeMap[ KEY_CONNECT ]					= asapgl::keycodes::snapi_connect;
		m_keyCodeMap[ KEY_FINANCE ]					= asapgl::keycodes::snapi_finance;
		m_keyCodeMap[ KEY_SPORT ]					= asapgl::keycodes::snapi_sport;
		m_keyCodeMap[ KEY_SHOP ]					= asapgl::keycodes::snapi_shop;
		m_keyCodeMap[ KEY_ALTERASE ]				= asapgl::keycodes::snapi_alterase;
		m_keyCodeMap[ KEY_CANCEL ]					= asapgl::keycodes::snapi_cancel;
		m_keyCodeMap[ KEY_BRIGHTNESSDOWN ]			= asapgl::keycodes::snapi_brightnessdown;
		m_keyCodeMap[ KEY_BRIGHTNESSUP ]			= asapgl::keycodes::snapi_brightnessup;
		m_keyCodeMap[ KEY_MEDIA ]					= asapgl::keycodes::snapi_media;
		m_keyCodeMap[ KEY_SWITCHVIDEOMODE ]			= asapgl::keycodes::snapi_switchvideomode;
		m_keyCodeMap[ KEY_KBDILLUMTOGGLE ]			= asapgl::keycodes::snapi_kbdillumtoggle;
		m_keyCodeMap[ KEY_KBDILLUMDOWN ]			= asapgl::keycodes::snapi_kbdillumdown;
		m_keyCodeMap[ KEY_KBDILLUMUP ]				= asapgl::keycodes::snapi_kbdillumup;
		m_keyCodeMap[ KEY_SEND ]					= asapgl::keycodes::snapi_send;
		m_keyCodeMap[ KEY_REPLY ]					= asapgl::keycodes::snapi_reply;
		m_keyCodeMap[ KEY_FORWARDMAIL ]				= asapgl::keycodes::snapi_forwardmail;
		m_keyCodeMap[ KEY_SAVE ]					= asapgl::keycodes::snapi_save;
		m_keyCodeMap[ KEY_DOCUMENTS ]				= asapgl::keycodes::snapi_documents;
		m_keyCodeMap[ KEY_BATTERY ]					= asapgl::keycodes::snapi_battery;
		m_keyCodeMap[ KEY_BLUETOOTH ]				= asapgl::keycodes::snapi_bluetooth;
		m_keyCodeMap[ KEY_WLAN ]					= asapgl::keycodes::snapi_wlan;
		m_keyCodeMap[ KEY_UWB ]						= asapgl::keycodes::snapi_uwb;
		m_keyCodeMap[ KEY_UNKNOWN ]					= asapgl::keycodes::snapi_unknown;
		m_keyCodeMap[ KEY_VIDEO_NEXT ]				= asapgl::keycodes::snapi_video_next;
		m_keyCodeMap[ KEY_VIDEO_PREV ]				= asapgl::keycodes::snapi_video_prev;
		m_keyCodeMap[ KEY_BRIGHTNESS_CYCLE ]		= asapgl::keycodes::snapi_brightness_cycle;
		m_keyCodeMap[ KEY_BRIGHTNESS_AUTO ]			= asapgl::keycodes::snapi_brightness_auto;
		m_keyCodeMap[ KEY_BRIGHTNESS_ZERO ]			= asapgl::keycodes::snapi_brightness_zero;
		m_keyCodeMap[ KEY_DISPLAY_OFF ]				= asapgl::keycodes::snapi_display_off;
		m_keyCodeMap[ KEY_WWAN ]					= asapgl::keycodes::snapi_wwan;
		m_keyCodeMap[ KEY_WIMAX ]					= asapgl::keycodes::snapi_wimax;
		m_keyCodeMap[ KEY_RFKILL ]					= asapgl::keycodes::snapi_rfkill;
		m_keyCodeMap[ KEY_MICMUTE ]					= asapgl::keycodes::snapi_micmute;

    	bfu::CallbackId id;
		SYSTEMS::GetObject().EVENTS.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
	    {
		    ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
	    	m_mouseXmax = args->m_width; 
	    	m_mouseYmax = args->m_height; 
	    });
	}

}