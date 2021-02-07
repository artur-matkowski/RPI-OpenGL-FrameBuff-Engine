#ifndef _H_TimeSystem
#define _H_TimeSystem
#include <chrono>
#include <ctime>
#include <ratio>


namespace asapgl
{
	class TimeSystem
	{
		struct Random
		{
			Random()
			{
				srand (time(NULL));
			}
			inline void SetSeed(unsigned int seed)
			{
				srand( seed );				
			}

			uint32_t UINT32()
			{
				return rand();
			}
		};

		std::chrono::_V2::system_clock::time_point 		m_applicationStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStartTime;
		std::chrono::_V2::system_clock::time_point		m_frameStopTime;
		std::chrono::duration<double>					m_frameCalculationTime;
		std::chrono::duration<double>					m_frameTargetTime;


	public:
		Random 		RANDOM;

		TimeSystem()
		{
			m_applicationStartTime = std::chrono::high_resolution_clock::now();
		}

		inline void FrameStart()
		{
			m_frameStartTime = std::chrono::high_resolution_clock::now();
		}

		inline void FrameCalculationEnd()
		{
			m_frameCalculationTime = std::chrono::high_resolution_clock::now() - m_frameStartTime;
		}

		inline void SetTargetFPS(double fps)
		{
			m_frameTargetTime = std::chrono::duration<double>(1.0 / fps);
		}

	};
}

#endif