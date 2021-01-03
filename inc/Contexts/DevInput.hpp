#ifndef _H_devinput
#define _H_devinput
#include <vector>
#include <poll.h>
#include <map>
#include "KeyCodes.hpp"
#include "object.hpp"

#define AsciMap 

namespace asapgl
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

		asapgl::keycodes		m_keyCodeMap[1+(int)asapgl::keycodes::unknown] = {asapgl::keycodes::unknown};
		std::map<int, asapgl::mousecodes>
							m_mouseCodeMap;


		void openDevice(const char* path, std::vector<int>& devices);
		void closeDevice();
	public:
		devinput();

		void SetDisplaySize(int mouseXmax, int mouseYmax);


		void poolEvents();

		void scanForDevices();
	};
}
#endif
