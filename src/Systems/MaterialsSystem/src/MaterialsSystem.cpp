
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "MaterialsSystem.hpp"
#include "File.hpp"
#include "ResourceTracker.hpp"


#define MATERIAL_ASSET_EXTENSION ".mat"
#define MATERIAL_DATA_EXTENSION ".mat.json"

namespace asapi
{
	std::string MaterialsSystem::s_projectPath;


	MaterialReference* MaterialsSystem::GetMaterialReference(const UniqueID& id)
	{
		MaterialReference* ret = nullptr;

		for(auto it = m_materialsReference.begin(); it!=m_materialsReference.end(); it++)
		{
			if( it->GetMaterialInstanceID() == id )
			{
				ret = &(*it);
			}
		}

		return ret;
	}

	void MaterialsSystem::DispouseMaterialReference( const MaterialReference& matRef )
	{

	}


	void MaterialsSystem::SetProjectPath( const std::string& path )
	{
		s_projectPath = path;
		RefreshResources();
	}

	void MirrorMaterialDataFilesFromAssetsMaterials(const std::vector<std::string>& materialAssetsPaths,
								 const std::string& projectPath )
	{
		FILE::MMAP materialAssetFile;
		std::vector<std::string> materialAssetUuids_str;
		std::vector<std::string> materialDataPaths;
		std::string materialDatasPath = projectPath + RESOURCE_BINARIES_DIR;
		materialAssetUuids_str.resize( materialAssetsPaths.size() );

		for(int i=0; i<materialAssetsPaths.size(); ++i)
		{
			std::string path = projectPath + std::string(ASSETS_DIR "/") + materialAssetsPaths[i];
			uint64_t uuid_raw;

			materialAssetFile.InitForRead( path.c_str() );

			const char* uuid_cstr = (const char*) materialAssetFile.Data();

			sscanf( uuid_cstr, "%llu", &uuid_raw );
			materialAssetFile.Close();
			materialAssetUuids_str[i] = std::to_string( uuid_raw );


			std::string materialDataPath = projectPath + RESOURCE_BINARIES_DIR + "/";
			materialDataPath += materialAssetUuids_str[i] + MATERIAL_DATA_EXTENSION;

			if( !FILE::FileExist( materialDataPath.c_str() ) )
			{
				FILE::Touch( materialDataPath.c_str() );
				std::string materialDatasPath = projectPath + RESOURCE_BINARIES_DIR;
				ListFiles( materialDataPaths, {MATERIAL_DATA_EXTENSION}, ListingStrategy::whitelist, materialDatasPath.c_str() );

				log::debug << "Touching material" << " --- " << materialDataPath.c_str() << std::endl;
			}
		}

		ListFiles( materialDataPaths, {MATERIAL_DATA_EXTENSION}, ListingStrategy::whitelist, materialDatasPath.c_str() );

		for(int i=0; i<materialDataPaths.size(); i++)
		{
			bool dataFileFound = false;
			for(int j=0; j<materialAssetUuids_str.size(); j++)
			{
				if( strstr( materialDataPaths[i].c_str(), materialAssetUuids_str[j].c_str() ) != nullptr )
				{
					dataFileFound = true;
				}
			}
			if( !dataFileFound )
			{
				std::string dataFilePath = projectPath 
							+ std::string(RESOURCE_BINARIES_DIR "/") 
							+ materialDataPaths[i].c_str();

				FILE::Remove( dataFilePath.c_str() );
				log::debug << "Removing material" << " --- " << dataFilePath.c_str() << std::endl;				
			}
		}
	}


	void MaterialsSystem::RefreshResources()
	{
		std::vector<std::string> materialAssetsPaths;
		//std::vector<std::string> materialDataPaths;
		std::string materialAssetsPath = s_projectPath + ASSETS_DIR;
		//std::string materialDatasPath = s_projectPath + RESOURCE_BINARIES_DIR;

		ListFiles( materialAssetsPaths, {MATERIAL_ASSET_EXTENSION}, ListingStrategy::whitelist, materialAssetsPath.c_str() );
		//ListFiles( materialDataPaths, {MATERIAL_DATA_EXTENSION}, ListingStrategy::whitelist, materialDatasPath.c_str() );


		MirrorMaterialDataFilesFromAssetsMaterials( materialAssetsPaths, s_projectPath );


	}

	#ifdef IS_EDITOR
	void MaterialsSystem::OnGUI()
	{
		if( ImGui::Button("Create new Material Type") )
		{
			FILE::STREAM file;
			int IntanceNameCounter = 0;
			UniqueID uuid;
			std::string uuidStr = std::to_string( uuid.ID() );

			std::string materialPath = s_projectPath + ASSETS_DIR + "/New Material" MATERIAL_ASSET_EXTENSION;


			while( FILE::FileExist( materialPath.c_str() ) )
			{
				IntanceNameCounter++;
				materialPath = s_projectPath + ASSETS_DIR + "/New Material ";
				materialPath += std::to_string(IntanceNameCounter) + MATERIAL_ASSET_EXTENSION;
			}


			file.InitForWrite( materialPath.c_str() );

			file.Write( uuidStr.c_str(), uuidStr.size() );

			RefreshResources();
		}
	}
	#endif
}