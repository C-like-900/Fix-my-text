#pragma once
#include <Windows.h>
#include "Aliases.h"

// Магические значения
constexpr wchar_t LOG_DIRECTORY[] = L"Logs";
constexpr wchar_t LOG_FORMAT[] = L"log";
constexpr UDLONG LOG_LIFETIME = 10ULL * 24 * 60 * 60 * 10000000;

// Связанные типы данных
enum class LogType : BYTE { DEFAULT, COMMENT, ACTION, ACCEPTABLE_ERROR, FATAL_ERROR, OK };

// Предварительное объявление класса
class Logger;