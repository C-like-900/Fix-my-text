#pragma once
#include "Clipboard.h"
#include <string>
#include <vector>
#include "Logger.h"

// Определение класса
class Clipboard
{
public:
    // Конструктор
    Clipboard();

    // Статические методы класса
    static bool Open(HWND window);
    static bool Close();
    static void Clear();

    // Методы класса
    unsigned GetFormat() const;
    size_t GetSize() const;
    size_t GetLength() const;
    const std::wstring GetText() const;
    bool IsEmpty() const;

    void Read(HWND window);
    void Write(HWND window);
    void Log(const Logger* logger, const wchar_t* message);
    void SetText(const std::wstring text);

private:
    // Поля класса
    unsigned format;
    std::vector<BYTE> bytes;
};