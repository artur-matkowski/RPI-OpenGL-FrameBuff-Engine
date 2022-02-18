#ifndef H_MaterialsSystem
#define H_MaterialsSystem

#include <vector>
#include "MaterialType.hpp"
#include "MaterialReference.hpp"
#include "UniqueID.hpp"


namespace asapi
{
	class MaterialsSystem
	{
		static std::string s_projectPath;

		std::vector<MaterialReference> 		m_materialsReference;
		std::vector<UniqueID>  				m_materialTypesIds;

	public:
		MaterialReference GetMaterialReference(const UniqueID& id);
		void DispouseMaterialReference( const MaterialReference& matRef );

		void SetProjectPath( const std::string& path );
		void RefreshResources();

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}

#endif