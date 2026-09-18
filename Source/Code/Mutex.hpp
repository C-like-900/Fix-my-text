#pragma once
#include "Mutex.h"
#include <Windows.h>

// Определение класса
class Mutex
{
public:
    // Конструктор и деструктор
    Mutex();
    ~Mutex();

private:
    // Поля класса
    HANDLE mutexHandle;
};