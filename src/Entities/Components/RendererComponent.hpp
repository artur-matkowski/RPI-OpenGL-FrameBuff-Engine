#ifndef _H_MaterialComponent
#define _H_MaterialComponent
#include "ComponentBase.hpp"
#include "MaterialType.hpp"
#include "MeshComponent.hpp"
#include "ResourcePtr.hpp"
#include "MaterialReference.hpp"

namespace asapi
{
	using bfu::stream;

	class RendererComponent: public ComponentBase<RendererComponent>
	{
		SERIALIZABLE_OBJ(RendererComponent, MaterialReference,
													m_incomingMaterialImpl);

		MeshComponent*								p_meshComponent;
		glm::mat4* 									p_modelViewMat;

	public:
		RendererComponent(bfu::MemBlockBase* mBlock);
		~RendererComponent(){};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnIsDirty() override;

		virtual void PreSerializationCallback() override;
		virtual void PostDeserializationCallback() override;
	
	
		void Render(glm::mat4* projectionMatrix, glm::mat4* viewMatrix);
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif

	};
}

#endif