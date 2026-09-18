#pragma once
#include <Windows.h>

// Магические значения
const BYTE RECT_SIZE = 4;

// Связанные типы данных
enum class SideID : BYTE { LEFT, TOP, RIGHT, BOTTOM };

// Предварительное объявление класса
class RectEx;