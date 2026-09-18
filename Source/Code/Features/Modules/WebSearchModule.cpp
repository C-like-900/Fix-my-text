#include "WebSearchModule.hpp"
#include <format>
#include "UrlTools.hpp"

// Конструктор
WebSearchModule::WebSearchModule(Hotkey defaultHotkey, bool isYandex) : Module(defaultHotkey), isYandex(isYandex) {}

// Перегруженные методы класса
ModuleType WebSearchModule::GetType() const { return ModuleType::WEB_SEARCH; }
const wchar_t* WebSearchModule::GetName() const { return L"Поиск в интернете"; }
std::wstring WebSearchModule::Execute(std::wstring text)
{
	// Преобразование текста в запрос
	UrlTools::Open(std::format(L"{}{}", isYandex ? YANDEX_SEARCH : GOOGLE_SEARCH, UrlTools::Escape(text.c_str())).c_str());

	// Возврат пустого значения
	return {};
}
void WebSearchModule::ReadSettings(std::wistream* file)
{
	// Базовое чтение параметров
	Module::ReadSettings(file);

	// Чтение настройки поисковика
	bool isYandex = false;
	if (*file >> isYandex)
		this->isYandex = isYandex;
}
void WebSearchModule::WriteSettings(std::wostream* file) const
{
	// Базовая запись параметров
	Module::WriteSettings(file);

	// Запись настройки поисковика
	*file << L' ' << isYandex;
}