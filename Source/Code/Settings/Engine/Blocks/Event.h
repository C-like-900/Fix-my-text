#pragma once
#include <Windows.h>
#include "BlockClass.h"

// Переопределения
#define EVENT(name) [](Block name, WPARAM wParam, LPARAM lParam)

// Сокращения
using Event = void (*)(Block, WPARAM, LPARAM);