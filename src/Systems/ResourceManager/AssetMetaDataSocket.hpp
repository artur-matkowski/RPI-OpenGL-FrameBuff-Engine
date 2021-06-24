#ifndef H_AssetMetaDataSocket
#define H_AssetMetaDataSocket
#include "bfu.hpp"

namespace asapi
{
	using bfu::string;

	enum eAssetImportType{BrandNew, Moved, Updated, Untouched, Invalid};

	class AssetMetaDataSocket: public bfu::SerializableClassBase<AssetMetaDataSocket>
	{
		SERIALIZABLE_VAR( AssetMetaDataSocket, string, m_hash );
		SERIALIZABLE_VAR( AssetMetaDataSocket, string, m_assetPath );


	public:

		AssetMetaDataSocket();

		static eAssetImportType AssetImportState(const char* path);
		static string GetHash(const char* path);

		static void OnAssetMoved(const char* newPath, AssetMetaDataSocket * asset);
		static AssetMetaDataSocket OnAssetAdded(const char* path, const string & hash);
	};
}

#endif