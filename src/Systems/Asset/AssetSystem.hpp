#ifndef _H_AssetSystem
#define _H_AssetSystem
#include <vector>
#include <array>
#include "object.hpp"
#include "AssetMetaDataSocket.hpp"

namespace asapi
{
	class AssetSystem
	{
		std::vector< std::string > v_paths;
	public:
		
		void Init(const int argc, const char** argv);

		void RefreshAssets();
	};
}

#endif