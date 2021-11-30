#include "log.hpp"

#define TESTS
#include "AssetSystem.hpp"

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
	//strcat(*resourcePath, "/assets");

}

void CleanUpTests()
{
	Command("rm %s/assets/*txt", *resourcePath);
	Command("rm %s/assets/Resource_Trackers/*", *resourcePath);
}

struct resourceEntry
{
	bfu::string 	m_content_hash;
	uint64_t  		m_resourceID;

	resourceEntry(bfu::string& arg1, uint64_t arg2) : m_content_hash(arg1), m_resourceID(arg2){};
};

std::vector<resourceEntry> resources;
bool findEntry(bfu::string& arg1, uint64_t arg2)
{
	for(int i=0; i<resources.size(); ++i)
	{
		if(resources[i].m_content_hash == arg1 && resources[i].m_resourceID == arg2)
			return true;
	}
	return false;
}

int main(int argc, char** argv)
{
	if(argc<2)
	{
		log::error << "argv[1] must be a path to project directory." << std::endl;
		return -1;
	}


	SetupTests(argv);


	int expectedUpdates = 0;
	int expectedresources = 0;
	asapi::AssetSystem res;

	res.Init( *resourcePath );



	Command("touch %s/assets/testImage.txt", *resourcePath);
	Command("echo testData > %s/assets/testImage.txt", *resourcePath);
	expectedresources++;

	Command("touch %s/assets/testImage2.txt", *resourcePath);
	Command("echo testData2 > %s/assets/testImage2.txt", *resourcePath);
	expectedresources++;

	Command("touch %s/assets/testImage3.txt", *resourcePath);
	Command("echo testData3 > %s/assets/testImage3.txt", *resourcePath);
	expectedresources++;

	res.RefreshResources();


	resources.clear();
	for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
	{
		resources.push_back( resourceEntry(res.v_ResourceTrackers[i].m_content_hash, res.v_ResourceTrackers[i].m_resourceID.ID()) );
	}


	log::debug << res << std::endl;


	/////////////////////////////////////////////////////////////////////////////////////////
	//move resource
	/////////////////////////////////////////////////////////////////////////////////////////
	Command("mv %s/assets/testImage.txt %s/assets/testImagenew.txt", *resourcePath, *resourcePath);
	expectedUpdates++;

	Command("mv %s/assets/testImage3.txt %s/assets/testImagenew2.txt", *resourcePath, *resourcePath);
	expectedUpdates++; //this is actually expectedresources++ becouse of what happens next
	

	/////////////////////////////////////////////////////////////////////////////////////////
	//move resource and paste newer version in it's original place.
	//expected behaviour for us is to concider old path, as the same resource, and old, movedbinary data, as new (legacy) resource
	/////////////////////////////////////////////////////////////////////////////////////////
	Command("touch %s/assets/testImage3.txt", *resourcePath);
	Command("echo testData3.1 > %s/assets/testImage3.txt", *resourcePath);
	expectedresources++; // this is actually expectedUpdates++ becouse of "mv %s/testImage3.txt %s/testImagenew2.txt"


	res.RefreshResources();
	log::debug << res << std::endl;

	int founded_oldResources = 0;
	for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
	{
		if( findEntry( res.v_ResourceTrackers[i].m_content_hash, res.v_ResourceTrackers[i].m_resourceID.ID()) )
			founded_oldResources++;
		else
		{
			//log::info << "New Resource found: " << res.v_ResourceTrackers[i].m_filename << std::endl;
		}
	}
	

	if(founded_oldResources == (expectedresources-expectedUpdates) && res.v_ResourceTrackers.size() == expectedresources)
		log::info << "TEST passed : old resources found: " << founded_oldResources << " all reosurces found :" << res.v_ResourceTrackers.size() << std::endl;
	else
	{
		log::error << "TEST failed : old resources found: " << founded_oldResources << " all reosurces found :" << res.v_ResourceTrackers.size() << std::endl;
		log::error << "Expected: : old resources found: " << expectedresources-expectedUpdates << " all reosurces found :" << expectedresources << std::endl;
		return -1;
	}

	resources.clear();
	for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
	{
		resources.push_back( resourceEntry(res.v_ResourceTrackers[i].m_content_hash, res.v_ResourceTrackers[i].m_resourceID.ID()) );
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	Command("rm %s/assets/testImage3.txt", *resourcePath);
	expectedUpdates = 0;
	expectedresources--;


	res.RefreshResources();
	log::debug << res << std::endl;


	founded_oldResources = 0;
	for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
	{
		if( findEntry( res.v_ResourceTrackers[i].m_content_hash, res.v_ResourceTrackers[i].m_resourceID.ID()) )
			founded_oldResources++;
		else
		{
			//log::info << "New Resource found: " << res.v_ResourceTrackers[i].m_filename << std::endl;
		}
	}

	if(founded_oldResources == (expectedresources-expectedUpdates) && res.v_ResourceTrackers.size() == expectedresources)
		log::info << "TEST passed : old resources found: " << founded_oldResources << " all reosurces found :" << res.v_ResourceTrackers.size() << std::endl;
	else
	{
		log::error << "TEST failed : old resources found: " << founded_oldResources << " all reosurces found :" << res.v_ResourceTrackers.size() << std::endl;
		log::error << "Expected: : old resources found: " << expectedresources-expectedUpdates << " all reosurces found :" << expectedresources << std::endl;
		return -1;
	}

	resources.clear();
	for(int i=0; i<res.v_ResourceTrackers.size(); ++i)
	{
		resources.push_back( resourceEntry(res.v_ResourceTrackers[i].m_content_hash, res.v_ResourceTrackers[i].m_resourceID.ID()) );
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	//CleanUpTests();

	return 0;
}