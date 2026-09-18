#pragma once
#include "CompositeClass.h"
#include "BlockClass.hpp"

// Определение класса
class CompositeClass : public BlockClass
{
public:	
	// Перегруженные методы
	const BlockType GetType() const override;

protected:
	// Конструктор
	CompositeClass(const wchar_t* name, Block parent);
};