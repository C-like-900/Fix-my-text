#pragma once
#include <Windows.h>

// Магические значения
const BYTE SCROLL_BAR_SIZE = 6U;
const BYTE SCROLL_BAR_RADIUS = 3U;
const BYTE SCROLL_BAR_OFFSET = 6U;
const BYTE SCROLL_BAR_FRAME_OFFSET = 9U;
const float SCROLL_BAR_PHANTOM_ALPHA = 0.25f;

// Предварительное объявление класса
class ScrollBarClass;

// Создание псевдонимов класса
using ScrollBar = ScrollBarClass*;