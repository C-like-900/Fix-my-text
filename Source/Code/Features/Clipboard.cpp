#include "Clipboard.hpp"
#include <format>
#include "Logger.hpp"

// Конструктор
Clipboard::Clipboard() : format(0), bytes() {}

// Статические методы класса
bool Clipboard::Open(HWND window)
{
    // Попытки открыть буфер обмена
    for (BYTE i = 0; i != OPENING_ATTEMPTS_COUNT; i++)
    {
        // Попытка получить доступ к буферу обмена
        if (OpenClipboard(window))
            return true;

        // Небольшая пауза перед повторной попыткой
        Sleep(USER_TIMER_MINIMUM);
    }

    // Буфер обмена занят
    return false;
}
bool Clipboard::Close() { return static_cast<bool>(CloseClipboard()); };
void Clipboard::Clear()
{
    // Открытие буфера обмена
    if (!Open(nullptr))
        return;

    // Очистка буфера обмена
    EmptyClipboard();

    // Закрытие буфера обмена
    Close();
}

// Методы класса
unsigned Clipboard::GetFormat() const { return format; }
size_t Clipboard::GetSize() const { return bytes.size(); }
size_t Clipboard::GetLength() const
{
    // Игнорирование не текста
    if (format != CF_UNICODETEXT)
        return 0;

    // Возврат длинны текста
    return bytes.size() / sizeof(wchar_t);
}
const std::wstring Clipboard::GetText() const
{
    // Игнорирование не текста
    if (format != CF_UNICODETEXT)
        return {};

    // Возврат текста
    return reinterpret_cast<const wchar_t*>(bytes.data());
}
bool Clipboard::IsEmpty() const { return bytes.empty(); }
void Clipboard::Read(HWND window)
{
    // Открытие буфера обмена
    if (!Open(window))
        return;
    
    // Очитска старых данных
    format = 0;
    bytes.clear();

    // Получение формата данных в буфере обмена
    if (IsClipboardFormatAvailable(CF_UNICODETEXT))
        format = CF_UNICODETEXT;
    else if (IsClipboardFormatAvailable(CF_DIBV5))
        format = CF_DIBV5;
    else if (IsClipboardFormatAvailable(CF_DIB))
        format = CF_DIB;

    // Чтение только при известном формате
    if (format)
    {
        // Получение данных из буфера обмена 
        HANDLE handle = GetClipboardData(format);
        if (handle)
        {
            // Получение размера
            SIZE_T byteSize = GlobalSize(handle);
            if (byteSize)
            {
                // Блокировка данных
                const void* locked = GlobalLock(handle);
                if (locked)
                {
                    // Копирование байтов в вектор
                    bytes.resize(byteSize);
                    memcpy(bytes.data(), locked, byteSize);
                    GlobalUnlock(handle);
                }
            }
        }
    }

    // Закрытие буфера обмена
    CloseClipboard();
}
void Clipboard::Write(HWND window)
{
    // Открытие буфера обмена
    if (!Open(window))
        return;

    // Подготовка данных до очистки буфера обмена
    HGLOBAL handle = nullptr;
    if (!bytes.empty())
    {
        // Выделение памяти в буфере обмена
        handle = GlobalAlloc(GMEM_MOVEABLE, bytes.size());
        if (handle)
        {
            // Блокировка данных
            void* locked = GlobalLock(handle);
            if (locked)
            {
                // Копирование байтов в буфер обмена
                memcpy(locked, bytes.data(), bytes.size());
                GlobalUnlock(handle);
            }
            else
            {
                GlobalFree(handle);
                CloseClipboard();
                return;
            }
        }
        else
        {
            CloseClipboard();
            return;
        }
    }

    // Очистка буфера обмена
    EmptyClipboard();

    // Установка новых данных
    if (handle && SetClipboardData(format, handle))
    {
        handle = nullptr;
        bytes.clear();
    }

    // Очистка данных при неудаче
    if (handle)
        GlobalFree(handle);

    // Закрытие буфера обмена
    CloseClipboard();
}
void Clipboard::Log(const Logger* logger, const wchar_t* message)
{
    // Создание сообщения
    std::wstring logMessage = std::format(L"{}\t\t", message, bytes.size());

    // 
    if (bytes.empty())
        logMessage += L"* НИЧЕГО *";
    else if (format != CF_UNICODETEXT)
        logMessage = std::format(L"{}Объём: {} байт", logMessage, bytes.size());
    else
    {
        // Получение части строки
        std::wstring dataText = GetText();
        if (dataText.size() > MAX_PATH)
        {
            dataText.resize(MAX_PATH - 3);
            dataText += L"...";
        }

        // Формирование итоговой строки
        logMessage = std::format(L"{}Объём: {} символов -> {}", logMessage, GetLength(), dataText);
    }

    // Запись в журнал
    logger->Log(logMessage.c_str(), LogType::DEFAULT);
}
void Clipboard::SetText(const std::wstring text)
{
    // Установка текстового формата
    format = CF_UNICODETEXT;

    // Копирование текста
    bytes.resize((text.size() + 1) * sizeof(wchar_t));
    memcpy(bytes.data(), text.c_str(), bytes.size());
}