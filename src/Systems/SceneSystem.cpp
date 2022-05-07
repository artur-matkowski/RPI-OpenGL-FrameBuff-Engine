#include "SceneSystem.hpp"
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

		Transform3D* trans = dynamic_cast<Transform3D*>(p_root->GET_COMPONENT( Transform3D ));
		*trans->GetModelMatrix() = glm::mat4(1.0f);
	}


	bfu::JSONSerializer& SceneSystem::GetJSONSerializer()
	{
		return m_jsonSerializer;
	}



	bool SceneSystem::File2JSON(bfu::JSONSerializer& jsonStream, const char* filePath)
	{
		::FILE * pFile = fopen (filePath,"rb");

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
		::FILE * pFile = fopen (filePath,"wb");

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

		::FILE * pFile = fopen (filePath,"rb");

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

		::FILE * pFile = fopen (filePath,"wb");

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

	bool SceneSystem::OpenProject()
	{
    	PrefabLoaderComponent* cmp = (PrefabLoaderComponent*) GetRootNode().GET_COMPONENT(PrefabLoaderComponent);
    	return cmp->Load_JSON(); 
	}
}