#ifndef H_PersistanceSystem
#define H_PersistanceSystem
#include "bfu.hpp"
#include "UniqueID.hpp"
#include <map>
#include <vector>
#include <string>

namespace asapi
{
	using bfu::string;


	class AssetInfo: public  bfu::SerializableClassBase<AssetInfo>
	{
	public:
		SERIALIZABLE_OBJ(AssetInfo, UniqueID, m_assetID);

		std::string 	m_assetDisplayName;
		string 			m_resourcePath;

		AssetInfo(){};
		AssetInfo(const char* path);
	};
	

	class PersistanceSystem
	{
		std::string  m_projectPath;
		const char* 		m_assetExtension = ".json.asset";

		std::map< std::string, std::vector<AssetInfo> > m_assetsTypeToAssetInfoMap;

		int FindAssetByName(const char* assetType, const char* displayname);

	public:
		void SetProjectPath( const char* path );

		bool LoadAsset(const char* assetType, const UniqueID& id, bfu::SerializerBase* out);

		void CleanUp();


		#ifdef IS_EDITOR
	public:
		bool SaveAsset(const char* assetType, const UniqueID& id, bfu::SerializerBase& in);

		UniqueID CreateAsset(const char* assetType, const char* defaultDisplayedName);
		void DestroyAsset(const char* assetType, const UniqueID& id);

		void RefreshResources();
		void OnGUI();
		void OnGUI_select(const char* assetType, const UniqueID& currentID, UniqueID* newID);
		#endif
	};


}

#endif