#include "CompositeClass.hpp"

// Перегруженные методы
const BlockType CompositeClass::GetType() const { return BlockType::COMPOSITE; }

// Конструктор
CompositeClass::CompositeClass(const wchar_t* name, Block parent) : BlockClass{ name, parent } {}