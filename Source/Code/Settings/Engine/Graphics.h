#pragma once
#include <Windows.h>

// Магические значения
constexpr wchar_t SETTINGS_WINDOW_CLASS[] = L"FMT Settings Window";
constexpr wchar_t SETTINGS_WINDOW_NAME[] = L"Настройки FMT";

// Связанные типы данных
enum GraphicMessages : unsigned
{
	GM_DESTROY_BLOCK = WM_USER + 1,
	GM_UPDATE,
	GM_CREATE_ANIMATION,
	GM_ANIMATION
};

// Предварительное объявление класса
class Graphics;