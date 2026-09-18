#include "Hotkey.hpp"

// Конструктор
Hotkey::Hotkey(unsigned modifiers, unsigned key) : modifiers(modifiers), key(key), isRegistered(false) {}

// Методы класса
BYTE Hotkey::GetModifiers() const { return modifiers; }
WORD Hotkey::GetKey() const { return key; }
bool Hotkey::IsRegistered() const { return isRegistered; }
bool Hotkey::Register(HWND window, int id)
{
	// Регистрация хоткея
	if (!isRegistered && RegisterHotKey(window, id, static_cast<unsigned>(modifiers) | MOD_NOREPEAT, static_cast<unsigned>(key)))
		isRegistered = true;

	// Возврат результата
	return isRegistered;
}
void Hotkey::Unregister(HWND window, int id)
{
	// Проверка на регистрацию
	if (!isRegistered)
		return;

	// Отмена регистрация хоткея
	UnregisterHotKey(window, id);
	isRegistered = false;
}
