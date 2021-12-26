#include "TestResourceBinaries.hxx"
#include "sha256.h"


std::vector<std::string> ResourceTXTProcessor::data;


bool ResourceTXTProcessor::ProcessResource2Binary(const asapi::ResourceTracker& in_currentResource
										, asapi::FILE::MMAP* in_resourceFile
										, const char* in_projectPath
										, std::vector<asapi::SubResourceData>* out_resourceBinaries)
{
	std::string binaryResourceDir = in_projectPath;
	binaryResourceDir += RESOURCE_BINARIES_DIR;
	binaryResourceDir += "/";

	out_resourceBinaries->clear();

	//const std::string binaryResource = binaryResourceDir + std::to_string( in_currentResource->GetResourceID() ) + std::string(".txt.bin");


	std::string databuff( (char*)in_resourceFile->Data(), (char*)in_resourceFile->Data()+in_resourceFile->Size() );
	std::istringstream iss(databuff);

	std::string line;
	int i = 0;
	while (std::getline(iss, line))
	{
		asapi::FILE::MMAP _out;
		std::string binaryFilename;
		asapi::UniqueID subresourceID;
		std::string binaryResource;

		bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( std::to_string(i), subresourceID );




		//////////////////////////////////////////////////////////////////////////////////////////////////
		//																								//
		//																								//
		//																								//
		//																								//
		//				This if section needs to be reimplemented in custom implementations.			//
		//				Otherwise binary resource name will not be tranfered between updates			//
		//																								//
		//																								//
		//																								//
		//																								//
		///////////////////////////////////////////////////////////////////////////////////////////////////

		binaryResource = std::to_string( (uint64_t)subresourceID ) + ".txt.bin";

		asapi::SubResourceData subresource(
								subresourceID
								, std::to_string(i));

		binaryResource = binaryResourceDir + binaryResource;

		_out.InitForWrite( binaryResource.c_str(), in_resourceFile->Size());
		memcpy( _out.Data(), (void*)line.c_str(), line.size() );

		i++;
		out_resourceBinaries->push_back( subresource );
	}

	return true;
}

void TestResourceBinaries::Command(const char *format, ...)
{
	char buff[2048];

	va_list args;
	va_start (args, format);
	vsprintf (buff, format, args);
	va_end (args);

	log::info << "Runing command: " << buff << std::endl;
	system(buff);
}


TestResourceBinaries::TestResourceBinaries(const char* testProjectPath)
{
	strncpy(m_testProjectPath, testProjectPath, MAX_PATH_SIZE);

	sprintf(m_ResourceFilesDirPath, "%s" RESOURCE_TRACKERS_DIR "/", testProjectPath);

	m_resourceSystem.Init(  );
	m_resourceSystem.SetProjectPath( m_testProjectPath );
}

void TestResourceBinaries::CreateResource(const char* filename, const char* content)
{
	resourceEntry res;
	res.filename = filename;
	res.content = content;
	char buff[MAX_PATH_SIZE];

	SHA256 sha256; 
	sha256( res.content.c_str(), res.content.size() );
	res.content_hash = sha256.getHash();

	currentResources.push_back( res );

	sprintf(buff, "%s" ASSETS_DIR "/%s", m_testProjectPath, filename);

	asapi::FILE::STREAM file;

	file.InitForWrite( buff );
	file.Write(content, strlen(content));

	introducedResources++;
}
void TestResourceBinaries::MoveResource(const char* source, const char* destination)
{
	for(int i=0; i<currentResources.size(); ++i)
	{
		if( strcmp( currentResources[i].filename.c_str(), source ) == 0 )
		{
			currentResources[i].filename = destination;
		}
	}

	Command("mv %s" ASSETS_DIR "/%s %s" ASSETS_DIR "/%s", m_testProjectPath, source, m_testProjectPath, destination);


	movedResources++;
}
void TestResourceBinaries::RemoveResource(const char* filename)
{
	for(auto it = currentResources.begin(); it!=currentResources.end(); ++it)
	{
		if( strcmp( it->filename.c_str(), filename ) == 0 )
		{
			currentResources.erase( it );
			break;
		}
	}

	Command("rm %s" ASSETS_DIR "/%s", m_testProjectPath, filename);


	removedResources++;
}
void TestResourceBinaries::AppendResource(const char* filename, const char* content)
{

	Command("echo %s > %s" ASSETS_DIR "/%s", content, m_testProjectPath, filename);


	removedResources++;
}

bool TestResourceBinaries::TestDataCohesion()
{
	m_resourceSystem.RefreshResources();

	asapi::ResourceTracker* tracker = m_resourceSystem.GetResourceTrackerByIndex(0);

	asapi::UniqueID id = tracker->GetSubResourceByIndex(0)->m_resourceID;


	m_resourceSystem.InitializeResource<ResourceTXTSharedReference, ResourceTXTProcessor>( id, &m_testResource );

	//asapi::ListFiles(resourceFiles, {".bin"}, asapi::ListingStrategy::blacklist, m_ResourceFilesDirPath );

	return true;
}

bfu::stream& operator<<(bfu::stream& st, const TestResourceBinaries& obj)
{
	ResourceTXTProcessor t;

	st << obj.m_resourceSystem;

	st << t;
	return st;
}


bfu::stream& operator<<(bfu::stream& st, const ResourceTXTProcessor& out )
{
	st << "\n\tResourceTXTProcessor::data:";

	for(int i=0; i<ResourceTXTProcessor::data.size(); i++)
	{
		st << "\n\t\t" << ResourceTXTProcessor::data[i].c_str();
	}

	return st;
}