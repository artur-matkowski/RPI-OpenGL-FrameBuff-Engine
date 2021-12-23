#include "TestsResourceTracker.hxx"

class TMP1
{
public:
	static void FUN()
	{
		log::debug << "TMP1::FUN()" << std::endl;
	}
};
class TMP2
{
public:
	static void FUN()
	{
		log::debug << "TMP2::FUN()" << std::endl;
	}
};



template<class... ResourceReferencesTs>
class TMP
{
	template<int... Is>
	void classIterator(std::integer_sequence<int, Is...> const &)
	{
		using unused = int[];

		(void)unused { 0, (std::tuple_element_t<Is, mytuple>::FUN(), 0)... };
	}
public:
	
	using mytuple = std::tuple<ResourceReferencesTs ...>;

	void fun()
	{
		constexpr int tupleSize = std::tuple_size<mytuple>();
		classIterator(std::make_integer_sequence<int, tupleSize>{});
	}
};

int main(int argc, char** argv)
{
	TMP<TMP2, TMP1> dd;
	dd.fun();

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
	tests.CreateResource("resoruce2.txt", "randomData11\nline2data");
	tests.CreateResource("somedir/resoruce3.txt", "randomData111");
	testsPassed = testsPassed && tests.TestDataCohesion();
	tests.Print();
	//return 0;


	tests.MoveResource("resoruce1.txt", "resoruce1new.txt");
	testsPassed = testsPassed && tests.TestDataCohesion();
	tests.Print();



	tests.RemoveResource("resoruce1new.txt");
	testsPassed = testsPassed && tests.TestDataCohesion();
	tests.Print();
	//return 0;



	tests.AppendResource("resoruce2.txt", "\nrandomData22");
	testsPassed = testsPassed && tests.TestDataCohesion();
	tests.Print();



	return 0;
	asapi::FILE::MMAP file;
	std::string path = argv[1];
	path+= "/autoCreatingDirectoryTest/dfu/tmp.bin";

	file.InitForWrite(path.c_str(), 512);

	strcpy((char*)file.Data(), "dummy content");


	return testsPassed ? 0 : -1;
}