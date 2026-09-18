#pragma once
#include "ScrollContextClass.h"
#include "CompositeClass.hpp"

// Определение класса
class ScrollContextClass : public CompositeClass
{
public:
	// Фабрика
	static ScrollContext Create(const wchar_t* name = DEFAULT_NAME, Block parent = nullptr, D2SizeF size = {});

	// Методы класса
	float GetHorizontalEndOffset() const;
	float GetVerticalEndOffset() const;
	void SetHorizontalEndOffset(float value);
	void SetVerticalEndOffset(float value);

protected:
	// Поля класса
	D2SizeF		endOffset;

	// Конструктор
	ScrollContextClass(const wchar_t* name, Block parent, D2SizeF size);

	// Перегруженные защищённые методы
	void Update() override;

private:
	// Приватные методы класса
	void FixOffset(float* offset, float parentSize, float contentSize);
}; 