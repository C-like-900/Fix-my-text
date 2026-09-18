#pragma once
#include "Logger.h"
#include <string>
#include "Application.h"

// Определение класса
class Logger
{
public:
    // Конструктор
    Logger(const Application* application);

	// Методы класса
    void Log(const wchar_t* message = L"", LogType type = LogType::DEFAULT, BYTE newLineCount = 0) const;

private:
    // Поля класса
	const Application* application;
    std::wstring logPath;

    // Приватные статические методы класса
    static UDLONG GetFileTimeTicks(FILETIME fileTime);
};