#include "SceneSystem.hpp"
#include "ImguiXlib.hpp"
#include "Systems.hpp"
#include "PrefabLoaderComponent.hpp"

namespace asapi
{

	SceneSystem::SceneSystem()
		:m_jsonSerializer( SYSTEMS::GetObject().MEMORY.GetSystemsAllocator() )
	{}

	void SceneSystem::Init( bfu::MemBlockBase* mBlock, const int argc, const char** argv )
	{
		p_root = (GameObject*) mBlock->allocate(1, sizeof(GameObject), alignof(GameObject));
		p_root->Init(mBlock);
		p_root->SetName("EntryPoint");
		PrefabLoaderComponent* rootLoader = (PrefabLoaderComponent*) p_root->ADD_COMPONENT( PrefabLoaderComponent );
		rootLoader->SetPrefabID(0);

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


	bfu::JSONSerializer& SceneSystem::GetJSONSerializer()
	{
		return m_jsonSerializer;
	}



	bool SceneSystem::File2JSON(bfu::JSONSerializer& jsonStream, const char* filePath)
	{
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


	bool SceneSystem::JSON2File(bfu::JSONSerializer& jsonStream, const char* filePath)
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

	bool SceneSystem::File2JSON(bfu::SerializableClassInterface* obj, const char* filePath)
	{
		bfu::JSONSerializer& jsonSerializer = SYSTEMS::GetObject().SCENE.GetJSONSerializer();
		jsonSerializer.clear();

		FILE * pFile = fopen (filePath,"rb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return false;
 		}
		
		fseek(pFile, 0L, SEEK_END); 
		auto fileSize = ftell(pFile); 
		fseek(pFile, 0L, SEEK_SET); 

		jsonSerializer.resize(fileSize);
		fread(jsonSerializer.c_str(), 1, fileSize, pFile);
		jsonSerializer.OverrideWriteCursorPos(fileSize);

		fclose (pFile);

		jsonSerializer.Deserialize(obj);

		return true;
	}


	bool SceneSystem::JSON2File(bfu::SerializableClassInterface* obj, const char* filePath)
	{
		bfu::JSONSerializer& jsonSerializer = SYSTEMS::GetObject().SCENE.GetJSONSerializer();
		jsonSerializer.clear();

		FILE * pFile = fopen (filePath,"wb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return false;
 		}

		jsonSerializer.Serialize(obj);
		fwrite(jsonSerializer.c_str(), 1, jsonSerializer.size(), pFile);

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