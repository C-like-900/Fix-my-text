#include "KeyboardTools.hpp"
#include <iterator>
#include <vector>

// Статические методы класса
bool KeyboardTools::EmulateInput(std::initializer_list<WORD> keys, bool isKeysUp)
{
	// Инициализация INPUT вектора
	std::vector<INPUT> input{ keys.size() };
	DWORD keyEvent = isKeysUp ? KEYEVENTF_KEYUP : 0;
	BYTE id = 0;
	for (WORD key : keys)
	{
		input[id].type = INPUT_KEYBOARD;
		input[id].ki.wVk = key;
		input[id].ki.dwFlags = keyEvent;
		id++;
	}

	// Эмулирование клавиш
	return SendInput(static_cast<unsigned>(input.size()), input.data(), sizeof(INPUT)) != input.size();
}