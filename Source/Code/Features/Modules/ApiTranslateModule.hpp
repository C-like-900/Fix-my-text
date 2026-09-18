#pragma once
#include "ApiTranslateModule.h"
#include "Module.hpp"

// Определение класса
class ApiTranslateModule : public Module
{
public:
    // Конструктор
    ApiTranslateModule(Hotkey defaultHotkey);

    // Перегруженные методы класса
    ModuleType GetType() const override;
    const wchar_t* GetName() const override;
    std::wstring Execute(std::wstring text) override;
};