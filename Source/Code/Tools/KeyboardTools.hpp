#pragma once
#include <Windows.h>
#include <initializer_list>

// Определение класса
class KeyboardTools
{
public:
    // Статические методы класса
    static bool EmulateInput(std::initializer_list<WORD> keys, bool isKeysUp);
};