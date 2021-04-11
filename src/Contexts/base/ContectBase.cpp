#include "ContextBase.hpp"
#include "Systems.hpp"
#include <EGL/egl.h>


namespace asapi{
	ContextBase::ContextBase( bfu::MemBlockBase* mBlock )
	{
		m_frameDelay = std::chrono::duration<double> (1.0/60.0);
		bfu::EventSystem& es = SYSTEMS::GetObject().EVENTS;
		
		p_ev_ResizeWindow 		= es.RegisterFastEvent("ResizeWindow", sizeof(ResizeWindowArgs), mBlock, false);
		#ifdef IS_EDITOR
		p_ev_MoveWindowArgs 	= es.RegisterFastEvent("MoveWindowArgs", sizeof(MoveWindowArgs), mBlock, false);
		#endif
		p_ev_MouseMoveEvent 	= es.RegisterFastEvent("MouseMoveEvent", sizeof(MouseMoveEvent), mBlock, false);
		p_ev_MouseClickEvent 	= es.RegisterFastEvent("MouseClickEvent", sizeof(MouseClickEvent), mBlock, false);
		p_ev_KeyboardEvent 		= es.RegisterFastEvent("KeyboardEvent", sizeof(KeyboardEvent), mBlock, false);
	}

	void ContextBase::SetFPS(double FPS)
	{
		m_frameDelay = std::chrono::duration<double> (1.0/FPS);
	}

}