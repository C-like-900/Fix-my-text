#pragma once
#include "Tray.h"
#include "Application.h"

// Определение класса
class Tray
{
public:
    // Конструктор и деструктор
    Tray(const Application* application);
    ~Tray();

	// Методы класса
    void AddNewVersion();
    void Restart();
    void Show() const;
    void OpenGithub() const;

    // Методы обработки запросов
    void TrayProcessing(LPARAM lParam);
    void MenuProcessing(TrayButtons button);
    void TaskbarProcessing(unsigned message);

private:
    // Поля класса
    const Application* application;
    HICON icon;
    HICON baloonIcon;
    unsigned dataID;
    unsigned taskbarRestartMessage;

    // Приватные методы класса
    void AddButton(HMENU menu, unsigned flags, unsigned id, const wchar_t* text, unsigned bitmapId) const;
    void CreateData(NOTIFYICONDATAW* data) const;
};