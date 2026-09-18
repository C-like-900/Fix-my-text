#pragma once
#include <Windows.h>

// Магические значения
constexpr wchar_t TRAY_TIP[] = L"Fix my text";
constexpr wchar_t TRAY_BALLOON_TITLE[] = L"Появилась новая версия приложения!";
constexpr wchar_t TRAY_BALLOON_BODY[] = L"Нажмите для перехода на страницу с новой версией приложения";

// Связанные типы данных
enum class TrayButtons : BYTE { SETTINGS = 1U, EXIT, NEW_VERSION };

// Предварительное объявление класса
class Tray;