#ifndef H_StatsWindow
#define H_StatsWindow

namespace asapi
{
	class StatsWindow
	{
	public:
		#ifdef IS_EDITOR
		static void OnGUI();
		#endif
	};
}

#endif