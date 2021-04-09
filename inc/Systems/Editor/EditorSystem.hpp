#ifndef _H_EditorSystem
#define _H_EditorSystem
#ifdef IS_EDITOR

namespace asapi
{
	class EditorSystem
	{		
	public:
		EditorSystem();
		~EditorSystem();

		void OnGUI();
		
	};
}
#endif
#endif