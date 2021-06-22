#ifndef H_AssetMetaDataSocket
#define H_AssetMetaDataSocket
#include "object.hpp"
#include "UniqueID.hpp"
#include "bfu.hpp"

namespace asapi
{
	using bfu::stream;

	class AssetMetaDataSocket: public bfu::SerializableClassBase<AssetMetaDataSocket>
	{
		SERIALIZABLE_OBJ( AssetMetaDataSocket, UniqueID, m_id );
		SERIALIZABLE_VAR( AssetMetaDataSocket, stream, m_assetFileName );
		char m_fileNameBuff[MAX_FILENAME_SIZE];
	public:

		AssetMetaDataSocket(bfu::MemBlockBase* mBlock);

		static bool IsDirty(const char* path);
		static std::string GetHash(const char* path);
	};
}

#endif