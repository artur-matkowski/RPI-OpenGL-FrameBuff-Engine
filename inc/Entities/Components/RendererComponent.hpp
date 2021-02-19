#ifndef _H_MaterialComponent
#define _H_MaterialComponent
#include "ComponentBase.hpp"
#include "MaterialType.hpp"
#include "Mesh.hpp"
#include "ResourcePtr.hpp"

namespace asapi
{
	class RendererComponent: public ComponentBase<RendererComponent>
	{
		//bfu::SerializableVar< bfu::string > 		m_usedMaterial;
		//bfu::SerializableVar< bfu::string >	 		m_usedMesh;

		ResourcePtr< MaterialType > 				m_material;
		ResourcePtr< Mesh >		 					m_mesh;



	public:
		RendererComponent(bfu::MemBlockBase* mBlock);
		~RendererComponent(){};


		void Init( bfu::MemBlockBase* mBlock )
		{
			new (this) RendererComponent(mBlock);
		};
	
		inline void Render()
		{
			m_material->BindMaterial();
			m_mesh->Render();
		};
		
	};
}

#endif