#include "SceneSystem.hpp"
#include "ImguiXlib.hpp"
#include "Systems.hpp"
#include "PrefabLoaderComponent.hpp"

namespace asapi
{

	SceneSystem::SceneSystem( bfu::MemBlockBase* mBlock )
		:m_stream(tmpbuff, 2/*, mBlock*/) // using std allocator to dont waste large chunks of memory on reallocations
	{
		#ifdef IS_EDITOR
		m_stream.resize(1024*1024*10); // reserve 10 Mb buff for reading json
		#endif
	}

	void SceneSystem::Init( bfu::MemBlockBase* mBlock, const int argc, const char** argv )
	{
		p_root = (GameObject*) mBlock->allocate(1, sizeof(GameObject), alignof(GameObject));
		p_root->Init(mBlock);
		p_root->SetName("EntryPoint");
		PrefabLoaderComponent* rootLoader = (PrefabLoaderComponent*) p_root->ADD_COMPONENT( PrefabLoaderComponent );
		rootLoader->SetPrefabID(1);

		GameObject *pgo, *pgo2;

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
				}
				else
				{
					log::error << "You did not suplied path for project. Run '?' fur help." << std::endl;
				}
			}
		}
		#ifdef IS_PLAYER
		startWithJSON ? OpenProject() : LoadRootMMP();
		#endif
	}

	void SceneSystem::OnGUI()
	{
		
	}


	bfu::JSONStream& SceneSystem::GetJSONStreamWorkBuffer()
	{
		return m_stream;
	}


	bool SceneSystem::File2JSON(bfu::JSONStream& jsonStream, const char* filePath)
	{
		char buff[2048];
		bfu::stream stream(buff, 2048, SYSTEMS::SYSTEMS_ALLOCATOR );
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