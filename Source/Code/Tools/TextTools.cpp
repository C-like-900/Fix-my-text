#include "TextTools.hpp"
#include <Windows.h>

// Статические методы класса
bool TextTools::IsMostlyEnglish(const std::wstring& text, size_t size)
{
    // Подсчёт символов, принадлежащих каждой раскладке
    size_t english = 0, russian = 0;
    for (size_t i = 0; i != size; i++)
    {
        // Пропуск одинаковых символов
        if (wcschr(GENERAL_CHARS, text[i]))
            continue;

        // Увеличение нужного счётчика
        (wcschr(ENGLISH_CHARS, text[i]) ? english : russian)++;
    }

    // Возврат результата
    return english >= russian;
}
std::wstring TextTools::StringToWstring(const std::string& text)
{
    // Пропуск пустой строки
    if (text.empty())
        return {};

    // Получение размера
    int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), nullptr, 0);
    if (size <= 0)
        return {};

    // Преобразование типов
    std::wstring result(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), result.data(), size);
    return result;
}
std::string TextTools::WstringToString(const std::wstring& text)
{
    // Пропуск пустой строки
    if (text.empty())
        return {};

    // Получение размера
    int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), nullptr, 0, nullptr, nullptr);
    if (size <= 0)
        return {};

    // Преобразование типов
    std::string result(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, text.c_str(), static_cast<int>(text.size()), result.data(), size, nullptr, nullptr);
    return result;
}
std::wstring TextTools::Decode(const std::wstring& text)
{
    // Обратное преобразование символов
    std::wstring result;
    for (size_t i = 0; i != text.size(); i++)
    {
        // Ожидание слэша
        if (text[i] != L'\\' || i + 1 >= text.size())
        {
            result += text[i];
            continue;
        }

        // Преобразование символа
        switch (text[i + 1])
        {
        case L'"':  result += L'"';         break;
        case L'\\': result += L'\\';        break;
        case L'/':  result += L'/';         break;
        case L'b':  result += L'\b';        break;
        case L'f':  result += L'\f';        break;
        case L'n':  result += L'\n';        break;
        case L'r':  result += L'\r';        break;
        case L't':  result += L'\t';        break;
        default:    result += text[i + 1];  break;
        }

        // Пропуск символа 
        i++;
    }

    // Возврат результата
    return result;
}