#ifndef H_MeshComponent
#define H_MeshComponent
#include "ComponentBase.hpp"
#include "ResourcePtr.hpp"
#include "Mesh_old.hpp"
#include "glm.hpp"


namespace asapi
{
	using bfu::stream;

	//this component MUST be added BEFORE  RendererComponent or PointerColider, therefore it is done autmaticly
	class MeshComponent: public ComponentBase<MeshComponent>
	{
	protected:
		SERIALIZABLE_VAR(MeshComponent, stream, m_meshName );
		ResourcePtr< Mesh_old >		 		m_mesh;
		char buffMesh[255];
		
	public:
		MeshComponent(bfu::MemBlockBase* mBlock);
		MeshComponent(const MeshComponent&) = delete;
		~MeshComponent(){};

		Mesh_old* GetMeshResource(){ return (Mesh_old*)m_mesh.GetRawPtr(); }


		//virtual void OnAttach() override;
		virtual void OnIsDirty() override;
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif
	};
}

#endif