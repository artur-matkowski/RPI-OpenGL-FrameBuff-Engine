#ifndef _H_EntityBase
#define _H_EntityBase
#include "UniqueID.hpp"

namespace asapgl
{
	class EntityBase: public object, public bfu::SerializableClassBase
	{
	protected:
		//SERIALIZABLE_VAR( UniqueID ) m_ID;

	public:
		EntityBase( bfu::MemBlockBase* mBlock )
			:bfu::SerializableClassBase(mBlock)
		{};
		~EntityBase(){};


	};
}

#endif