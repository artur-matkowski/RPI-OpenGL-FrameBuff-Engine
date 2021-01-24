#ifndef _H_EntityBase
#define _H_EntityBase
#include "UniqueID.hpp"

namespace asapgl
{
	class EntityBase: public object
	{
	protected:
		//SERIALIZABLE_VAR( UniqueID ) m_ID;

	public:
		EntityBase(EntityBase* _this)
			//:SERIALIZABLE_VAR_CONSTRUCTOR(m_ID, _this)
		{};
		~EntityBase(){};


	};
}

#endif