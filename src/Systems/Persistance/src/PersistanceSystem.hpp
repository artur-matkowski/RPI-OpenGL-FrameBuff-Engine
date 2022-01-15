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
		int FindAssetByID(const char* assetType, const UniqueID& id);
		
		static bool ResourceComboGetter(void* data, int idx, const char** out_text);

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
		void OnGUI_select( UniqueID* out_newID, const UniqueID& in_oldId, const char* asset_type);
		#endif

		static bool Serialize(const char* path, bfu::SerializableClassInterface* obj);
		static bool Deserialize(const char* path, bfu::SerializableClassInterface* obj);
	};


}

#endif