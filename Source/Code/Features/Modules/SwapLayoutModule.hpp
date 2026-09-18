#pragma once
#include "Module.hpp"

// Определение класса
class SwapLayoutModule : public Module
{
public:
    // Конструктор
    SwapLayoutModule(Hotkey defaultHotkey, bool needChangeLanguage);

    // Перегруженные методы класса
    ModuleType GetType() const override;
    const wchar_t* GetName() const override;
    std::wstring Execute(const std::wstring text) override;
    void ReadSettings(std::wistream* file) override;
    void WriteSettings(std::wostream* file) const override;

protected:
    // Поля класса
    bool needChangeLanguage;
};