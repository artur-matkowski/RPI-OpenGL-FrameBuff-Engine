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
		std::string 		m_materialName;
		UniqueID 			m_materialUuid;
	};

	class MaterialsSystem
	{
		static std::string s_projectPath;

		std::vector<MaterialReference> 			m_materialsReference;
		std::vector<MaterialInstance> 			m_materialTypes;

		#ifdef IS_EDITOR
		std::vector<MaterialInfo>				m_selectableMaterials;
		#endif


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