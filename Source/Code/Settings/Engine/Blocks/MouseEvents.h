#pragma once
#include <Windows.h>

// Магические значения
const BYTE MOUSE_EVENTS_SIZE = 7;

// Связанные типы данных
enum class MouseEventType : BYTE { ENTER, STAY, EXIT, PRESS, HOLD, RELEASE, SCROLL };

// Предварительное объявление класса
class MouseEvents;