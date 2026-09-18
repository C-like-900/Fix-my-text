#pragma once
#include <Windows.h>

// Связанные типы данных
enum class ModuleType : BYTE { SWAP_LAYOUT = 1U, INVERT_CASE, WEB_SEARCH, WEB_TRANSLATE, API_TRANSLATE };

// Предварительное объявление класса
class Module;