#include "Systems.hpp"
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include "Systems.hpp"


using namespace asapgl;


void signalHandler( int signum )
{
	SYSTEMS::GetObject().cloaseApp();
	exit(0);
}

int main(const int argc, const char** argv)
{
	SYSTEMS::GetObject().init( argc, argv );
	SYSTEMS::GetObject().mainAppLoop();

    return 0;
}