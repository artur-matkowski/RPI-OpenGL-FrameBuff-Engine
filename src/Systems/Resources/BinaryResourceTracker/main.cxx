
#include "TestResourceBinaries.hxx"



int main(int argc, char** argv)
{
	bool testsPassed = true;

	if(argc<2)
	{
		log::error << "argv[1] must be a path to project directory." << std::endl;
		return -1;
	}
	log::debug << "Starting tests" << std::endl;

	TestResourceBinaries tests(argv[1]);


	tests.CreateResource("resoruce1.txt", "randomData1\nuut");
	tests.CreateResource("resoruce2.txt", "randomData11");
	tests.CreateResource("resoruce3.txt", "randomData111");

	testsPassed = testsPassed && tests.TestDataCohesion();
	log::debug << tests << std::endl;


	tests.MoveResource("resoruce1.txt", "resoruce1new.txt");

	testsPassed = testsPassed && tests.TestDataCohesion();
	log::debug << tests << std::endl;


	tests.RemoveResource("resoruce1new.txt");

	testsPassed = testsPassed && tests.TestDataCohesion();
	log::debug << tests << std::endl;


	tests.AppendResource("resoruce2.txt", "\nrandomData22");

	testsPassed = testsPassed && tests.TestDataCohesion();
	log::debug << tests << std::endl;




	return testsPassed ? 0 : -1;
}