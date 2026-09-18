#include "MouseEvents.hpp"

// Конструкторы
MouseEvents::MouseEvents(Event enter, Event stay, Event exit, Event press, Event hold, Event release, Event scroll)
	: enter(enter),	stay(stay), exit(exit), press(press), hold(hold), release(release), scroll(scroll) {}
MouseEvents::MouseEvents(const MouseEvents& events) : enter(events.enter), stay(events.stay), exit(events.exit),
	press(events.press), hold(events.hold), release(events.release), scroll(events.scroll) {}

// Методы класса
void MouseEvents::ExecuteEvent(MouseEventType type, Block block, WPARAM wParam, LPARAM lParam) const
{
	Event event = events[static_cast<BYTE>(type)];
	if (event)
		event(block, wParam, lParam);
}
void MouseEvents::SetEvent(MouseEventType type, Event event) { events[static_cast<BYTE>(type)] = event; }