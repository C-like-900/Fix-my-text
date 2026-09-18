#pragma once
#include <Windows.h>

// Магические значения
constexpr wchar_t APPDATA_ENVIRONMENT[] = L"%APPDATA%\\Fix My Text";
constexpr wchar_t TRAY_WINDOW_CLASS[] = L"FMT Tray Window";
constexpr wchar_t GITHUB_RELEASE_URL[] = L"https://github.com/C-like-900/Fix-my-text/releases/latest";
constexpr unsigned VERSION_CHECK_INTERVAL = 60 * 60 * 1000;
constexpr wchar_t VERSION[] = L"8.0";

// Связанные типы данных
enum class TimerID : unsigned { CHECK_NEW_VERSION = 1U };

// Предварительное объявление класса
class Application;