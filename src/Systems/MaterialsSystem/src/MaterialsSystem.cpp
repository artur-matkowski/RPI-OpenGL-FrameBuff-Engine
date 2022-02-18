
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "MaterialsSystem.hpp"
#include "File.hpp"

#define MATERIALS_DIR "/materials"

namespace asapi
{
	std::string MaterialsSystem::s_projectPath;


	MaterialReference MaterialsSystem::GetMaterialReference(const UniqueID& id)
	{

	}

	void MaterialsSystem::DispouseMaterialReference( const MaterialReference& matRef )
	{

	}


	void MaterialsSystem::SetProjectPath( const std::string& path )
	{
		s_projectPath = path;
	}

	void MaterialsSystem::RefreshResources()
	{
		m_materialTypesIds.clear();

		std::vector<std::string> filePaths;

		std::string materialsPath = s_projectPath + MATERIALS_DIR;

		ListFiles( filePaths, {".mat.json"}, ListingStrategy::whitelist, materialsPath.c_str() );

		for(int i=0; i<filePaths.size(); ++i)
		{
			const char* sID = filePaths[i].c_str();

			for( int s=filePaths[i].size()-1; s>=0; --s)
			{
				if( sID[s] == '\\' || sID[s]=='/' )
				{
					sID += (s+1);
					break;
				}
			}

			log::debug << filePaths[i] << " --- " << sID << std::endl;
		}
	}

	#ifdef IS_EDITOR
	void MaterialsSystem::OnGUI()
	{
		if( ImGui::Button("Create new Material Type") )
		{
			FILE::STREAM file;

			std::string materialPath = s_projectPath + MATERIALS_DIR + "/";
			materialPath += std::to_string(UniqueID()) + ".mat.json";


			file.InitForWrite( materialPath.c_str() );

			RefreshResources();
		}
		if( ImGui::Button("Create new Material Instance") )
		{
			//m_materialsReference.emplace_back( MaterialReference:: )
		}
	}
	#endif
}