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

namespace asapi
{

	void devinput::poolEvents()
	{
		#define toPrint(x) #x": %d\n", x
		struct input_event *evp;
		bool mouseMoved = false;
		struct input_event ev[64];
		int result;
		static bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;
		KeyboardEvent keyboardEvent;
		MouseClickEvent mouseClickEvent;
		MouseMoveEvent mouseMoveEvent;

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

				    		asapi::keystates state = (evp->value != 1) ? asapi::keystates::snapi_up : asapi::keystates::snapi_down;
			        		if(evp->code < 255)
			        		{
						    	keyboardEvent.m_key = (int)m_keyCodeMap[evp->code]; 
						    	keyboardEvent.m_state = (int)state; 

						    	p_ev_KeyboardEvent->Invoke( &keyboardEvent );
			        		}
			        		else
			        		{
						    	keyboardEvent.m_key = (int)asapi::keycodes::unknown; 
						    	keyboardEvent.m_state = (int)state; 

						    	p_ev_KeyboardEvent->Invoke( &keyboardEvent );							    
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
				    			asapi::keystates state = (evp->value == 0) ? asapi::keystates::snapi_up : asapi::keystates::snapi_down;
				        		
								mouseClickEvent.m_Xpos = (int)m_mouse_posX;
								mouseClickEvent.m_Ypos = (int)m_mouse_posY;
								mouseClickEvent.m_key = (int)asapi::mousecodes::snapi_wheelY;
								mouseClickEvent.m_state = (int)state;

								p_ev_MouseClickEvent->Invoke( &mouseClickEvent );
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
				    			asapi::keystates state = (evp->value == 0) ? asapi::keystates::snapi_up : asapi::keystates::snapi_down;
				        		
								mouseClickEvent.m_Xpos = (int)m_mouse_posX;
								mouseClickEvent.m_Ypos = (int)m_mouse_posY;
								mouseClickEvent.m_key = (int)asapi::mousecodes::snapi_left;
								mouseClickEvent.m_state = (int)state;

								p_ev_MouseClickEvent->Invoke( &mouseClickEvent );							    
				        	}
				        	else 
				        	{
				        		auto it = m_mouseCodeMap.find( evp->code );
				    			asapi::keystates state = (evp->value == 0) ? asapi::keystates::snapi_up : asapi::keystates::snapi_down;

				        		if(it == m_mouseCodeMap.end() )
				        		{
									mouseClickEvent.m_Xpos = (int)m_mouse_posX;
									mouseClickEvent.m_Ypos = (int)m_mouse_posY;
									mouseClickEvent.m_key = (int)asapi::mousecodes::unknown;
									mouseClickEvent.m_state = (int)state;
								    
									p_ev_MouseClickEvent->Invoke( &mouseClickEvent );
				        		}
				        		else
				        		{
									mouseClickEvent.m_Xpos = (int)m_mouse_posX;
									mouseClickEvent.m_Ypos = (int)m_mouse_posY;
									mouseClickEvent.m_key = (int)it->second;
									mouseClickEvent.m_state = (int)state;
								    
									p_ev_MouseClickEvent->Invoke( &mouseClickEvent );
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

			mouseMoveEvent.m_Xpos = (int)m_mouse_posX;
			mouseMoveEvent.m_Ypos = (int)m_mouse_posY;
		    
		    p_ev_MouseMoveEvent->Invoke( &mouseMoveEvent );
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
			SYSTEMS::DEALLOCATE<struct pollfd>(m_kbdPollStruct, 1);
			//delete m_kbdPollStruct;
			m_kbdPollStruct = 0;
		}


		for(auto it = m_mouses.begin(); it!=m_mouses.end(); ++it)
		{
			close(*it);
		}
		m_mouses.clear();

		if( m_mousePollStruct != 0 ){
			SYSTEMS::DEALLOCATE<struct pollfd>(m_mousePollStruct, 1);
			//delete m_mousePollStruct;
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
		//m_kbdPollStruct = new struct pollfd[ m_kbdFdsSize ];
		m_kbdPollStruct = SYSTEMS::ALLOCATE<struct pollfd>(m_kbdFdsSize);
		new (m_kbdPollStruct) struct pollfd();
		for(int i=0; i<m_kbdFdsSize; ++i)
		{
		    m_kbdPollStruct[i].fd = m_keyboards[i];
		    m_kbdPollStruct[i].events = POLLIN;
		    m_kbdPollStruct[i].revents = 0;
		}

		m_mouseFdsSize = m_mouses.size();
		//m_mousePollStruct = new struct pollfd[ m_mouseFdsSize ];
		m_mousePollStruct = SYSTEMS::ALLOCATE<struct pollfd>(m_mouseFdsSize);
		new (m_mousePollStruct) struct pollfd();
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
		bfu::EventSystem& es = SYSTEMS::GetObject().EVENTS;
		p_ev_KeyboardEvent = es.GetFastEvent("KeyboardEvent");
		p_ev_MouseMoveEvent = es.GetFastEvent("MouseMoveEvent");
		p_ev_MouseClickEvent = es.GetFastEvent("MouseClickEvent");

	   	scanForDevices();

	   	m_mouseCodeMap[ BTN_LEFT ]					= asapi::mousecodes::snapi_left;
	   	m_mouseCodeMap[ BTN_RIGHT ]					= asapi::mousecodes::snapi_right;
	   	m_mouseCodeMap[ BTN_MIDDLE ]				= asapi::mousecodes::snapi_middle;
	   	m_mouseCodeMap[ BTN_BACK ]					= asapi::mousecodes::snapi_back;
	   	m_mouseCodeMap[ BTN_FORWARD ]				= asapi::mousecodes::snapi_forward;

	   	m_keyCodeMap[ KEY_RESERVED ]				= asapi::keycodes::snapi_reserved;
		m_keyCodeMap[ KEY_ESC ]						= asapi::keycodes::snapi_esc;
		m_keyCodeMap[ KEY_1 ]						= asapi::keycodes::snapi_1;
		m_keyCodeMap[ KEY_2 ]						= asapi::keycodes::snapi_2;
		m_keyCodeMap[ KEY_3 ]						= asapi::keycodes::snapi_3;
		m_keyCodeMap[ KEY_4 ]						= asapi::keycodes::snapi_4;
		m_keyCodeMap[ KEY_5 ]						= asapi::keycodes::snapi_5;
		m_keyCodeMap[ KEY_6 ]						= asapi::keycodes::snapi_6;
		m_keyCodeMap[ KEY_7 ]						= asapi::keycodes::snapi_7;
		m_keyCodeMap[ KEY_8 ]						= asapi::keycodes::snapi_8;
		m_keyCodeMap[ KEY_9 ]						= asapi::keycodes::snapi_9;
		m_keyCodeMap[ KEY_0 ]						= asapi::keycodes::snapi_0;
		m_keyCodeMap[ KEY_MINUS ]					= asapi::keycodes::snapi_minus;
		m_keyCodeMap[ KEY_EQUAL ]					= asapi::keycodes::snapi_equal;
		m_keyCodeMap[ KEY_BACKSPACE ]				= asapi::keycodes::snapi_backspace;
		m_keyCodeMap[ KEY_TAB ]						= asapi::keycodes::snapi_tab;
		m_keyCodeMap[ KEY_Q ]						= asapi::keycodes::snapi_q;
		m_keyCodeMap[ KEY_W ]						= asapi::keycodes::snapi_w;
		m_keyCodeMap[ KEY_E ]						= asapi::keycodes::snapi_e;
		m_keyCodeMap[ KEY_R ]						= asapi::keycodes::snapi_r;
		m_keyCodeMap[ KEY_T ]						= asapi::keycodes::snapi_t;
		m_keyCodeMap[ KEY_Y ]						= asapi::keycodes::snapi_y;
		m_keyCodeMap[ KEY_U ]						= asapi::keycodes::snapi_u;
		m_keyCodeMap[ KEY_I ]						= asapi::keycodes::snapi_i;
		m_keyCodeMap[ KEY_O ]						= asapi::keycodes::snapi_o;
		m_keyCodeMap[ KEY_P ]						= asapi::keycodes::snapi_p;
		m_keyCodeMap[ KEY_LEFTBRACE ]				= asapi::keycodes::snapi_leftbrace;
		m_keyCodeMap[ KEY_RIGHTBRACE ]				= asapi::keycodes::snapi_rightbrace;
		m_keyCodeMap[ KEY_ENTER ]					= asapi::keycodes::snapi_enter;
		m_keyCodeMap[ KEY_LEFTCTRL ]				= asapi::keycodes::snapi_leftctrl;
		m_keyCodeMap[ KEY_A ]						= asapi::keycodes::snapi_a;
		m_keyCodeMap[ KEY_S ]						= asapi::keycodes::snapi_s;
		m_keyCodeMap[ KEY_D ]						= asapi::keycodes::snapi_d;
		m_keyCodeMap[ KEY_F ]						= asapi::keycodes::snapi_f;
		m_keyCodeMap[ KEY_G ]						= asapi::keycodes::snapi_g;
		m_keyCodeMap[ KEY_H ]						= asapi::keycodes::snapi_h;
		m_keyCodeMap[ KEY_J ]						= asapi::keycodes::snapi_j;
		m_keyCodeMap[ KEY_K ]						= asapi::keycodes::snapi_k;
		m_keyCodeMap[ KEY_L ]						= asapi::keycodes::snapi_l;
		m_keyCodeMap[ KEY_SEMICOLON ]				= asapi::keycodes::snapi_semicolon;
		m_keyCodeMap[ KEY_APOSTROPHE ]				= asapi::keycodes::snapi_apostrophe;
		m_keyCodeMap[ KEY_GRAVE ]					= asapi::keycodes::snapi_grave;
		m_keyCodeMap[ KEY_LEFTSHIFT ]				= asapi::keycodes::snapi_leftshift;
		m_keyCodeMap[ KEY_BACKSLASH ]				= asapi::keycodes::snapi_backslash;
		m_keyCodeMap[ KEY_Z ]						= asapi::keycodes::snapi_z;
		m_keyCodeMap[ KEY_X ]						= asapi::keycodes::snapi_x;
		m_keyCodeMap[ KEY_C ]						= asapi::keycodes::snapi_c;
		m_keyCodeMap[ KEY_V ]						= asapi::keycodes::snapi_v;
		m_keyCodeMap[ KEY_B ]						= asapi::keycodes::snapi_b;
		m_keyCodeMap[ KEY_N ]						= asapi::keycodes::snapi_n;
		m_keyCodeMap[ KEY_M ]						= asapi::keycodes::snapi_m;
		m_keyCodeMap[ KEY_COMMA ]					= asapi::keycodes::snapi_comma;
		m_keyCodeMap[ KEY_DOT ]						= asapi::keycodes::snapi_dot;
		m_keyCodeMap[ KEY_SLASH ]					= asapi::keycodes::snapi_slash;
		m_keyCodeMap[ KEY_RIGHTSHIFT ]				= asapi::keycodes::snapi_rightshift;
		m_keyCodeMap[ KEY_KPASTERISK ]				= asapi::keycodes::snapi_kpasterisk;
		m_keyCodeMap[ KEY_LEFTALT ]					= asapi::keycodes::snapi_leftalt;
		m_keyCodeMap[ KEY_SPACE ]					= asapi::keycodes::snapi_space;
		m_keyCodeMap[ KEY_CAPSLOCK ]				= asapi::keycodes::snapi_capslock;
		m_keyCodeMap[ KEY_F1 ]						= asapi::keycodes::snapi_f1;
		m_keyCodeMap[ KEY_F2 ]						= asapi::keycodes::snapi_f2;
		m_keyCodeMap[ KEY_F3 ]						= asapi::keycodes::snapi_f3;
		m_keyCodeMap[ KEY_F4 ]						= asapi::keycodes::snapi_f4;
		m_keyCodeMap[ KEY_F5 ]						= asapi::keycodes::snapi_f5;
		m_keyCodeMap[ KEY_F6 ]						= asapi::keycodes::snapi_f6;
		m_keyCodeMap[ KEY_F7 ]						= asapi::keycodes::snapi_f7;
		m_keyCodeMap[ KEY_F8 ]						= asapi::keycodes::snapi_f8;
		m_keyCodeMap[ KEY_F9 ]						= asapi::keycodes::snapi_f9;
		m_keyCodeMap[ KEY_F10 ]						= asapi::keycodes::snapi_f10;
		m_keyCodeMap[ KEY_NUMLOCK ]					= asapi::keycodes::snapi_numlock;
		m_keyCodeMap[ KEY_SCROLLLOCK ]				= asapi::keycodes::snapi_scrolllock;
		m_keyCodeMap[ KEY_KP7 ]						= asapi::keycodes::snapi_kp7;
		m_keyCodeMap[ KEY_KP8 ]						= asapi::keycodes::snapi_kp8;
		m_keyCodeMap[ KEY_KP9 ]						= asapi::keycodes::snapi_kp9;
		m_keyCodeMap[ KEY_KPMINUS ]					= asapi::keycodes::snapi_kpminus;
		m_keyCodeMap[ KEY_KP4 ]						= asapi::keycodes::snapi_kp4;
		m_keyCodeMap[ KEY_KP5 ]						= asapi::keycodes::snapi_kp5;
		m_keyCodeMap[ KEY_KP6 ]						= asapi::keycodes::snapi_kp6;
		m_keyCodeMap[ KEY_KPPLUS ]					= asapi::keycodes::snapi_kpplus;
		m_keyCodeMap[ KEY_KP1 ]						= asapi::keycodes::snapi_kp1;
		m_keyCodeMap[ KEY_KP2 ]						= asapi::keycodes::snapi_kp2;
		m_keyCodeMap[ KEY_KP3 ]						= asapi::keycodes::snapi_kp3;
		m_keyCodeMap[ KEY_KP0 ]						= asapi::keycodes::snapi_kp0;
		m_keyCodeMap[ KEY_KPDOT ]					= asapi::keycodes::snapi_kpdot;
		m_keyCodeMap[ KEY_ZENKAKUHANKAKU ]			= asapi::keycodes::snapi_zenkakuhankaku;
		m_keyCodeMap[ KEY_102ND ]					= asapi::keycodes::snapi_102nd;
		m_keyCodeMap[ KEY_F11 ]						= asapi::keycodes::snapi_f11;
		m_keyCodeMap[ KEY_F12 ]						= asapi::keycodes::snapi_f12;
		m_keyCodeMap[ KEY_RO ]						= asapi::keycodes::snapi_ro;
		m_keyCodeMap[ KEY_KATAKANA ]				= asapi::keycodes::snapi_katakana;
		m_keyCodeMap[ KEY_HIRAGANA ]				= asapi::keycodes::snapi_hiragana;
		m_keyCodeMap[ KEY_HENKAN ]					= asapi::keycodes::snapi_henkan;
		m_keyCodeMap[ KEY_KATAKANAHIRAGANA ]		= asapi::keycodes::snapi_katakanahiragana;
		m_keyCodeMap[ KEY_MUHENKAN ]				= asapi::keycodes::snapi_muhenkan;
		m_keyCodeMap[ KEY_KPJPCOMMA ]				= asapi::keycodes::snapi_kpjpcomma;
		m_keyCodeMap[ KEY_KPENTER ]					= asapi::keycodes::snapi_kpenter;
		m_keyCodeMap[ KEY_RIGHTCTRL ]				= asapi::keycodes::snapi_rightctrl;
		m_keyCodeMap[ KEY_KPSLASH ]					= asapi::keycodes::snapi_kpslash;
		m_keyCodeMap[ KEY_SYSRQ ]					= asapi::keycodes::snapi_sysrq;
		m_keyCodeMap[ KEY_RIGHTALT ]				= asapi::keycodes::snapi_rightalt;
		m_keyCodeMap[ KEY_LINEFEED ]				= asapi::keycodes::snapi_linefeed;
		m_keyCodeMap[ KEY_HOME ]					= asapi::keycodes::snapi_home;
		m_keyCodeMap[ KEY_UP ]						= asapi::keycodes::snapi_up;
		m_keyCodeMap[ KEY_PAGEUP ]					= asapi::keycodes::snapi_pageup;
		m_keyCodeMap[ KEY_LEFT ]					= asapi::keycodes::snapi_left;
		m_keyCodeMap[ KEY_RIGHT ]					= asapi::keycodes::snapi_right;
		m_keyCodeMap[ KEY_END ]						= asapi::keycodes::snapi_end;
		m_keyCodeMap[ KEY_DOWN ]					= asapi::keycodes::snapi_down;
		m_keyCodeMap[ KEY_PAGEDOWN ]				= asapi::keycodes::snapi_pagedown;
		m_keyCodeMap[ KEY_INSERT ]					= asapi::keycodes::snapi_insert;
		m_keyCodeMap[ KEY_DELETE ]					= asapi::keycodes::snapi_delete;
		m_keyCodeMap[ KEY_MACRO ]					= asapi::keycodes::snapi_macro;
		m_keyCodeMap[ KEY_MUTE ]					= asapi::keycodes::snapi_mute;
		m_keyCodeMap[ KEY_VOLUMEDOWN ]				= asapi::keycodes::snapi_volumedown;
		m_keyCodeMap[ KEY_VOLUMEUP ]				= asapi::keycodes::snapi_volumeup;
		m_keyCodeMap[ KEY_POWER ]					= asapi::keycodes::snapi_power;
		m_keyCodeMap[ KEY_KPEQUAL ]					= asapi::keycodes::snapi_kpequal;
		m_keyCodeMap[ KEY_KPPLUSMINUS ]				= asapi::keycodes::snapi_kpplusminus;
		m_keyCodeMap[ KEY_PAUSE ]					= asapi::keycodes::snapi_pause;
		m_keyCodeMap[ KEY_SCALE ]					= asapi::keycodes::snapi_scale;
		m_keyCodeMap[ KEY_KPCOMMA ]					= asapi::keycodes::snapi_kpcomma;
		m_keyCodeMap[ KEY_HANGEUL ]					= asapi::keycodes::snapi_hangeul;
		m_keyCodeMap[ KEY_HANGUEL ]					= asapi::keycodes::snapi_hanguel;
		m_keyCodeMap[ KEY_HANJA ]					= asapi::keycodes::snapi_hanja;
		m_keyCodeMap[ KEY_YEN ]						= asapi::keycodes::snapi_yen;
		m_keyCodeMap[ KEY_LEFTMETA ]				= asapi::keycodes::snapi_leftmeta;
		m_keyCodeMap[ KEY_RIGHTMETA ]				= asapi::keycodes::snapi_rightmeta;
		m_keyCodeMap[ KEY_COMPOSE ]					= asapi::keycodes::snapi_compose;
		m_keyCodeMap[ KEY_STOP ]					= asapi::keycodes::snapi_stop;
		m_keyCodeMap[ KEY_AGAIN ]					= asapi::keycodes::snapi_again;
		m_keyCodeMap[ KEY_PROPS ]					= asapi::keycodes::snapi_props;
		m_keyCodeMap[ KEY_UNDO ]					= asapi::keycodes::snapi_undo;
		m_keyCodeMap[ KEY_FRONT ]					= asapi::keycodes::snapi_front;
		m_keyCodeMap[ KEY_COPY ]					= asapi::keycodes::snapi_copy;
		m_keyCodeMap[ KEY_OPEN ]					= asapi::keycodes::snapi_open;
		m_keyCodeMap[ KEY_PASTE ]					= asapi::keycodes::snapi_paste;
		m_keyCodeMap[ KEY_FIND ]					= asapi::keycodes::snapi_find;
		m_keyCodeMap[ KEY_CUT ]						= asapi::keycodes::snapi_cut;
		m_keyCodeMap[ KEY_HELP ]					= asapi::keycodes::snapi_help;
		m_keyCodeMap[ KEY_MENU ]					= asapi::keycodes::snapi_menu;
		m_keyCodeMap[ KEY_CALC ]					= asapi::keycodes::snapi_calc;
		m_keyCodeMap[ KEY_SETUP ]					= asapi::keycodes::snapi_setup;
		m_keyCodeMap[ KEY_SLEEP ]					= asapi::keycodes::snapi_sleep;
		m_keyCodeMap[ KEY_WAKEUP ]					= asapi::keycodes::snapi_wakeup;
		m_keyCodeMap[ KEY_FILE ]					= asapi::keycodes::snapi_file;
		m_keyCodeMap[ KEY_SENDFILE ]				= asapi::keycodes::snapi_sendfile;
		m_keyCodeMap[ KEY_DELETEFILE ]				= asapi::keycodes::snapi_deletefile;
		m_keyCodeMap[ KEY_XFER ]					= asapi::keycodes::snapi_xfer;
		m_keyCodeMap[ KEY_PROG1 ]					= asapi::keycodes::snapi_prog1;
		m_keyCodeMap[ KEY_PROG2 ]					= asapi::keycodes::snapi_prog2;
		m_keyCodeMap[ KEY_WWW ]						= asapi::keycodes::snapi_www;
		m_keyCodeMap[ KEY_MSDOS ]					= asapi::keycodes::snapi_msdos;
		m_keyCodeMap[ KEY_COFFEE ]					= asapi::keycodes::snapi_coffee;
		m_keyCodeMap[ KEY_SCREENLOCK ]				= asapi::keycodes::snapi_screenlock;
		m_keyCodeMap[ KEY_ROTATE_DISPLAY ]			= asapi::keycodes::snapi_rotate_display;
		m_keyCodeMap[ KEY_DIRECTION ]				= asapi::keycodes::snapi_direction;
		m_keyCodeMap[ KEY_CYCLEWINDOWS ]			= asapi::keycodes::snapi_cyclewindows;
		m_keyCodeMap[ KEY_MAIL ]					= asapi::keycodes::snapi_mail;
		m_keyCodeMap[ KEY_BOOKMARKS ]				= asapi::keycodes::snapi_bookmarks;
		m_keyCodeMap[ KEY_COMPUTER ]				= asapi::keycodes::snapi_computer;
		m_keyCodeMap[ KEY_BACK ]					= asapi::keycodes::snapi_back;
		m_keyCodeMap[ KEY_FORWARD ]					= asapi::keycodes::snapi_forward;
		m_keyCodeMap[ KEY_CLOSECD ]					= asapi::keycodes::snapi_closecd;
		m_keyCodeMap[ KEY_EJECTCD ]					= asapi::keycodes::snapi_ejectcd;
		m_keyCodeMap[ KEY_EJECTCLOSECD ]			= asapi::keycodes::snapi_ejectclosecd;
		m_keyCodeMap[ KEY_NEXTSONG ]				= asapi::keycodes::snapi_nextsong;
		m_keyCodeMap[ KEY_PLAYPAUSE ]				= asapi::keycodes::snapi_playpause;
		m_keyCodeMap[ KEY_PREVIOUSSONG ]			= asapi::keycodes::snapi_previoussong;
		m_keyCodeMap[ KEY_STOPCD ]					= asapi::keycodes::snapi_stopcd;
		m_keyCodeMap[ KEY_RECORD ]					= asapi::keycodes::snapi_record;
		m_keyCodeMap[ KEY_REWIND ]					= asapi::keycodes::snapi_rewind;
		m_keyCodeMap[ KEY_PHONE ]					= asapi::keycodes::snapi_phone;
		m_keyCodeMap[ KEY_ISO ]						= asapi::keycodes::snapi_iso;
		m_keyCodeMap[ KEY_CONFIG ]					= asapi::keycodes::snapi_config;
		m_keyCodeMap[ KEY_HOMEPAGE ]				= asapi::keycodes::snapi_homepage;
		m_keyCodeMap[ KEY_REFRESH ]					= asapi::keycodes::snapi_refresh;
		m_keyCodeMap[ KEY_EXIT ]					= asapi::keycodes::snapi_exit;
		m_keyCodeMap[ KEY_MOVE ]					= asapi::keycodes::snapi_move;
		m_keyCodeMap[ KEY_EDIT ]					= asapi::keycodes::snapi_edit;
		m_keyCodeMap[ KEY_SCROLLUP ]				= asapi::keycodes::snapi_scrollup;
		m_keyCodeMap[ KEY_SCROLLDOWN ]				= asapi::keycodes::snapi_scrolldown;
		m_keyCodeMap[ KEY_KPLEFTPAREN ]				= asapi::keycodes::snapi_kpleftparen;
		m_keyCodeMap[ KEY_KPRIGHTPAREN ]			= asapi::keycodes::snapi_kprightparen;
		m_keyCodeMap[ KEY_NEW ]						= asapi::keycodes::snapi_new;
		m_keyCodeMap[ KEY_REDO ]					= asapi::keycodes::snapi_redo;
		m_keyCodeMap[ KEY_F13 ]						= asapi::keycodes::snapi_f13;
		m_keyCodeMap[ KEY_F14 ]						= asapi::keycodes::snapi_f14;
		m_keyCodeMap[ KEY_F15 ]						= asapi::keycodes::snapi_f15;
		m_keyCodeMap[ KEY_F16 ]						= asapi::keycodes::snapi_f16;
		m_keyCodeMap[ KEY_F17 ]						= asapi::keycodes::snapi_f17;
		m_keyCodeMap[ KEY_F18 ]						= asapi::keycodes::snapi_f18;
		m_keyCodeMap[ KEY_F19 ]						= asapi::keycodes::snapi_f19;
		m_keyCodeMap[ KEY_F20 ]						= asapi::keycodes::snapi_f20;
		m_keyCodeMap[ KEY_F21 ]						= asapi::keycodes::snapi_f21;
		m_keyCodeMap[ KEY_F22 ]						= asapi::keycodes::snapi_f22;
		m_keyCodeMap[ KEY_F23 ]						= asapi::keycodes::snapi_f23;
		m_keyCodeMap[ KEY_F24 ]						= asapi::keycodes::snapi_f24;
		m_keyCodeMap[ KEY_PLAYCD ]					= asapi::keycodes::snapi_playcd;
		m_keyCodeMap[ KEY_PAUSECD ]					= asapi::keycodes::snapi_pausecd;
		m_keyCodeMap[ KEY_PROG3 ]					= asapi::keycodes::snapi_prog3;
		m_keyCodeMap[ KEY_PROG4 ]					= asapi::keycodes::snapi_prog4;
		m_keyCodeMap[ KEY_DASHBOARD ]				= asapi::keycodes::snapi_dashboard;
		m_keyCodeMap[ KEY_SUSPEND ]					= asapi::keycodes::snapi_suspend;
		m_keyCodeMap[ KEY_CLOSE ]					= asapi::keycodes::snapi_close;
		m_keyCodeMap[ KEY_PLAY ]					= asapi::keycodes::snapi_play;
		m_keyCodeMap[ KEY_FASTFORWARD ]				= asapi::keycodes::snapi_fastforward;
		m_keyCodeMap[ KEY_BASSBOOST ]				= asapi::keycodes::snapi_bassboost;
		m_keyCodeMap[ KEY_PRINT ]					= asapi::keycodes::snapi_print;
		m_keyCodeMap[ KEY_HP ]						= asapi::keycodes::snapi_hp;
		m_keyCodeMap[ KEY_CAMERA ]					= asapi::keycodes::snapi_camera;
		m_keyCodeMap[ KEY_SOUND ]					= asapi::keycodes::snapi_sound;
		m_keyCodeMap[ KEY_QUESTION ]				= asapi::keycodes::snapi_question;
		m_keyCodeMap[ KEY_EMAIL ]					= asapi::keycodes::snapi_email;
		m_keyCodeMap[ KEY_CHAT ]					= asapi::keycodes::snapi_chat;
		m_keyCodeMap[ KEY_SEARCH ]					= asapi::keycodes::snapi_search;
		m_keyCodeMap[ KEY_CONNECT ]					= asapi::keycodes::snapi_connect;
		m_keyCodeMap[ KEY_FINANCE ]					= asapi::keycodes::snapi_finance;
		m_keyCodeMap[ KEY_SPORT ]					= asapi::keycodes::snapi_sport;
		m_keyCodeMap[ KEY_SHOP ]					= asapi::keycodes::snapi_shop;
		m_keyCodeMap[ KEY_ALTERASE ]				= asapi::keycodes::snapi_alterase;
		m_keyCodeMap[ KEY_CANCEL ]					= asapi::keycodes::snapi_cancel;
		m_keyCodeMap[ KEY_BRIGHTNESSDOWN ]			= asapi::keycodes::snapi_brightnessdown;
		m_keyCodeMap[ KEY_BRIGHTNESSUP ]			= asapi::keycodes::snapi_brightnessup;
		m_keyCodeMap[ KEY_MEDIA ]					= asapi::keycodes::snapi_media;
		m_keyCodeMap[ KEY_SWITCHVIDEOMODE ]			= asapi::keycodes::snapi_switchvideomode;
		m_keyCodeMap[ KEY_KBDILLUMTOGGLE ]			= asapi::keycodes::snapi_kbdillumtoggle;
		m_keyCodeMap[ KEY_KBDILLUMDOWN ]			= asapi::keycodes::snapi_kbdillumdown;
		m_keyCodeMap[ KEY_KBDILLUMUP ]				= asapi::keycodes::snapi_kbdillumup;
		m_keyCodeMap[ KEY_SEND ]					= asapi::keycodes::snapi_send;
		m_keyCodeMap[ KEY_REPLY ]					= asapi::keycodes::snapi_reply;
		m_keyCodeMap[ KEY_FORWARDMAIL ]				= asapi::keycodes::snapi_forwardmail;
		m_keyCodeMap[ KEY_SAVE ]					= asapi::keycodes::snapi_save;
		m_keyCodeMap[ KEY_DOCUMENTS ]				= asapi::keycodes::snapi_documents;
		m_keyCodeMap[ KEY_BATTERY ]					= asapi::keycodes::snapi_battery;
		m_keyCodeMap[ KEY_BLUETOOTH ]				= asapi::keycodes::snapi_bluetooth;
		m_keyCodeMap[ KEY_WLAN ]					= asapi::keycodes::snapi_wlan;
		m_keyCodeMap[ KEY_UWB ]						= asapi::keycodes::snapi_uwb;
		m_keyCodeMap[ KEY_UNKNOWN ]					= asapi::keycodes::snapi_unknown;
		m_keyCodeMap[ KEY_VIDEO_NEXT ]				= asapi::keycodes::snapi_video_next;
		m_keyCodeMap[ KEY_VIDEO_PREV ]				= asapi::keycodes::snapi_video_prev;
		m_keyCodeMap[ KEY_BRIGHTNESS_CYCLE ]		= asapi::keycodes::snapi_brightness_cycle;
		m_keyCodeMap[ KEY_BRIGHTNESS_AUTO ]			= asapi::keycodes::snapi_brightness_auto;
		m_keyCodeMap[ KEY_BRIGHTNESS_ZERO ]			= asapi::keycodes::snapi_brightness_zero;
		m_keyCodeMap[ KEY_DISPLAY_OFF ]				= asapi::keycodes::snapi_display_off;
		m_keyCodeMap[ KEY_WWAN ]					= asapi::keycodes::snapi_wwan;
		m_keyCodeMap[ KEY_WIMAX ]					= asapi::keycodes::snapi_wimax;
		m_keyCodeMap[ KEY_RFKILL ]					= asapi::keycodes::snapi_rfkill;
		m_keyCodeMap[ KEY_MICMUTE ]					= asapi::keycodes::snapi_micmute;


		// bfu::CallbackId id;
		// SYSTEMS::GetObject().EVENTS.RegisterCallback<ResizeWindowArgs>(id, [&](bfu::EventArgsBase& a)
		// 	{
		// 		ResizeWindowArgs* args = (ResizeWindowArgs*)&a;
		// 		m_mouseXmax = args->m_width; 
		// 		m_mouseYmax = args->m_height; 
		// 	});

		es.GetFastEvent("ResizeWindow")->RegisterCallback(this, devinput::ResizeWindowCallback);
	}

	void devinput::ResizeWindowCallback(void* receiver, void* data)
	{
		devinput* _this = (devinput*)receiver;
		ResizeWindowArgs* args = (ResizeWindowArgs*)data;

		_this->m_mouseXmax = args->m_width; 
		_this->m_mouseYmax = args->m_height; 
	}
}