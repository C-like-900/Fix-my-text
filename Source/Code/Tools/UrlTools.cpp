#include "UrlTools.hpp"
#include <Shlwapi.h>
#include <WinInet.h>
#include <format>
#include "TextTools.hpp"

// Статические методы класса
std::wstring UrlTools::Escape(const std::wstring& url)
{
    // Получение размера итогового буфера
    DWORD size = 1;
    DWORD flags = URL_ESCAPE_SEGMENT_ONLY | URL_ESCAPE_PERCENT | URL_ESCAPE_AS_UTF8;
    std::wstring result(size, L'\0');
    UrlEscapeW(url.c_str(), result.data(), &size, flags);

    // Преобразование строки
    result.resize(size);
    UrlEscapeW(url.c_str(), result.data(), &size, flags);
    return result;
}
void UrlTools::Open(const std::wstring& url) { ShellExecuteW(nullptr, nullptr, url.c_str(), nullptr, nullptr, SW_SHOWNORMAL); }
std::wstring UrlTools::Request(const std::wstring& url, const std::wstring& headers)
{   
    // Создание результата
    std::string result;

    // Открытие интернета
    HINTERNET internet = InternetOpenW(L"Fix My Text", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    if (internet)
    {
        // Установка таймаута ответа
        DWORD timeout = INTERNET_RECEIVE_TIMEOUT;
        InternetSetOptionW(internet, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));

        // Открытие ссылки
        HINTERNET connection = InternetOpenUrlW(internet, url.c_str(), headers.c_str(), static_cast<DWORD>(headers.size()) , INTERNET_FLAG_NO_UI | INTERNET_FLAG_NO_CACHE_WRITE, 0);
        if (connection)
        {
            // Поблочное чтение страницы
            char buffer[4096]{};
            DWORD byteRead = 0;
            while (InternetReadFile(connection, buffer, sizeof(buffer), &byteRead) && byteRead)
            {
                result.append(buffer, buffer + byteRead);
                byteRead = 0;
            }

            // Закрытие соединения
            InternetCloseHandle(connection);
        }
        
        // Закрытие интернета
        InternetCloseHandle(internet);
    }

    // Возврат результата
    return TextTools::StringToWstring(result);
}
std::wstring UrlTools::FindTagValue(const std::wstring& json, const std::wstring& tag)
{
    // Создание результата
    std::wstring result;

    // Поиск тэга
    std::wstring fullTag = std::format(L"\"{}\":", tag);
    size_t startID = json.find(fullTag);
    if (startID != std::wstring::npos)
    {
        // Поиск начала значения по тэгу
        startID += fullTag.size();
        startID = json.find('"', startID);
        if (startID != std::wstring::npos)
        {
            // Поиск конца значения по тэгу
            startID++;
            size_t endID = json.find('"', startID);
            if (endID != std::wstring::npos)
                result = json.substr(startID, endID - startID);
        }
    }
    
    // Возврат результата
    return result;
}