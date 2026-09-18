#pragma once
#include "UrlTools.h"
#include <string>

// Определение класса
class UrlTools
{
public:
    // Статические методы класса
    static std::wstring Escape(const std::wstring& url);
    static void Open(const std::wstring& url);
    static std::wstring Request(const std::wstring& url, const std::wstring& headers = L"");
    static std::wstring FindTagValue(const std::wstring& json, const std::wstring& tag);
};