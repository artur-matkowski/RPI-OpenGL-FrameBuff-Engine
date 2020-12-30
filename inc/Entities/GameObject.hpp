#ifndef _H_GameObject
#define _H_GameObject
#include "EntityBase.hpp"

namespace asapgl
{
	class GameObject: public EntityBase
	{
	protected:
		SERIALIZABLE_VAR_VEC( UniqueID ) 	m_usedComponentsIDs;
		SERIALIZABLE_VAR_VEC( UniqueID )	m_usedGameObjectsChildrensIDs;
		SERIALIZABLE_VAR( UniqueID )		m_usedGameObjectParrentID;


	public:
		
		#ifndef _PLAYER
			virtual void Serialize(bfu::JSONStream& stream);
			virtual void Deserialize(bfu::JSONStream& stream);
		#endif

		virtual void Serialize(std::ofstream& stream);
		virtual void Deserialize(std::ifstream& stream);
	};
}

#endif