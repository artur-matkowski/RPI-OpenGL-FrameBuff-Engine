#include "PersistanceSystem.hpp"
#include "ResourceTracker.hpp"
#include "File.hpp"
#ifdef IS_EDITOR
#include "imgui.h"
#endif
#include "bfu.hpp"


namespace asapi
{
	AssetInfo::AssetInfo(const char* path)
		:m_resourcePath(path)
	{
		bfu::JSONSerializer serializer;

		FILE::MMAP file;
		file.InitForRead(path);

		serializer.assignData((char*)file.Data(), file.Size());

		serializer.Deserialize( this );

		constexpr int displayNameBuffSize = 128;
		char displayNameBuff[displayNameBuffSize];

		FILE::GetFileNameWithoutExtension(displayNameBuff, displayNameBuffSize, path, strlen(path));

		m_assetDisplayName = displayNameBuff;
	}

	bool PersistanceSystem::LoadAsset(const char* assetType, const UniqueID& id, bfu::SerializerBase* out)
	{
		return false;
	}

	void PersistanceSystem::CleanUp()
	{
		m_assetsTypeToAssetInfoMap.clear();
	}

	void PersistanceSystem::SetProjectPath( const char* path ) 
	{ 
		m_projectPath = path;
		CleanUp();
		RefreshResources();;
	}

	int PersistanceSystem::FindAssetByName(const char* assetType, const char* displayName)
	{
		auto vec = m_assetsTypeToAssetInfoMap.find( assetType );

		if( vec==m_assetsTypeToAssetInfoMap.end() )
			return -1;

		for(int i=0; i<vec->second.size(); ++i)
		{
			if( strstr( vec->second[i].m_assetDisplayName.c_str(), displayName) != nullptr )
			{
				return i;
			}
		}

		return -1;
	}

	int PersistanceSystem::FindAssetByID(const char* assetType, const UniqueID& id)
	{
		auto vec = m_assetsTypeToAssetInfoMap.find( assetType );

		if( vec==m_assetsTypeToAssetInfoMap.end() )
			return -1;

		for(int i=0; i<vec->second.size(); ++i)
		{
			if( vec->second[i].m_assetID.ID() == id.ID() )
			{
				return i;
			}
		}

		return -1;
	}


	#ifdef IS_EDITOR
	bool PersistanceSystem::SaveAsset(const char* assetType, const UniqueID& id, bfu::SerializerBase& in)
	{
		return false;
	}

	UniqueID PersistanceSystem::CreateAsset(const char* assetType, const char* defaultDisplayedName)
	{
		const UniqueID retID;

		std::string binarySubresourcePath = m_projectPath 
										+ RESOURCE_BINARIES_DIR "/" 
										+ std::to_string( retID.ID() ) 
										+ std::string( assetType ) 
										+ std::string( ".json" ) ;


		auto& it = m_assetsTypeToAssetInfoMap[ std::string(assetType) ];

		FILE::STREAM resourceFile, subresourceFile;
		bfu::JSONSerializer serializer;

		it.emplace_back();

		int assetIndex = FindAssetByName( assetType, defaultDisplayedName );
		int nameSuffix = 2;
		std::string displayName = defaultDisplayedName;

		while( assetIndex!=-1 )
		{
			displayName = std::string( defaultDisplayedName ) + " ";
			displayName += std::to_string( nameSuffix );
			assetIndex = FindAssetByName( assetType, displayName.c_str() );
			nameSuffix++;
		}

		it.back().m_assetID = retID;
		it.back().m_assetDisplayName = displayName.c_str();
		it.back().m_resourcePath = m_projectPath 
										+ ASSETS_DIR "/"
										+ displayName
										+ std::string( assetType ) 
										+ std::string( m_assetExtension ) ;

		serializer.Serialize( &it.back() );

		resourceFile.InitForWrite( it.back().m_resourcePath.c_str()  );
		resourceFile.Write( serializer.data(), serializer.size() );
		resourceFile.Close();

		subresourceFile.InitForWrite( binarySubresourcePath.c_str() );
		subresourceFile.Close();

		return retID;
	}
	void PersistanceSystem::DestroyAsset(const char* assetType, const UniqueID& id)
	{
		std::string binarySubresourcePath = m_projectPath 
										+ RESOURCE_BINARIES_DIR "/" 
										+ std::to_string( id.ID() ) 
										+ std::string( assetType ) 
										+ std::string( ".json" ) ;

		auto tmp = m_assetsTypeToAssetInfoMap.find( std::string(assetType) );

		if( tmp==m_assetsTypeToAssetInfoMap.end() )
		{
			log::warning << "Can not remove Resource file... Unknown type: " << assetType << std::endl;
			return;
		}

		auto& vec = tmp->second;


		for(auto it = vec.begin(); it!=vec.end(); it++)
		//for(int i=0; i<it->second.size(); i++)
		{
			if( strstr( it->m_resourcePath.c_str(), std::to_string( id.ID() ).c_str() ) != nullptr )
			{
				int resoult = unlink( it->m_resourcePath.c_str() );
				if( resoult!=0 ) log::warning << "Can not remove Resource file: " << it->m_resourcePath.c_str() << std::endl;
			
				resoult = unlink( binarySubresourcePath.c_str() );
				if( resoult!=0 ) log::warning << "Can not remove Binary Resource file: " << binarySubresourcePath.c_str() << std::endl;

				vec.erase(it);

				break;
			}
		}

	}


