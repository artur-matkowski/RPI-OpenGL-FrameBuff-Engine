#ifndef _H_devinput
#define _H_devinput
#include <vector>
#include <poll.h>
#include <map>
#include "KeyCodes.hpp"
#include "object.hpp"

#define AsciMap 

namespace snapi
{
	class devinput
	{
		std::vector<int> 	m_keyboards;
		struct pollfd*		m_kbdPollStruct = 0;
		int					m_kbdFdsSize = 0;
		std::vector<int> 	m_mouses;
		struct pollfd*		m_mousePollStruct = 0;
		int					m_mouseFdsSize = 0;

		int 				m_mouse_posX = 0;
		int 				m_mouse_posY = 0;

		int 				m_mouseXmax = -1;
		int 				m_mouseYmax = -1;

		snapi::keycodes		m_keyCodeMap[1+(int)snapi::keycodes::unknown] = {snapi::keycodes::unknown};
		std::map<int, snapi::mousecodes>
							m_mouseCodeMap;

		void (*m_mouseMoveCallback)(int posX, int posY) = 0;
		void (*m_mouseClickCallback)(int posX, int posY, snapi::mousecodes key, snapi::keystates state) = 0;
		void (*m_keyboardCallback)(snapi::keycodes key, snapi::keystates state) = 0;


		void openDevice(const char* path, std::vector<int>& devices);
		void closeDevice();
	public:
		devinput();

		void SetDisplaySize(int mouseXmax, int mouseYmax);

		void SetMouseMoveCallback(void (*mouseMoveCallback)(int posX, int posY) );
		void SetMuseClickCallback(void (*mouseClickCallback)(int posX, int posY, snapi::mousecodes key, snapi::keystates state) );
		void SetKeyboardCallback(void (*keyboardCallback)(snapi::keycodes key, snapi::keystates state) );

		void poolEvents();

		void scanForDevices();
	};
}
#endif
