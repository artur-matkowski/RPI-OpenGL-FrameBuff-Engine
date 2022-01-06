#include "MeshComponent.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "Systems.hpp"

namespace asapi
{
	MeshComponent::MeshComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<MeshComponent>(mBlock)
	{
	};

}