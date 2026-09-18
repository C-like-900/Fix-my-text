#include "ApiTranslateModule.hpp"
#include <format>
#include "TextTools.hpp"
#include "UrlTools.hpp"

// Конструктор
ApiTranslateModule::ApiTranslateModule(Hotkey defaultHotkey) : Module(defaultHotkey) {}

// Перегруженные методы класса
ModuleType ApiTranslateModule::GetType() const { return ModuleType::API_TRANSLATE; }
const wchar_t* ApiTranslateModule::GetName() const { return L"API-перевод"; }
std::wstring ApiTranslateModule::Execute(std::wstring text)
{
    // Определение результата
    std::wstring result{};

    // Определение языка
    bool isEnglish = TextTools::IsMostlyEnglish(text, text.size());
    
    // Формирование запроса
    std::wstring url = std::format(L"{}&id={:x}00000-0-0&lang={}&text={}", YANDEX_API, static_cast<unsigned long long>(time(nullptr) * 1000), 
        isEnglish ? L"en-ru" : L"ru-en", UrlTools::Escape(text));

    // Получение ответа от переводчика
    std::wstring response = UrlTools::Request(url);
    if (!response.empty())
    {
        // Поиск переведённого текста
        response = UrlTools::FindTagValue(response, L"text");
        if (!response.empty())
            result = TextTools::Decode(result);
    }
        
    // Уведомляем об ошибке
    if (result.empty())
		MessageBeep(MB_ICONWARNING);

    // Возврат текста
    return result;
}