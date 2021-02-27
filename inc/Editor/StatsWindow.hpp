#ifndef H_StatsWindow
#define H_StatsWindow

namespace asapi
{
	class StatsWindow
	{
		static char 	m_openedProjectPath[2048];
	public:
		#ifdef IS_EDITOR
		static void OnGUI();
		#endif
	};
}

#endif