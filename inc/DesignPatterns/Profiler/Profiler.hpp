#include "ProfilingTimer.hpp"

#ifdef USE_PROFILER
#define PRIFILE_FUNCTION(x) { \
Prifiler::ProfilingTimer s( [] (double microseconds){ \
			std::cerr << "Execution time of function \""  << #x << "\" :" << microseconds << "us called from " << __FILE__<<":"<<__LINE__ << std::endl; \
		} ); \
		x;}

#else
#define PRIFILE_FUNCTION(x) x;
#endif