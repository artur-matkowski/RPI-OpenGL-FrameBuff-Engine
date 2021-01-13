#include "Systems.hpp"
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include "Systems.hpp"


using namespace asapgl;


void signalHandler( int signum )
{
	SYSTEMS::GetObject().cloaseApp();
	//memoryRaport();
	//exit(0);
}

int main(const int argc, const char** argv)
{
	signal(SIGINT, signalHandler);  

	SYSTEMS::GetObject().init( argc, argv );

		bfu::CallbackId id;
		bfu::EventSystem& events = SYSTEMS::GetObject().EVENTS;


	    events.RegisterCallback<KeyboardEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    KeyboardEvent* args = (KeyboardEvent*)&a;
			log::debug << "KeyboardEvent: " << (int)args->m_key << "x" << (int)args->m_state  << std::endl;
	    });

	    events.RegisterCallback<MouseClickEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    MouseClickEvent* args = (MouseClickEvent*)&a;
			log::debug << "MouseClickEvent: " << (int)args->m_key << "x" << (int)args->m_state  << "  " <<  (int)args->m_Xpos << "x" << (int)args->m_Ypos  << std::endl;
	    });
/*
	    events.RegisterCallback<MouseMoveEvent>(id, [&](bfu::EventArgsBase& a)
	    {
		    MouseMoveEvent* args = (MouseMoveEvent*)&a;
			log::debug << "MouseMoveEvent: " <<  (int)args->m_Xpos << "x" << (int)args->m_Ypos  << std::endl;
	    });
*/
	SYSTEMS::GetObject().mainAppLoop();

    return 0;
}