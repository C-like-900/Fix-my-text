#pragma once
#include "Module.hpp"

// Определение класса
class InvertCaseModule : public Module
{
public:
    // Конструктор
    InvertCaseModule(Hotkey defaultHotkey, bool needChangeCapslock);

    // Перегруженные методы класса
    ModuleType GetType() const override;
    const wchar_t* GetName() const override;
    std::wstring Execute(const std::wstring text) override;
    void ReadSettings(std::wistream* file) override;
    void WriteSettings(std::wostream* file) const override;

protected:
    // Поля класса
    bool needChangeCapslock;
};