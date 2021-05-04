#ifndef _H_MaterialComponent
#define _H_MaterialComponent
#include "ComponentBase.hpp"
#include "MaterialType.hpp"
#include "MeshComponent.hpp"
#include "ResourcePtr.hpp"

namespace asapi
{
	using bfu::stream;

	class RendererComponent: public ComponentBase<RendererComponent>
	{
		ResourcePtr< MaterialType > 				m_material;

		MeshComponent*								p_meshComponent;
		UniformFloat4x4*							p_modelViewUniform;
		glm::mat4* 									p_modelViewMat;

		char buffMat[255];
		SERIALIZABLE_VAR(RendererComponent, stream, m_MaterialName );

	public:
		RendererComponent(bfu::MemBlockBase* mBlock);
		~RendererComponent(){};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnIsDirty() override;
	
	
		void Render(glm::mat4* projectionMatrix, glm::mat4* viewMatrix);
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif

	};
}

#endif