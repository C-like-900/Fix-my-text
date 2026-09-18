#pragma once
#include "Module.h"
#include <string>
#include <fstream>
#include "Hotkey.hpp"

// Определение класса
class Module
{
public:
    // Конструктор и деструктор
    Module(Hotkey defaultHotkey);
    virtual ~Module();

    // Методы класса
    Hotkey GetHotkey() const;
    bool IsEnable() const;
    void Enable(HWND window);
    void Disable(HWND window);
    void Register(HWND window);
    void Unregister(HWND window);
    void SetHotkey(HWND window, Hotkey hotkey);

    // Виртуальные методы класса
    virtual ModuleType GetType() const = 0;
    virtual const wchar_t* GetName() const = 0;
    virtual std::wstring Execute(const std::wstring text) = 0;
    virtual void ReadSettings(std::wistream* file);
    virtual void WriteSettings(std::wostream* file) const;

protected:
    // Поля класса
    Hotkey hotkey;
    bool isEnable;
};