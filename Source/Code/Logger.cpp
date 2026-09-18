#include "Logger.hpp"
#include <format>
#include <fstream>
#include "Application.hpp"
#include "LogFrame.hpp"

// Конструктор
Logger::Logger(const Application* application) : application(application)
{
    // Получение текущего времени
    SYSTEMTIME time{};
    GetLocalTime(&time);

    // Создание директории журналов
    std::wstring logFolder = std::format(L"{}\\{}", *application->GetAppDataPath(), LOG_DIRECTORY);
    CreateDirectoryW(logFolder.c_str(), nullptr);

    // Получение пути из даты запуска приложения
    logPath = std::format(L"{}\\{:04}-{:02}-{:02}.{}", logFolder, time.wYear, time.wMonth, time.wDay, LOG_FORMAT);

    // Удаление лишних журналов
    std::wstring logsMask = std::format(L"{}\\*.{}", logFolder, LOG_FORMAT);

    WIN32_FIND_DATAW fileData{};
    HANDLE filesHandle = FindFirstFileW(logsMask.c_str(), &fileData);
    if (filesHandle != INVALID_HANDLE_VALUE)
    {
        // Получение максимального времени жизни
        FILETIME fileTime{};
        GetSystemTimeAsFileTime(&fileTime);
        UDLONG minFileTimeTicks = GetFileTimeTicks(fileTime) - LOG_LIFETIME;
        
        // Проход по всем журналам
        do
        {
            // Удаление старых журналов
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && minFileTimeTicks > GetFileTimeTicks(fileData.ftCreationTime))
                DeleteFileW(std::format(L"{}\\{}", logFolder, fileData.cFileName).c_str());
        } while (FindNextFileW(filesHandle, &fileData));

        // Закрытие дескриптора поиска
        FindClose(filesHandle);
    }
}

// Методы класса
void Logger::Log(const wchar_t* message, LogType type, BYTE newLineCount) const
{
	// Проверка на пустое сообщение
    if (!message || *message == L'\0')
		return;
  
	// Выбор рамки для сообщения
    LogFrame frame{};
    switch (type)
    {
    case LogType::COMMENT:          frame = LogFrame{ L"// ", L"" };    break;
    case LogType::ACTION:           frame = LogFrame{ L"# ", L" #" };   break;
    case LogType::ACCEPTABLE_ERROR: frame = LogFrame{ L"[", L"]" };     break;
    case LogType::FATAL_ERROR:      frame = LogFrame{ L"[[[", L"]]]" }; break;
    case LogType::OK:               frame = LogFrame{ L"< ", L" >" };   break;
    default:                                                            break;
    }

    // Получение текущего времени
    SYSTEMTIME time{};
    GetLocalTime(&time);

    // Открытие файла для записи
    std::wofstream file;
    file.open(logPath, std::ios::app);
    if (!file)
        return;

    // Запись результата в файл
	file << std::format(L"{:02}:{:02}:{:02}:{:03}\t{}{}{}\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, frame.GetStart(), message, frame.GetEnd());
    for (BYTE i = 0; i != newLineCount; i++)
        file << L"\n";
}

// Приватные статические методы класса
UDLONG Logger::GetFileTimeTicks(FILETIME fileTime)
{
    // Преобразование типов
    ULARGE_INTEGER maxFileTime;
    maxFileTime.LowPart = fileTime.dwLowDateTime;
    maxFileTime.HighPart = fileTime.dwHighDateTime;

    // Возврат тиков
    return maxFileTime.QuadPart;
}
