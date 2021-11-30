#ifndef H_AssetSystem
#define H_AssetSystem
#include <vector>
#include "ResourceTracker.hpp"


namespace asapi
{
	class AssetSystem
	{
		std::string						s_assetsDirectoryPath;
		bfu::JSONSerializer				p_JSONSerializer;
		bfu::BinarySerializer			p_BinarySerializer;
#ifdef TESTS
	public:
#endif
		std::vector<ResourceTracker> 	v_ResourceTrackers;
	public:
		void Init(const char* projectPath);
		void SetProjectPath(const char* projectPath);
		void RefreshResources();


		friend bfu::stream& operator<<(bfu::stream&, const AssetSystem& );
	};
}

#endif