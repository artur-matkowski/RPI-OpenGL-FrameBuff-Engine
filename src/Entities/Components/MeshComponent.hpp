#ifndef H_MeshComponent
#define H_MeshComponent
#include "ComponentBase.hpp"
#include "ResourcePtr.hpp"
#include "Mesh_old.hpp"
#include "_ResourceProcessorsInclude.hpp"
#include "glm.hpp"


namespace asapi
{
	using bfu::stream;

	//this component MUST be added BEFORE  RendererComponent or PointerColider, therefore it is done autmaticly
	class MeshComponent: public ComponentBase<MeshComponent>
	{
	protected:

		SERIALIZABLE_OBJ(MeshComponent, ResourceFBXSharedReference, m_meshReference );
		
	public:
		MeshComponent(bfu::MemBlockBase* mBlock);
		MeshComponent(const MeshComponent&) = delete;
		~MeshComponent(){};

		inline void* GetMeshHandle() { return m_meshReference.GetRawHandle(); }
	};
}

#endif