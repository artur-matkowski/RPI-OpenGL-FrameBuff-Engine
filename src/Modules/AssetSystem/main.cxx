
#include "Tests.hxx"

int main(int argc, char** argv)
{
	if(argc<2)
	{
		log::error << "argv[1] must be a path to project directory." << std::endl;
		return -1;
	}
	log::debug << "Starting tests" << std::endl;

	Tests tests(argv[1]);


	tests.CreateResource("resoruce1.txt", "randomData1");
	tests.CreateResource("resoruce2.txt", "randomData11");
	tests.CreateResource("resoruce3.txt", "randomData111");

	tests.TestDataCohesion();


	tests.MoveResource("resoruce1.txt", "resoruce1new.txt");

	tests.TestDataCohesion();


	tests.RemoveResource("resoruce1new.txt");

	tests.TestDataCohesion();


	tests.RemoveResource("resoruce2.txt");
	tests.CreateResource("resoruce2.txt", "randomData22");

	tests.TestDataCohesion();



	return 0;
}