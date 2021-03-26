#include "SceneSystem.hpp"
#include "ImguiXlib.hpp"
#include "Systems.hpp"
#include "PrefabLoaderComponent.hpp"

namespace asapi
{

	SceneSystem::SceneSystem()
		:m_stream(tmpbuff, 2) // using std allocator to dont waste large chunks of memory on reallocations
		,m_jsonSerializer( SYSTEMS::GetObject().MEMORY.GetSystemsAllocator() )
	{

	}

	void SceneSystem::Init( bfu::MemBlockBase* mBlock, const int argc, const char** argv )
	{
		#ifdef IS_EDITOR
		m_stream.resize(1024*1024*10); // reserve 10 Mb buff for reading json
		#endif

		p_root = (GameObject*) mBlock->allocate(1, sizeof(GameObject), alignof(GameObject));
		p_root->Init(mBlock);
		p_root->SetName("EntryPoint");
		PrefabLoaderComponent* rootLoader = (PrefabLoaderComponent*) p_root->ADD_COMPONENT( PrefabLoaderComponent );
		rootLoader->SetPrefabID(1);

		const char* path = "--wrong path--";

		bool startWithJSON = false;
		for(int i=1; i<argc; ++i)
		{
			#ifdef IS_PLAYER
			if( strcmp(argv[i], "-json") == 0 )
			{
				startWithJSON = true;
			}
			else
			#endif
			if( strcmp(argv[i], "-path") == 0 )
			{
				if(argc > i) // if there is next argument
				{
					SetProjectPath(argv[i+1]);
					path = argv[i+1];
				}
				else
				{
					log::error << "You did not suplied path for project. Run '?' fur help." << std::endl;
				}
			}
		}
		#ifdef IS_PLAYER
		if( !(startWithJSON ? OpenProject() : LoadRootMMP() ) )
		{
			log::error << "Could not open project at path " << path << std::endl;
		}
		#endif
	}

	void SceneSystem::OnGUI()
	{
		
	}


	bfu::JSONStream& SceneSystem::GetJSONStreamWorkBuffer()
	{
		return m_stream;
	}
	bfu2::JSONSerializer& SceneSystem::GetJSONSerializer()
	{
		return m_jsonSerializer;
	}


	bool SceneSystem::File2JSON(bfu::JSONStream& jsonStream, const char* filePath)
	{
		char buff[2048];
		bfu::stream stream(buff, 2048, SYSTEMS::STD_ALLOCATOR );
		FILE * pFile = fopen (filePath,"rb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return false;
 		}
		
		fseek(pFile, 0L, SEEK_END); 
		auto fileSize = ftell(pFile); 
		fseek(pFile, 0L, SEEK_SET); 

		jsonStream.resize(fileSize);
		fread(jsonStream.c_str(), 1, fileSize, pFile);
		jsonStream.OverrideWriteCursorPos(fileSize);

		fclose (pFile);
		return true;
	}


	bool SceneSystem::JSON2File(bfu::JSONStream& jsonStream, const char* filePath)
	{
		FILE * pFile = fopen (filePath,"wb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return false;
 		}

		fwrite(jsonStream.c_str(), 1, jsonStream.size(), pFile);

		fclose (pFile);
		return true;
	}

	bool SceneSystem::File2JSON(bfu2::JSONSerializer& jsonStream, const char* filePath)
	{
		char buff[2048];
		bfu::stream stream(buff, 2048, SYSTEMS::STD_ALLOCATOR );
		FILE * pFile = fopen (filePath,"rb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return false;
 		}
		
		fseek(pFile, 0L, SEEK_END); 
		auto fileSize = ftell(pFile); 
		fseek(pFile, 0L, SEEK_SET); 

		jsonStream.resize(fileSize);
		fread(jsonStream.c_str(), 1, fileSize, pFile);
		jsonStream.OverrideWriteCursorPos(fileSize);

		fclose (pFile);
		return true;
	}


	bool SceneSystem::JSON2File(bfu2::JSONSerializer& jsonStream, const char* filePath)
	{
		FILE * pFile = fopen (filePath,"wb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return false;
 		}

		fwrite(jsonStream.c_str(), 1, jsonStream.size(), pFile);

		fclose (pFile);
		return true;
	}


	void SceneSystem::SetProjectPath(const char* path)
	{
		strcpy(m_ProjectPath, path );
	}


	bool SceneSystem::OpenProject()
	{
    	PrefabLoaderComponent* cmp = (PrefabLoaderComponent*) GetRootNode().GET_COMPONENT(PrefabLoaderComponent);
    	return cmp->Load_JSON(); 
	}

	bool SceneSystem::LoadRootMMP()
	{
		return false;
	}
}