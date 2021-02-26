#include "ProfilingTimer.hpp"

#ifdef USE_PROFILER
#define PRIFILE_FUNCTION(x) { \
Prifiler::ProfilingTimer s( [] (double microseconds){ \
			std::cerr << "Execution time of function \""  << #x << "\" :" << microseconds << "us" << std::endl; \
		} ); \
		x;}

#else
#define PRIFILE_FUNCTION(x) x;
#endif