#pragma once
#include <Windows.h>

// Определение класса
class Hotkey
{
public:
    // Конструктор
    Hotkey(unsigned modifiers = 0U, unsigned key = 0U);

    // Методы класса
    BYTE GetModifiers() const;
    WORD GetKey() const;
    bool IsRegistered() const;
    bool Register(HWND window, int id);
    void Unregister(HWND window, int id);

private:
    // Поля класса
    bool isRegistered;
    BYTE modifiers;
    WORD key;
};