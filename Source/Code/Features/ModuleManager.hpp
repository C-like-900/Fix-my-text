#pragma once
#include "ModuleManager.h"
#include <string>
#include <vector>
#include <initializer_list>
#include "Application.h"
#include "Module.hpp"

// Определение класса
class ModuleManager
{
public:
    // Конструктор и деструктор
    ModuleManager(const Application* application);
    ~ModuleManager();

    // Методы обработки запросов
    void HotkeyProcessing(ModuleType type);

private:
    // Поля класса
    const Application* application;
    std::wstring settingsPath;
    std::vector<Module*> modules;

    // Приватные методы класса
    void EmulateInput(std::initializer_list<WORD> keys, bool isKeysUp);
};
