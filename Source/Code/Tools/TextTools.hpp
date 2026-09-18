#pragma once
#include "TextTools.h"
#include <string>

// Определение класса
class TextTools
{
public:
    // Статические методы класса
    static bool IsMostlyEnglish(const std::wstring& text, size_t size);
    static std::wstring StringToWstring(const std::string& text);
    static std::string WstringToString(const std::wstring& text);
    static std::wstring Decode(const std::wstring& text);
};