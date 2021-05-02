#ifndef H_MeshComponent
#define H_MeshComponent
#include "ComponentBase.hpp"
#include "ResourcePtr.hpp"
#include "Mesh.hpp"
#include "glm.hpp"


namespace asapi
{
	using bfu::stream;

	//this component MUST be added BEFORE  RendererComponent, therefore it is done autmaticly
	class MeshComponent: public ComponentBase<MeshComponent>
	{
	protected:
		SERIALIZABLE_VAR(MeshComponent, stream, m_meshName );

		char buffMesh[255];
		ResourcePtr< Mesh >		 		m_mesh;
		
	public:
		MeshComponent(bfu::MemBlockBase* mBlock);
		MeshComponent(const MeshComponent&) = delete;
		~MeshComponent(){};

		Mesh* GetMeshResource(){ return (Mesh*)m_mesh.GetRawPtr(); }


		//virtual void OnAttach() override;
		virtual void OnIsDirty() override;
		#ifdef IS_EDITOR
		virtual void OnGUI() override;
		#endif
	};
}

#endif