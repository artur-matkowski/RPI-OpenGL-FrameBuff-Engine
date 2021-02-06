#ifndef _H_GameObjectLoader
#define _H_GameObjectLoader
#include "GameObject.hpp"
#include <bitforge/utils/bfu.hpp>

namespace asapgl
{
	class GameObjectLoader: public GameObject
	{
		bfu::SerializableVar<bfu::string>  	m_prefabMemFile;
		GameObject 							*m_child;

		virtual void RegisterChild(GameObject* newChild);
		virtual void UnRegisterChild(GameObject* deleteChild);

	public:
		GameObjectLoader( bfu::MemBlockBase* mBlock );
		~GameObjectLoader();


		void Init( bfu::MemBlockBase* mBlock );
		virtual void Dispouse();
		virtual void DispouseAndDeallocate();


		void SetPrefabMemFileName(const char* mmapFileName);


		bool Load();
		bool Save();

		virtual void Serialize();
		virtual void Deserialize();	
		virtual void Serialize(bfu::JSONStream& stream);
		virtual void Deserialize(bfu::JSONStream& stream);	

	};
}

#endif