#include "PersistanceObjectReference.hpp"
#include "Systems.hpp"

namespace asapi
{
	std::string  PersistanceObjectBase::s_projectPath;
	std::map<UniqueID, PersistanceObjectBase*>	
				 PersistanceObjectBase::s_persistanceObjectsMap;

	void PersistanceObjectBase::OnGUI_select( UniqueID* out_newID, const UniqueID& in_oldId, const char* asset_type)
	{
		SYSTEMS::GetObject().PERSISTANCE.OnGUI_select( out_newID, in_oldId, asset_type );
	}

	void PersistanceObjectBase::SetProjectPath(const char* path)
	{
		CleanUp();
		s_projectPath = path;
	}

	void PersistanceObjectBase::CleanUp()
	{
		for(auto it=s_persistanceObjectsMap.begin(); it!=s_persistanceObjectsMap.end(); it++)
		{
			delete it->second;
		}

		s_persistanceObjectsMap.clear();
	}
}