#ifndef H_ASSETBASE
#define H_ASSETBASE
#include <vector>
#include <pair>
#include "buf.hpp"

namespace asapi
{
	template<class CRTP>
	class AssetBase
	{
		static std::vector< std::pair<CRTP*, uint16_t> > AssetReferenceList;
	public:
		static CRTP* RequestAssetReference(UniqueID id);
		static void DispouseAssetReference(CRTP* asset);

		static void SerializeAsset(CRTP* asset);

		static void CreateAsset(...);
		static void DestroyAsset(...);
	};


	template<class CRTP>
	std::vector< std::pair<CRTP*, uint16_t> > AssetBase<CRTP>::AssetReferenceList;
}

#endif