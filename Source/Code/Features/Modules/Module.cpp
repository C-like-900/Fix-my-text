#include "Module.hpp"
#include <stdexcept>

// Конструктор и деструктор
Module::Module(Hotkey defaultHotkey) : hotkey(defaultHotkey), isEnable(true) {}
Module::~Module() {}

// Методы класса
Hotkey Module::GetHotkey() const { return hotkey; }
bool Module::IsEnable() const { return isEnable; }
void Module::Enable(HWND window)
{
	// Установка флага
	isEnable = true;

	// Регистрация хоткея
	Register(window);
}
void Module::Disable(HWND window)
{
	// Установка флага
	isEnable = false;

	// Отмена регистрация хоткея
	Unregister(window);
}
void Module::Register(HWND window) { hotkey.Register(window, static_cast<int>(GetType())); }
void Module::Unregister(HWND window) { hotkey.Unregister(window, static_cast<int>(GetType())); }
void Module::SetHotkey(HWND window, Hotkey hotkey)
{
	// Отмена регистрация хоткея
	Unregister(window);

	// Установка нового хоткея
	this->hotkey = hotkey;

	// Регистрация нового хоткея
	if (isEnable)
		Register(window);
}

// Виртуальные методы класса
void Module::ReadSettings(std::wistream* file)
{
	// Чтение параметров
	unsigned modifiers = 0;
	unsigned key = 0;
	bool isEnable = false;
	if (*file >> modifiers >> key >> isEnable)
	{
		hotkey = Hotkey{ modifiers, key };
		this->isEnable = isEnable;
	}
}	
void Module::WriteSettings(std::wostream* file) const { *file << hotkey.GetModifiers() << L' ' << hotkey.GetKey() << L' ' << isEnable; }