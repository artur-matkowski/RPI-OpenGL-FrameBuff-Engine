#ifndef _H_SimpleTime
#define _H_SimpleTime
#include <chrono>
#include <ctime>
#include <ratio>


namespace asapgl
{
	class SimpleTime
	{
		std::chrono::_V2::system_clock::time_point 		m_applicationStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStopTime;
	public:
		SimpleTime()
		{
			m_applicationStartTime = std::chrono::high_resolution_clock::now();
		}

	};
}

#endif