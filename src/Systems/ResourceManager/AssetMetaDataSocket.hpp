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
		//SERIALIZABLE_VAR( AssetMetaDataSocket, UniqueID, m_id );
		//SERIALIZABLE_VAR( AssetMetaDataSocket, stream, m_assetFileName );
	protected:
	};
}

#endif