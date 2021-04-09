#ifndef H_StatsWindow
#define H_StatsWindow
#include "Systems.hpp"


namespace asapi
{
	using bfu::string;

	class Persistance: public bfu::SerializableClassBase<Persistance> 
	{
	public:
		SERIALIZABLE_VAR( Persistance, string, m_lastOpenProject0 );
		SERIALIZABLE_VAR( Persistance, string, m_lastOpenProject1 );
		SERIALIZABLE_VAR( Persistance, string, m_lastOpenProject2 );
		SERIALIZABLE_VAR( Persistance, string, m_lastOpenProject3 );

		Persistance()
			:m_lastOpenProject0( SYSTEMS::SYSTEMS_ALLOCATOR )
			,m_lastOpenProject1( SYSTEMS::SYSTEMS_ALLOCATOR )
			,m_lastOpenProject2( SYSTEMS::SYSTEMS_ALLOCATOR )
			,m_lastOpenProject3( SYSTEMS::SYSTEMS_ALLOCATOR )
		{
			m_lastOpenProject3 = "...none";
			m_lastOpenProject2 = "...none";
			m_lastOpenProject1 = "...none";
			m_lastOpenProject0 = "...none";

		}



		~Persistance()
		{}

		void Update(const char* path)
		{
			m_lastOpenProject3 = m_lastOpenProject2;
			m_lastOpenProject2 = m_lastOpenProject1;
			m_lastOpenProject1 = m_lastOpenProject0;
			m_lastOpenProject0 = path;

			SceneSystem::JSON2File(this, ".persistance.json");
		}

	};


	class StatsWindow
	{
		static char 	m_openedProjectPath[2048];
		static Persistance persistance;

		#ifdef IS_EDITOR
		static void drawFileGui(const std::string& dummyDlgKey);
		#endif
	public:
		#ifdef IS_EDITOR
		static void OnGUI();
		#endif

		static void Init()
		{
			SceneSystem::File2JSON(&persistance, ".persistance.json");			
		}

		static bool OpenProject(const char* path);
	};
}

#endif