#pragma once
#include "WebTranslateModule.h"
#include "Module.hpp"

// Определение класса
class WebTranslateModule : public Module
{
public:
    // Конструктор
    WebTranslateModule(Hotkey defaultHotkey, bool isYandex);

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