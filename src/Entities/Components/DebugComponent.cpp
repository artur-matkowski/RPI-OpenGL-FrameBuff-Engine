#include "DebugComponent.hpp"
#include "Systems.hpp"

namespace asapi
{
	DebugComponent::DebugComponent(bfu::MemBlockBase* mBlock)
		:ComponentBase<DebugComponent>(mBlock)
	{
		SYSTEMS::GetObject().RENDERER.debugComponent = this;
	}

	//virtual void OnAttach() override;
	void DebugComponent::OnIsDirty()
	{

	}
	#ifdef IS_EDITOR
	void DebugComponent::OnGUI()
	{

	}
	#endif


	void DebugComponent::Render()
	{
		log::debug << "logging from Render() on debug component()" << std::endl;
	}
}