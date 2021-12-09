#include "TestsResourceTracker.hxx"
#include "sha256.h"


	TestsResourceTracker::TestsResourceTracker(const char* testProjectPath)
	{
		strncpy(m_testProjectPath, testProjectPath, MAX_PATH_SIZE);

		sprintf(m_ResourceFilesDirPath, "%s/assets/Resource_Trackers/", testProjectPath);

		res.Init( m_testProjectPath );
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

		Command("echo %s > %s/assets/%s", content, m_testProjectPath, filename);


		removedResources++;
	}

	bool ProcessResourceTracker(asapi::ResourceTracker* restrack, void* data)
	{
		asapi::FILE::MMAP _in, _out;
		const std::string binaryResourceDir = std::string( (const char*)data ) + std::string("/assets/Resource_Binaries/");
		const std::string binaryResource = binaryResourceDir + std::to_string( restrack->GetResourceID() ) + std::string(".txt.bin");


		_in.InitForRead( restrack->GetPath().c_str() );

		_out.InitForWrite( binaryResource.c_str(), 64);


		strncpy( (char*)_out.Data(), (const char*)_in.Data(), _in.Size() );

		//log::debug << binaryResourceDir.c_str() << std::endl;

		return false;
	}

	bool TestsResourceTracker::TestDataCohesion(bool (*callback)(asapi::ResourceTracker*, void*))
	{
		bool dataCohesion = true;

		std::vector< std::string > resourceFiles;

		res.RefreshResources();
		res.IterateOverDirtyResourceTrackers( callback, (char*)m_testProjectPath );

		//check if we introduced new resources, and fetch their linkID if so
		for(int i=0; i<currentResources.size(); ++i)
		{
			if( currentResources[i].resourceLink == 0 )
			{
				asapi::ResourceTracker* r = res.FindResourceByContentHash( currentResources[i].content_hash );

				currentResources[i].resourceLink = r->GetResourceID();

				log::debug << "File extension is: " << r->GetFileExtension().c_str() << std::endl;
			}
		}



		asapi::ListFiles(resourceFiles, m_ResourceFilesDirPath, {".bin"} );



		if( currentResources.size() != resourceFiles.size() )
		{
			log::error << "Amount of resource tracker files (*.res.json) differes from test set\n\tcurrentResources.size(" << currentResources.size() << ") != resourceFiles.size(" << resourceFiles.size() << ")" << std::endl;
			dataCohesion |= false;
		}

		if( currentResources.size() != res.v_ResourceTrackers.size() )
		{
			log::error << "Amount of ResourceTracker class objects differes from test set\n\tcurrentResources.size(" << currentResources.size() << ") != AssetSystem::v_ResourceTrackers.size(" << res.v_ResourceTrackers.size() << ")" << std::endl;
			dataCohesion = false;
		}
		
		for(int i=0; i<currentResources.size(); ++i)
		{
			if( res.FindResourceByResourceID( currentResources[i].resourceLink ) == nullptr )
			{
				char buff[256];

				sprintf(buff, "%llu", currentResources[i].resourceLink );

				log::error << "Could not find resource by resourceID in any of the ResourceTracker class objects: "  << buff << std::endl;
				dataCohesion = false;
			}


			//check if resourceIDs coresponds to resourceIDs, files names (*.res.json)
			bool linkFound = false;
			for(int i=0; i<resourceFiles.size(); ++i)
			{
				if( strstr(resourceFiles[i].c_str(), std::to_string(currentResources[i].resourceLink).c_str() ) != 0 )
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

			if( res.FindResourceByResourceID( currentResources[i].resourceLink ) == nullptr )
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
