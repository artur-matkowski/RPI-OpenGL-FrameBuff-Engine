#include "ResourceSystem2.hpp"
#include "log.hpp"

char** resourcePath;

void Command(const char *format, ...)
{
	char buff[1024];

	va_list args;
	va_start (args, format);
	vsprintf (buff, format, args);
	va_end (args);

	system(buff);
}

void SetupTests(char** argv)
{
	resourcePath = new char*;
	*resourcePath = new char[1024];
	char command[1024];


	strcpy(*resourcePath, argv[1]);
	strcat(*resourcePath, "/assets");

	Command("touch %s/testImage.txt", *resourcePath);
}

void CleanUpTests()
{
	Command("rm %s/*txt", *resourcePath);
}

int main(int argc, char** argv)
{
	if(argc<2)
	{
		log::error << "argv[1] must be a path to project directory." << std::endl;
		return -1;
	}

	SetupTests(argv);



	asapi::ResourceSystem2 res;

	res.Init( resourcePath );
	res.RefreshResources();



	CleanUpTests();

	return 0;
}