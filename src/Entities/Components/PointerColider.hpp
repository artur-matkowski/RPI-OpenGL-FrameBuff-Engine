#ifndef H_PointerColider
#define H_PointerColider
#include "ComponentBase.hpp"
#include "MeshComponent.hpp"
#include "glm.hpp"


namespace asapi
{
	class PointerColider: public ComponentBase<PointerColider>
	{
		MeshComponent*								p_meshComponent;
		glm::mat4* 									p_modelViewMat;

	public:
		PointerColider(bfu::MemBlockBase* mBlock);
		~PointerColider(){};


		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnIsDirty() override;
	
	
		void Render(glm::mat4* projectionMatrix);
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif
	};
}

#endif