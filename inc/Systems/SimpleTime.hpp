#ifndef _H_SimpleTime
#define _H_SimpleTime
#include <chrono>
#include <ctime>


namespace asapgl
{
	class SimpleTime
	{
		std::chrono::_V2::system_clock::time_point 		m_applicationStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStartTime;
	public:
		SimpleTime()
		{
			m_applicationStartTime = std::chrono::high_resolution_clock::now();
		}

		inline void FrameTimerStart()
		{
			m_frameStartTime = std::chrono::high_resolution_clock::now();
		}

		inline double TimeSinceFrameStart()
		{
			return (std::chrono::high_resolution_clock::now()-m_frameStartTime).count();
		}

		inline double TimeSinceApplicationStart()
		{
			return (std::chrono::high_resolution_clock::now()-m_applicationStartTime).count();
		}
	};
}

#endif