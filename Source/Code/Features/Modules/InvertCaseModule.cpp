#include "InvertCaseModule.hpp"
#include "KeyboardTools.hpp"

// Конструктор
InvertCaseModule::InvertCaseModule(Hotkey defaultHotkey, bool needChangeCapslock) : Module(defaultHotkey), needChangeCapslock(needChangeCapslock) {}

// Перегруженные методы класса
ModuleType InvertCaseModule::GetType() const { return ModuleType::INVERT_CASE; }
const wchar_t* InvertCaseModule::GetName() const { return L"Смена регистра"; }
std::wstring InvertCaseModule::Execute(const std::wstring text)
{
	// Создание результата
	std::wstring result = text;

	// Инвертирование каждого символа
	for (wchar_t& letter : result)
	{
		// Пропуск не буквенных символов
		if (!IsCharAlphaW(letter))
			continue;

		// Инверсия регистра
		IsCharUpperW(letter) ? CharLowerBuffW(&letter, 1) : CharUpperBuffW(&letter, 1);
	}
		
	// Автопереключение регистра
	if (needChangeCapslock)
	{
		KeyboardTools::EmulateInput({ VK_CAPITAL }, false);
		KeyboardTools::EmulateInput({ VK_CAPITAL }, true);
	}

	// Возврат изменённого результата
	return result;
}
void InvertCaseModule::ReadSettings(std::wistream* file)
{
	// Базовое чтение параметров
	Module::ReadSettings(file);

	// Чтение настройки автопереключения регистра
	bool needChangeCapslock = false;
	if (*file >> needChangeCapslock)
		this->needChangeCapslock = needChangeCapslock;
}
void InvertCaseModule::WriteSettings(std::wostream* file) const
{
	// Базовая запись параметров
	Module::WriteSettings(file);

	// Запись настройки автопереключения регистра
	*file << L' ' << needChangeCapslock;
}