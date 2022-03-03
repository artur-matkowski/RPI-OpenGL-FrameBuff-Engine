#ifndef H_MaterialsSystem
#define H_MaterialsSystem

#include <vector>
#include "MaterialInstance.hpp"
#include "MaterialReference.hpp"
#include "UniqueID.hpp"


namespace asapi
{

	struct MaterialInfo
	{
		std::string 		m_materialPath;
		UniqueID 			m_materialuuid;
	};

	class MaterialsSystem
	{
		static std::string s_projectPath;

		std::vector<MaterialReference> 			m_materialsReference;
		std::vector<MaterialInstance> 			m_materialTypes;


	public:
		MaterialReference* GetMaterialReference(const UniqueID& id);
		void DispouseMaterialReference( const MaterialReference& matRef );

		void SetProjectPath( const std::string& path );
		void RefreshResources();



		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif