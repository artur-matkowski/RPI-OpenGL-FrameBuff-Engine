#include "GameObjectLoader.hpp"
#include "Systems.hpp"

namespace asapgl
{
	GameObjectLoader::GameObjectLoader( bfu::MemBlockBase* mBlock )
		:GameObject(mBlock)
		,m_prefabMemFile("m_prefabMemFile", this, mBlock)
	{
		m_prefabMemFile.reserve(GAMEOBJECT_MAX_NAME_LENGTH);
		b_isGameObjectLoader = true;

		m_child = (GameObject*)m_mBlock->allocate(1, sizeof(GameObject), alignof(GameObject));

		//TBD temporarly use parrent allocator
		m_child->Init(mBlock);
	}
	GameObjectLoader::~GameObjectLoader()
	{
		m_child->Dispouse();
		m_mBlock->deallocate(m_child, sizeof(GameObject));
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

 		jsonStream << *m_child;

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

 		jsonStream >> *m_child;

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