#pragma once
#include "WebSearchModule.h"
#include "Module.hpp"

// Определение класса
class WebSearchModule : public Module
{
public:
    // Конструктор
    WebSearchModule(Hotkey defaultHotkey, bool isYandex);

    // Перегруженные методы класса
    ModuleType GetType() const override;
    const wchar_t* GetName() const override;
    std::wstring Execute(std::wstring text) override;
    void ReadSettings(std::wistream* file) override;
    void WriteSettings(std::wostream* file) const override;

protected:
    // Поля класса
    bool isYandex;
};