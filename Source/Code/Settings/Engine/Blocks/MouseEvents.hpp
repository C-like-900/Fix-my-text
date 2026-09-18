#pragma once
#include "Event.h"
#include "MouseEvents.h"

// Определение класса
class MouseEvents
{
public:
    // Конструкторы
    MouseEvents(Event enter = nullptr, Event stay = nullptr, Event exit = nullptr, Event press = nullptr, Event hold = nullptr, Event release = nullptr, Event scroll = nullptr);
    MouseEvents(const MouseEvents& events);

    // Методы класса
    void ExecuteEvent(MouseEventType type, Block block, WPARAM wParam, LPARAM lParam) const;
    void SetEvent(MouseEventType type, Event event);

private:
    // Поля класса
    union
    {
        Event events[MOUSE_EVENTS_SIZE];
        struct
        {
            Event enter;
            Event stay;
            Event exit;
            Event press;
            Event hold;
            Event release;
			Event scroll;
        };
    };
};