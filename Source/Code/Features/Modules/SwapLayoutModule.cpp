#include "SwapLayoutModule.hpp"
#include "TextTools.hpp"

// Конструктор
SwapLayoutModule::SwapLayoutModule(Hotkey defaultHotkey, bool needChangeLanguage) : Module(defaultHotkey), needChangeLanguage(needChangeLanguage) {}

// Перегруженные методы класса
ModuleType SwapLayoutModule::GetType() const { return ModuleType::SWAP_LAYOUT; }
const wchar_t* SwapLayoutModule::GetName() const { return L"Смена раскладки"; }
std::wstring SwapLayoutModule::Execute(const std::wstring text)
{
    // Создание результата
    std::wstring result = text;

    // Обработка текста по словам, чтобы смешанный текст менялся корректнее
    size_t wordStart = 0;
    for (size_t i = 0; i <= result.size(); i++)
    {
        // Поиск символов разделителей
        if (i != result.size() && result[i] != L' ' && result[i] != L'\t' && result[i] != L'\n')
            continue;

        // Пословная смена
        size_t wordSize = i - wordStart;
        if (wordSize)
        {
            // Получение слова
            wchar_t* word = result.data() + wordStart;
            bool isMostlyEnglish = TextTools::IsMostlyEnglish(word, wordSize);

            // Определение алфавитов
            const wchar_t* charsIn = isMostlyEnglish ? ENGLISH_CHARS : RUSSIAN_CHARS;
            const wchar_t* charsOut = isMostlyEnglish ? RUSSIAN_CHARS : ENGLISH_CHARS;

            // Посимвольная смена алфавита
            for (size_t j = 0; j != wordSize; j++)
            {
                const wchar_t* position = wcschr(charsIn, word[j]);
                if (position)
                    word[j] = charsOut[position - charsIn];
            }
        }

        // Определение метки старта слова
        wordStart = i + 1;
    }

    // Автопереключение раскладки
    if (needChangeLanguage)
        PostMessageW(GetForegroundWindow(), WM_INPUTLANGCHANGEREQUEST, INPUTLANGCHANGE_FORWARD, 0);

    // Возврат изменённого результата
    return result;
}
void SwapLayoutModule::ReadSettings(std::wistream* file)
{
    // Базовое чтение параметров
    Module::ReadSettings(file);

    // Чтение настройки автопереключения раскладки
    bool needChangeLanguage = false;
    if (*file >> needChangeLanguage)
        this->needChangeLanguage = needChangeLanguage;
}
void SwapLayoutModule::WriteSettings(std::wostream* file) const
{
    // Базовая запись параметров
    Module::WriteSettings(file);

    // Запись настройки автопереключения раскладки
    *file << L' ' << needChangeLanguage;
}