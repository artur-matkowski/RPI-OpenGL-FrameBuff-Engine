#include "ResourceSystem.hpp"

namespace asapi{

	void ResourceSystem::Init(const int argc, const char** argv)
	{
		const char* path = "--wrong path--";

		bool startWithJSON = false;
		for(int i=1; i<argc; ++i)
		{
			#ifdef IS_PLAYER
			if( strcmp(argv[i], "-json") == 0 )
			{
				startWithJSON = true;
			}
			else
			#endif
			if( strcmp(argv[i], "-path") == 0 )
			{
				if(argc > i) // if there is next argument
				{
					SetProjectPath(argv[i+1]);
					path = argv[i+1];
				}
				else
				{
					log::error << "You did not suplied path for project. Run '?' fur help." << std::endl;
				}
			}
		}
		#ifdef IS_PLAYER
		if( !(startWithJSON ? OpenProject() : LoadRootMMP() ) )
		{
			log::error << "Could not open project at path " << path << std::endl;
		}
		#endif
	}
	void ResourceSystem::SetProjectPath(const char* path)
	{
		strncpy(m_ProjectPath, path, MAX_PATH );
		m_ProjectPath[MAX_PATH-1] = '\0';
	}

}