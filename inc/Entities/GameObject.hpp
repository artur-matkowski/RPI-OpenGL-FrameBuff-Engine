#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"

namespace asapgl
{
	class GameObject: public EntityBase
	{
	protected:
		//SERIALIZABLE_VAR_VEC( UniqueID ) 	m_usedComponentsIDs;
		//SERIALIZABLE_VAR_VEC( UniqueID )	m_usedGameObjectsChildrensIDs;
		//SERIALIZABLE_VAR( UniqueID )		m_usedGameObjectParrentID;
		bfu::SerializableVarVector<GameObject*>			v_children;


	public:
		GameObject( bfu::MemBlockBase* mBlock )
			:EntityBase(mBlock)
			,v_children("v_children", this, mBlock)
		{}
		
		#ifndef _PLAYER
			virtual void Serialize(bfu::JSONStream& stream);
			virtual void Deserialize(bfu::JSONStream& stream);
		#endif
	};
}

#endif