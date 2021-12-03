#ifndef H_DebugComponent
#define H_DebugComponent
#include "ComponentBase.hpp"
#include "glm.hpp"
#include "Mesh.hpp"

namespace asapi
{
	class DebugComponent: public ComponentBase<DebugComponent>
	{
	protected:

		Mesh mesh;
	public:
		DebugComponent(bfu::MemBlockBase* mBlock);
		DebugComponent(const DebugComponent&) = delete;
		~DebugComponent(){};


		//virtual void OnAttach() override;
		virtual void OnIsDirty() override;
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif

		void Render();
	};
}

#endif