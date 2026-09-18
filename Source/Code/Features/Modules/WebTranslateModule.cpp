#include "WebTranslateModule.hpp"
#include <format>
#include "UrlTools.hpp"

// Конструктор
WebTranslateModule::WebTranslateModule(Hotkey defaultHotkey, bool isYandex)	: Module(defaultHotkey), isYandex(isYandex) {}

// Перегруженные методы класса
ModuleType WebTranslateModule::GetType() const { return ModuleType::WEB_TRANSLATE; }
const wchar_t* WebTranslateModule::GetName() const { return L"Веб-перевод"; }
std::wstring WebTranslateModule::Execute(std::wstring text)
{
	// Преобразование текста в запрос
	UrlTools::Open(std::format(L"{}{}", isYandex ? YANDEX_TRANSLATE : GOOGLE_TRANSLATE, UrlTools::Escape(text.c_str())).c_str());

	// Возврат пустого значения
	return {};
}
void WebTranslateModule::ReadSettings(std::wistream* file)
{
	// Базовое чтение параметров
	Module::ReadSettings(file);

	// Чтение настройки переводчика
	bool isYandex = false;
	if (*file >> isYandex)
		this->isYandex = isYandex;
}
void WebTranslateModule::WriteSettings(std::wostream* file) const
{
	// Базовая запись параметров
	Module::WriteSettings(file);

	// Запись настройки переводчика
	*file << L' ' << isYandex;
}