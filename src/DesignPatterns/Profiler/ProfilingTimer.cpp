#include "ProfilingTimer.hpp"
#ifdef USE_PROFILER
#include <iostream>

namespace Prifiler
{
	// ProfilingTimer::ProfilingTimer()
	// {
	// 	new (this) ProfilingTimer( [](double microseconds) {
	// 		std::cerr << "Execution time is ===== " << microseconds << "us" << std::endl;
	// 	} );
	// }


	ProfilingTimer::~ProfilingTimer()
	{
		m_tStop = std::chrono::high_resolution_clock::now();

		double duration = std::chrono::duration_cast<std::chrono::microseconds>( m_tStop - m_tStart ).count();

		m_MesurmentFinishCallBack->Execute(duration);

		delete m_MesurmentFinishCallBack;
	}

}

#endif