
#include "Tests.hxx"

int main(int argc, char** argv)
{
	if(argc<2)
	{
		log::error << "argv[1] must be a path to project directory." << std::endl;
		return -1;
	}

	Tests tests(argv[1]);


	tests.CreateResource("resoruce1.txt", "randomData1");
	tests.CreateResource("resoruce2.txt", "randomData11");
	tests.CreateResource("resoruce3.txt", "randomData111");

	tests.TestDataCohesion();


	tests.MoveResource("resoruce1.txt", "resoruce1new.txt");

	tests.TestDataCohesion();

/*
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

	CleanUpTests();
*/



	return 0;
}