	void FindOutTypeAndID(char* assetTypeBuff
						, int buffSize
						, const char* data
						, int dataSize
						, const char* assetExtension)
	{
		const char* end = data + dataSize - strlen(assetExtension) - 1;

		const char* dot = end;
		const char* slash;
		char id_buff[128];

		for(; *dot!='.'; dot--);

		strncpy(assetTypeBuff, dot, end-dot+1);
		assetTypeBuff[end-dot+1] = '\0';
	}

	void PersistanceSystem::RefreshResources()
	{
		if( m_projectPath.size()==0 )
			return;

		std::vector< std::string > paths;

		ListFiles( paths
					, {m_assetExtension}
					, ListingStrategy::whitelist
					, (m_projectPath + ASSETS_DIR).c_str() );

		for(auto vec = m_assetsTypeToAssetInfoMap.begin(); vec!=m_assetsTypeToAssetInfoMap.end(); vec++)
		{
			for(auto asset = vec->second.begin(); ; asset++)
			{
				std::string ID = std::to_string( asset->m_assetID.ID() );

				for(auto path = paths.begin(); path!=paths.end(); path++)
				{
					if( strstr( path->c_str(), ID.c_str() )==0 )
					{
						DestroyAsset( vec->first.c_str(), asset->m_assetID );
					}
					else
					{
						asset->m_resourcePath = m_projectPath + ASSETS_DIR "/" + *path;
					}
				}

				if( asset==vec->second.end() )
					break;
			}
		}

		m_assetsTypeToAssetInfoMap.clear();
		for(auto reletivePath = paths.begin(); reletivePath!=paths.end(); reletivePath++)
		{
			char dataType[16];

			FindOutTypeAndID( dataType, 16, reletivePath->c_str(), reletivePath->size(), m_assetExtension );

			auto& it = m_assetsTypeToAssetInfoMap[ dataType ];

			std::string absolutePath = m_projectPath + ASSETS_DIR "/" + *reletivePath;

			it.emplace_back( absolutePath.c_str() );
		}
	}

	void PersistanceSystem::OnGUI()
	{
		ImGui::Text("PersistanceSystem:");

		for(auto it = m_assetsTypeToAssetInfoMap.begin(); it!=m_assetsTypeToAssetInfoMap.end(); it++)
		{
			ImGui::Text("\tType \"%s\":", it->first.c_str() );

			for(auto asset = it->second.begin(); asset!=it->second.end(); asset++)
			{
				ImGui::Text("\t%s", asset->m_assetDisplayName.c_str() );
				ImGui::Text("\t%s", asset->m_resourcePath.c_str() );
				ImGui::Text("\t%llu", asset->m_assetID.ID() );
				ImGui::Separator();
			}
			ImGui::Separator();
			ImGui::Separator();
		}
	}

	struct CombaData
	{
		const char* asset_type = 0;
		PersistanceSystem* persistanceSys = 0;
	};


	bool PersistanceSystem::ResourceComboGetter(void* data, int idx, const char** out_text)
	{
		CombaData* combodata = (CombaData*) data;

		auto vec = combodata->persistanceSys->m_assetsTypeToAssetInfoMap.find( combodata->asset_type );

		*out_text = vec->second[ idx ].m_assetDisplayName.c_str();

		return true;
	}

	void PersistanceSystem::OnGUI_select( UniqueID* out_newID, const UniqueID& in_oldId, const char* asset_type)
	{
		CombaData combodata;

		*out_newID = in_oldId;

		int currentIndex = FindAssetByID( asset_type, in_oldId );
		char text[512];
		snprintf(text, 512, "Select resource of type: %s", asset_type);
		combodata.asset_type = asset_type;
		combodata.persistanceSys = this;

		auto vec = m_assetsTypeToAssetInfoMap.find( asset_type );

		int size = vec!=m_assetsTypeToAssetInfoMap.end() ? vec->second.size() : 0;

		if( ImGui::Combo( text
						, &currentIndex
						, &PersistanceSystem::ResourceComboGetter
						, &combodata
						, size) )
		{
			*out_newID = vec->second[ currentIndex ].m_assetID;
		}
	}
	#endif


	bool PersistanceSystem::Serialize(const char* path, bfu::SerializableClassInterface* obj)
	{
		bfu::JSONSerializer json;
		FILE::STREAM file;

		file.InitForWrite(path);
		if( !file.IsValid() )
		{
			return false;
		}

		json.Serialize( obj );
		file.Write( json.data(), json.size() );
		file.Close();

		return true;
	}

	bool PersistanceSystem::Deserialize(const char* path, bfu::SerializableClassInterface* obj)
	{
		bfu::JSONSerializer json;
		FILE::MMAP file;

		if( !file.TryInitForRead(path) )
		{
			return false;
		}

		json.assignData( (char*)file.Data(), file.Size() );
		json.Deserialize( obj );
		file.Close();

		return true;
	}
}