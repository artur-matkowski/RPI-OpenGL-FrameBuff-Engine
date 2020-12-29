#ifndef _H_EntityBase
#define _H_EntityBase
#include "Serializable.hpp"
#include "UniqueID.hpp"

namespace asapgl
{
	class EntityBase: public SERIALIZABLR_CLASS_BASE
	{
	protected:
		SERIALIZABLE_VAR( UniqueID ) m_ID;

	public:
		EntityBase(EntityBase* _this)
			:SERIALIZABLE_VAR_CONSTRUCTOR(m_ID, _this)
		{};
		~EntityBase(){};
	};
}

#endif