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
		static std::string 						s_projectPath;

		std::vector<MaterialReference> 			m_materialsReference;

		#ifdef IS_EDITOR
		std::vector<MaterialInfo>				m_selectableMaterials;
		#endif


	public:
		bool UpdateMaterialReference(const UniqueID& id, MaterialReference* out);
		void DispouseMaterialReference( const MaterialReference& matRef );

		void SetProjectPath( const std::string& path );
		void RefreshResources();

		const char* GetMaterialNameByUuid(const UniqueID& uuid) const;



		#ifdef IS_EDITOR
		void OnGUI();

		void SelectMaterialReference( MaterialReference* materialReference );

		void SaveMaterialInstances();
		#endif
	};
}

#endif