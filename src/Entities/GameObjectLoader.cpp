#include "GameObjectLoader.hpp"
#include "Systems.hpp"

namespace asapgl
{
	void GameObjectLoader::RegisterChild(GameObject* newChild)
	{
		log::warning << "Can not RegisterChild to GameObjectLoader, passing to child" << std::endl;
		v_children[0]->RegisterChild( newChild );
	}
	void GameObjectLoader::UnRegisterChild(GameObject* deleteChild)
	{
		log::warning << "Can not UnRegisterChild to GameObjectLoader, passing to child" << std::endl;
		v_children[0]->UnRegisterChild( deleteChild );
	}

	void GameObjectLoader::InstatiatePrefab()
	{
		GameObject* GoRoot = 0;
		bfu::MmappedMemBlock* mBlock = SYSTEMS::GetObject().MEMORY.ObtainPrefabMemBlock(1024*1024*10, GoRoot, m_prefabMemFile.c_str() );
		GoRoot->Init(mBlock);

		GoRoot->SetName("Prefab Root");

		v_children.push_back(GoRoot);
	}

	GameObjectLoader::GameObjectLoader( bfu::MemBlockBase* mBlock )
		:GameObject(mBlock)
		,m_prefabMemFile("m_prefabMemFile", this, mBlock)
	{
		m_prefabMemFile.reserve(GAMEOBJECT_MAX_NAME_LENGTH);
		b_isGameObjectLoader = true;
	}
	GameObjectLoader::~GameObjectLoader()
	{
	}

	void GameObjectLoader::Init( bfu::MemBlockBase* mBlock )
	{
		new (this) GameObjectLoader(mBlock);
	}
	void GameObjectLoader::Dispouse()
	{
		for(auto it = v_children.begin(); 
			it != v_children.end();
			++it)
		{
			(*it)->DispouseAndDeallocate();
		}
		this->~GameObjectLoader();
	}
	void GameObjectLoader::DispouseAndDeallocate()
	{
		for(auto it = v_children.begin(); 
			it != v_children.end();
			++it)
		{
			(*it)->DispouseAndDeallocate();
		}
		this->~GameObjectLoader();
		m_mBlock->deallocate(this, sizeof(GameObjectLoader));
	}

	void GameObjectLoader::SetPrefabMemFileName(const char* mmapFileName)
	{
		m_prefabMemFile = mmapFileName;
	}
	bool GameObjectLoader::Load()
	{

		return true;
	}


	void GameObjectLoader::Serialize()
	{
		//we will pe using GameObject name as prefab file name
		char filePath[GAMEOBJECT_MAX_NAME_LENGTH+sizeof(SERIALIZATION_FILE_EXT)+sizeof(SERIALIZATION_FOLDER)]; 
		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();

		//building a file name;
		strcpy(filePath, SERIALIZATION_FOLDER );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1, m_prefabMemFile.c_str(), GAMEOBJECT_MAX_NAME_LENGTH );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1+m_prefabMemFile.size(), SERIALIZATION_FILE_EXT, sizeof(SERIALIZATION_FILE_EXT));

		FILE * pFile = fopen (filePath,"wb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return;
 		}

 		jsonStream << *v_children[0];

		fwrite(jsonStream.c_str(), 1, jsonStream.size(), pFile);

		fclose (pFile);

		jsonStream.clear();
	}
	void GameObjectLoader::Deserialize()
	{
		//we will pe using GameObject name as prefab file name
		char filePath[GAMEOBJECT_MAX_NAME_LENGTH+sizeof(SERIALIZATION_FILE_EXT)+sizeof(SERIALIZATION_FOLDER)]; 
		bfu::JSONStream& jsonStream = SYSTEMS::GetObject().SCENE.GetJSONStreamWorkBuffer();

		//building a file name;
		strcpy(filePath, SERIALIZATION_FOLDER );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1, m_prefabMemFile.c_str(), GAMEOBJECT_MAX_NAME_LENGTH );
		strncpy(filePath+sizeof(SERIALIZATION_FOLDER)-1+m_prefabMemFile.size(), SERIALIZATION_FILE_EXT, sizeof(SERIALIZATION_FILE_EXT));

		FILE * pFile = fopen (filePath,"rb");

		if( pFile==NULL )
		{
			log::error << "Could not open file: " << filePath << std::endl;
			return;
 		}
		
		fseek(pFile, 0L, SEEK_END); 
		auto fileSize = ftell(pFile); 
		fseek(pFile, 0L, SEEK_SET); 

		jsonStream.resize(fileSize);
		fread(jsonStream.c_str(), 1, fileSize, pFile);
		jsonStream.OverrideWriteCursorPos(fileSize);

 		jsonStream >> *v_children[0];

		fclose (pFile);

		jsonStream.clear();
	}

	void GameObjectLoader::Serialize(bfu::JSONStream& stream)
	{
		
	}
	void GameObjectLoader::Deserialize(bfu::JSONStream& stream)
	{
		
	}

}