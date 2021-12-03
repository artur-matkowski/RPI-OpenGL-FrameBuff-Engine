#include "Tests.hxx"

int main(int argc, char** argv)
{
	log::info << " started logging" << std::endl;

	bool testsPassed = true;

	Tests tests;
	TestResource res(0);
	TestResource res2(res);
	

	return testsPassed ? 0 : -1;
}