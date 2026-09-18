#pragma once
#include <Windows.h>
#include "Hotkey.hpp"

// Магические значения
constexpr USHORT CLIPBOARD_COPY_ATTEMPTS_COUNT = 30U;
constexpr USHORT CLIPBOARD_PASTE_TIMEOUT = 350U;
constexpr wchar_t MODULES_FILE[] = L"Settings";
const Hotkey SWAP_LAYOUT_HOTKEY = Hotkey{ MOD_SHIFT | MOD_WIN, 'Z' };
const Hotkey INVERT_CASE_HOTKEY = Hotkey{ MOD_SHIFT | MOD_WIN, 'X' };
const Hotkey WEB_SEARCH_HOTKEY = Hotkey{ MOD_CONTROL | MOD_WIN, 'Z' };
const Hotkey WEB_TRANSLATE_HOTKEY = Hotkey{ MOD_CONTROL | MOD_WIN, 'X' };
const Hotkey API_TRANSLATE_HOTKEY = Hotkey{ MOD_SHIFT | MOD_CONTROL | MOD_WIN, 'Z' };

// Предварительное объявление класса
class ModuleManager;