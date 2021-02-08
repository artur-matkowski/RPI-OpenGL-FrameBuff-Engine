#ifndef H_GameObjectViewWindow
#define H_GameObjectViewWindow

namespace asapgl
{
	class GameObject;

	class GameObjectViewWindow
	{
		GameObjectViewWindow();
		~GameObjectViewWindow();
	public:
		static void SetSelectedGameObject( GameObject* );
		static void OnGUI();

	};
}

#endif