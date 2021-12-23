#include "TestsResourceTracker.hxx"
#include "sha256.h"

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
			std::string binaryResource;

			bool subresourcePreviouslyExisted = in_currentResource.FindSubResourceByInternalID( std::to_string(i), binaryFilename );




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


			
			if( subresourcePreviouslyExisted )  // <------- this if needs to be reimplemented in all 
			{
				binaryResource = binaryFilename;
			}
			else
			{
				binaryResource = std::to_string( (uint64_t)asapi::UniqueID() ) + ".txt.bin";
			}


			asapi::SubResourceData subresource(
									binaryResource
									, std::to_string(i));

			binaryResource = binaryResourceDir + binaryResource;

			_out.InitForWrite( binaryResource.c_str(), in_resourceFile->Size());
			memcpy( _out.Data(), (void*)line.c_str(), line.size() );




			i++;
			out_resourceBinaries->push_back( subresource );
		}

		return true;
	}

	TestsResourceTracker::TestsResourceTracker(const char* testProjectPath)
	{
		strncpy(m_testProjectPath, testProjectPath, MAX_PATH_SIZE);

		sprintf(m_ResourceFilesDirPath, "%s/assets/Resource_Trackers/", testProjectPath);

		resourceTrackerManager.Init( m_testProjectPath );
	}
	TestsResourceTracker::~TestsResourceTracker()
	{
		//TestsResourceTracker::Command("rm %s/assets/*.txt", m_testProjectPath);
		//TestsResourceTracker::Command("rm %s/assets/Resource_Trackers/*.res.json", m_testProjectPath);
		//TestsResourceTracker::Command("rm -rf %s/auto*", m_testProjectPath);
	}

	void TestsResourceTracker::Command(const char *format, ...)
	{
		char buff[2048];

		va_list args;
		va_start (args, format);
		vsprintf (buff, format, args);
		va_end (args);

		log::info << "Runing command: " << buff << std::endl;
		system(buff);
	}



	void TestsResourceTracker::CreateResource(const char* filename, const char* content)
	{
		resourceEntry res;
		res.filename = filename;
		res.content = content;
		char buff[MAX_PATH_SIZE];

		SHA256 sha256; 
		sha256( res.content.c_str(), res.content.size() );
		res.content_hash = sha256.getHash();

		currentResources.push_back( res );

		sprintf(buff, "%s/assets/%s", m_testProjectPath, filename);

		asapi::FILE::STREAM file;

		file.InitForWrite( buff );
		file.Write(content, strlen(content));

		introducedResources++;
	}
	void TestsResourceTracker::MoveResource(const char* source, const char* destination)
	{
		for(int i=0; i<currentResources.size(); ++i)
		{
			if( strcmp( currentResources[i].filename.c_str(), source ) == 0 )
			{
				currentResources[i].filename = destination;
			}
		}

		Command("mv %s/assets/%s %s/assets/%s", m_testProjectPath, source, m_testProjectPath, destination);


		movedResources++;
	}
	void TestsResourceTracker::RemoveResource(const char* filename)
	{
		for(auto it = currentResources.begin(); it!=currentResources.end(); ++it)
		{
			if( strcmp( it->filename.c_str(), filename ) == 0 )
			{
				currentResources.erase( it );
				break;
			}
		}

		Command("rm %s/assets/%s", m_testProjectPath, filename);


		removedResources++;
	}
	void TestsResourceTracker::AppendResource(const char* filename, const char* content)
	{

		Command("printf '%s' >> %s/assets/%s", content, m_testProjectPath, filename);


		removedResources++;
	}

	

	bool TestsResourceTracker::TestDataCohesion()
	{
		bool dataCohesion = true;

		std::vector< std::string > resourceFiles;

		resourceTrackerManager.RefreshResources();

		//check if we introduced new resources, and fetch their linkID if so
		for(int i=0; i<currentResources.size(); ++i)
		{
			if( currentResources[i].resourceLink == 0 )
			{
				asapi::ResourceTracker* r = resourceTrackerManager.FindResourceByContentHash( currentResources[i].content_hash );

				currentResources[i].resourceLink = r->GetResourceID();

				log::debug << "File name is: " << r->GetFilename().c_str() << std::endl;
			}
		}



		asapi::ListFiles(resourceFiles, {".bin"}, asapi::ListingStrategy::blacklist, m_ResourceFilesDirPath );



		if( currentResources.size() != resourceFiles.size() )
		{
			log::error << "Amount of resource tracker files (*.res.json) differes from test set\n\tcurrentResources.size(" << (int)currentResources.size() << ") != resourceFiles.size(" << resourceFiles.size() << ")" << std::endl;
			dataCohesion = false;
		}

		if( currentResources.size() != resourceTrackerManager.CountResouceTrackers() )
		{
			log::error << "Amount of ResourceTracker class objects differes from test set\n\tcurrentResources.size(" << (int)currentResources.size() << ") != resourceTrackerManager.CountResouceTrackers(" << resourceTrackerManager.CountSubresources() << ")" << std::endl;
			dataCohesion = false;
		}
		
		for(int i=0; i<currentResources.size(); ++i)
		{
			if( resourceTrackerManager.FindResourceByResourceID( currentResources[i].resourceLink ) == nullptr )
			{
				char buff[256];

				sprintf(buff, "%llu", currentResources[i].resourceLink );

				log::error << "Could not find resource by resourceID in any of the ResourceTracker class objects: "  << buff << std::endl;
				dataCohesion = false;
			}


			//check if resourceIDs coresponds to resourceIDs, files names (*.res.json)
			bool linkFound = false;
			for(int j=0; j<resourceFiles.size(); ++j)
			{
				if( strstr(resourceFiles[j].c_str(), std::to_string(currentResources[i].resourceLink).c_str() ) != 0 )
				{
					linkFound = true;
				}
			}

			if( !linkFound )
			{
				char buff[256];

				sprintf(buff, "%llu", currentResources[i].resourceLink );
				
				log::error << "Could not find resource by resourceID in any of the ResourceTracker files (*.res.json): "  << buff << std::endl;
				dataCohesion = false;
			}

			if( resourceTrackerManager.FindResourceByResourceID( currentResources[i].resourceLink ) == nullptr )
			{
				char buff[256];

				sprintf(buff, "%llu", currentResources[i].resourceLink );

				log::error << "Could not find resource by resourceID in any of the ResourceTracker class objects: "  << buff << std::endl;
				dataCohesion = false;
			}
		}


		if( dataCohesion )
		{
			log::warning << "TEST passed" << std::endl;
		}

		else
		{
			log::error << "TEST failed"<< std::endl;
		}

		return dataCohesion;
	}

	void TestsResourceTracker::Print()
	{
		log::debug << resourceTrackerManager << std::endl;
	}
