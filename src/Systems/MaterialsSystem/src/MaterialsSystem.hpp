#include <map>
#include "MaterialType.hpp"
#include "MaterialReference.hpp"
#include "UniqueID.hpp"


namespace asapi
{
	class MaterialsSystem
	{
		std::map<UniqueID, MaterialReference> m_uuid2matRefMap;
		static std::string s_projectPath;

	public:
		MaterialReference GetMaterialReference(const UniqueID& id);


		void SetProjectPath( const std::string& path );
		void RefreshResources();

		#ifdef IS_EDITOR
		void OnGUI();
		#endif
	};
}
