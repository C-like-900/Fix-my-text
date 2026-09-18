#pragma once
#include "ScrollBarClass.h"
#include "CompositeClass.hpp"
#include "BoxClass.h"

// Определение класса
class ScrollBarClass : public CompositeClass
{
public:
	// Фабрика
	static ScrollBar Create(const wchar_t* name = DEFAULT_NAME, Block parent = nullptr, const ColorRGB& color = {}, bool isHorizontal = true);

	// Методы класса
	float GetDragMouse() const;
	float GetDragOffset() const;
	float GetRatio() const;
	bool IsVisible() const;
	void SetDragMouse(float value);
	void SetDragOffset(float value);
	void SetRatio(float value);

protected:
	// Поля класса
	float	dragMouse;
	float	dragOffset;
	float	ratio;
	bool	isHorizontal;
	bool	isVisible;

	// Дети класса
	Box		phantomBar;
	Box		currentBar;

	// Конструктор
	ScrollBarClass(const wchar_t* name, Block parent, const ColorRGB& color, bool isVertical);

	// Перегруженные защищённые методы
	void Update() override;
};