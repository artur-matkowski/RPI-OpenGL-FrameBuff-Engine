#ifndef _H_TimeSystem
#define _H_TimeSystem
#include <chrono>
#include <ctime>
#include <ratio>


namespace asapgl
{
	class TimeSystem
	{
		std::chrono::_V2::system_clock::time_point 		m_applicationStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStopTime;
	public:
		TimeSystem()
		{
			m_applicationStartTime = std::chrono::high_resolution_clock::now();
		}

	};
}

#endif