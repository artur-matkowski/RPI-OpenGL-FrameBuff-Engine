#ifndef H_StatsWindow
#define H_StatsWindow
#include "Systems.hpp"


namespace asapi
{
	class StatsWindow
	{
		class Persistance: public bfu::SerializableClassBase
		{
			bfu::JSONStream my_stream;
		public:
			bfu::SerializableVar<bfu::string> m_lastOpenProject0;
			bfu::SerializableVar<bfu::string> m_lastOpenProject1;
			bfu::SerializableVar<bfu::string> m_lastOpenProject2;
			bfu::SerializableVar<bfu::string> m_lastOpenProject3;

			Persistance()
				:bfu::SerializableClassBase( SYSTEMS::SYSTEMS_ALLOCATOR )
				,m_lastOpenProject0("m_lastOpenProject0", this, SYSTEMS::SYSTEMS_ALLOCATOR )
				,m_lastOpenProject1("m_lastOpenProject1", this, SYSTEMS::SYSTEMS_ALLOCATOR )
				,m_lastOpenProject2("m_lastOpenProject2", this, SYSTEMS::SYSTEMS_ALLOCATOR )
				,m_lastOpenProject3("m_lastOpenProject3", this, SYSTEMS::SYSTEMS_ALLOCATOR )
			{
				m_lastOpenProject3.GetRef() = "...none";
				m_lastOpenProject2.GetRef() = "...none";
				m_lastOpenProject1.GetRef() = "...none";
				m_lastOpenProject0.GetRef() = "...none";

				if( SceneSystem::File2JSON(my_stream, ".persistance.json") )
				{
					my_stream >> *this;
				}
			}

			~Persistance()
			{}

			void Update(const char* path)
			{
				m_lastOpenProject3.GetRef() = m_lastOpenProject2.GetRef().c_str();
				m_lastOpenProject2.GetRef() = m_lastOpenProject1.GetRef().c_str();
				m_lastOpenProject1.GetRef() = m_lastOpenProject0.GetRef().c_str();
				m_lastOpenProject0.GetRef() = path;

				my_stream.clear();
				my_stream << *this;
				SceneSystem::JSON2File(my_stream, ".persistance.json");
			}

		};

		static char 	m_openedProjectPath[2048];
		static Persistance* persistance;

		#ifdef IS_EDITOR
		static void drawFileGui(const std::string& dummyDlgKey);
		#endif
	public:
		#ifdef IS_EDITOR
		static void OnGUI();
		#endif

		static void Init()
		{
			persistance = SYSTEMS::ALLOCATE<Persistance>(1);
			new (persistance) Persistance();
		}

		static bool OpenProject(const char* path);
	};
}

#endif