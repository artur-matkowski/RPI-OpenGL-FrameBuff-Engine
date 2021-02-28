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

		Uniform<glm::mat4>*							p_modelViewMat;

	public:
		RendererComponent(bfu::MemBlockBase* mBlock);
		~RendererComponent(){};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
	
	
		void Render();
		
	};
}

#endif