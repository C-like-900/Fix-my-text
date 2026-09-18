#pragma once
#include "Application.h"
#include <string>
#include "Logger.h"
#include "Tray.h"
#include "ModuleManager.h"
#include "Module.h"

// Определение класса
class Application
{
public:
    // Конструктор и деструктор
    Application(HINSTANCE instance);
    ~Application();

    // Методы класса
    HINSTANCE GetInstance() const;
    const std::wstring* GetAppDataPath() const;
    HWND GetWindow() const;
    bool IsNewVersionAvailable() const;
    const Logger* GetLogger() const;
    void OpenSettings() const;

    // Метод запуска приложения
    void Run();

private:
    // Поля класса
    HINSTANCE instance;
	std::wstring appdataPath;
    const Logger* logger;
    HWND window;
    Tray* tray;
    ModuleManager* moduleManager;
    bool isNewVersionAvailable;

    // Методы обработки запросов
    LRESULT ApplicationProcessing(HWND window, unsigned message, WPARAM wParam, LPARAM lParam);

    // Функция обратного вызова
    static LRESULT CALLBACK WindowCallback(HWND window, unsigned message, WPARAM wParam, LPARAM lParam);

    // Приватные методы класса
    bool CheckNewVersion();
    int FindVersionComponent(std::wstring& version);
};
