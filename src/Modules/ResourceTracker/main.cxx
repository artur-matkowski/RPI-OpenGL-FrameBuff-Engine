#include "TestsResourceTracker.hxx"


int main(int argc, char** argv)
{
	bool testsPassed = true;

	if(argc<2)
	{
		log::error << "argv[1] must be a path to project directory." << std::endl;
		return -1;
	}
	log::debug << "Starting tests" << std::endl;

	TestsResourceTracker tests(argv[1]);

	tests.Print();


	tests.CreateResource("resoruce1.txt", "randomData1");
	tests.CreateResource("resoruce2.txt", "randomData11");
	tests.CreateResource("resoruce3.txt", "randomData111");
	testsPassed = testsPassed && tests.TestDataCohesion();



	tests.MoveResource("resoruce1.txt", "resoruce1new.txt");
	testsPassed = testsPassed && tests.TestDataCohesion();



	tests.RemoveResource("resoruce1new.txt");
	testsPassed = testsPassed && tests.TestDataCohesion();
	return 0;



	tests.AppendResource("resoruce2.txt", "randomData22");
	testsPassed = testsPassed && tests.TestDataCohesion();




	asapi::FILE::MMAP file;
	std::string path = argv[1];
	path+= "/autoCreatingDirectoryTest/dfu/tmp.bin";

	file.InitForWrite(path.c_str(), 512);

	strcpy((char*)file.Data(), "dummy content");


	return testsPassed ? 0 : -1;
}