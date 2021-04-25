#ifndef _H_MaterialComponent
#define _H_MaterialComponent
#include "ComponentBase.hpp"
#include "MaterialType.hpp"
#include "Mesh.hpp"
#include "ResourcePtr.hpp"

namespace asapi
{
	using bfu::stream;

	class RendererComponent: public ComponentBase<RendererComponent>
	{
		bool 										m_isAttached = false;

		ResourcePtr< MaterialType > 				m_material;
		ResourcePtr< Mesh >		 					m_mesh;

		Uniform<glm::mat4>*							p_modelViewMat;

		char buffMat[255];
		char buffMesh[255];
		SERIALIZABLE_VAR(RendererComponent, stream, m_MaterialName );
		SERIALIZABLE_VAR(RendererComponent, stream, m_meshName );

	public:
		RendererComponent(bfu::MemBlockBase* mBlock);
		~RendererComponent(){};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnIsDirty() override;

		void SetMaterial_Blocking(const char*);
		void SetMesh_Blocking(const char*);
	
	
		void Render(glm::mat4* projectionMatrix);
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif

	};
}

#endif