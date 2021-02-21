#ifndef H_Transform3D
#define H_Transform3D
#include "ComponentBase.hpp"
#include "glm.hpp"

namespace asapi
{
	class Transform3D: public ComponentBase<Transform3D>
	{
	protected:
		SerializableArray<float>   m_position;
		SerializableArray<float>   m_rotation;
		SerializableArray<float>   m_scale;
	public:
		Transform3D(bfu::MemBlockBase* mBlock)
			:ComponentBase<Transform3D>(mBlock)
			,m_position("m_position", this, 3, mBlock)
			,m_rotation("m_rotation", this, 3, mBlock)
			,m_scale("m_scale", this, 3, mBlock)
		{};
		~Transform3D(){};


		
	};
}

#endif