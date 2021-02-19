#ifndef H_Transform3D
#define H_Transform3D
#include "ComponentBase.hpp"

namespace asapi
{
	class Transform3D: public ComponentBase<Transform3D>
	{
	protected:
		
	public:
		Transform3D(bfu::MemBlockBase* mBlock)
			:ComponentBase<Transform3D>(mBlock)
		{};
		~Transform3D(){};


		
	};
}

#endif