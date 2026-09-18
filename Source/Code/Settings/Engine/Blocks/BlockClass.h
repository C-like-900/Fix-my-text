#pragma once
#include <Windows.h>
#include <stack>
#include <vector>

// Магические значения
const wchar_t DEFAULT_NAME[] = L"Block";
const wchar_t DEFAULT_TEXT[] = L"Sample text";
const USHORT DEFAULT_TEXT_HEIGHT = 16;

// Связанные типы данных
enum class BlockType : BYTE { ROOT, BLOCK, BOX, TEXT, PICTURE, BLUR, COMPOSITE };

// Предварительное объявление класса
class BlockClass;

// Создание псевдонимов класса
using Block = BlockClass*;
using BlockStack = std::stack<Block>;
using BlockVector = std::vector<Block>;