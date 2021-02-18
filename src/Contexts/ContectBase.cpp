#include "ContextBase.hpp"
#include "Systems.hpp"
#include <EGL/egl.h>


namespace asapi{
	ContextBase::ContextBase()
	{
		m_frameDelay = std::chrono::duration<double> (1.0/60.0);
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;
		
		events.InitEvent<ResizeWindowArgs>("ResizeWindow");
		#ifdef IS_EDITOR
		events.InitEvent<MoveWindowArgs>("MoveWindowArgs");
		#endif
		events.InitEvent<MouseMoveEvent>("MouseMoveEvent");
		events.InitEvent<MouseClickEvent>("MouseClickEvent");
		events.InitEvent<KeyboardEvent>("KeyboardEvent");
	}

	void ContextBase::SetFPS(double FPS)
	{
		m_frameDelay = std::chrono::duration<double> (1.0/FPS);
	}

}