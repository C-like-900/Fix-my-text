#pragma once
#include "ScrollClass.h"
#include "CompositeClass.hpp"
#include "ScrollContextClass.h"
#include "ScrollBarClass.h"

// Определение класса
class ScrollClass : public CompositeClass
{
public:
	// Фабрика
	static Scroll Create(const wchar_t* name = DEFAULT_NAME, Block parent = nullptr, D2SizeF size = {}, const ColorRGBA& color = {});

	// Методы класса
	ScrollContext GetContext() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetHorizontalOffset() const;
	float GetVerticalOffset() const;
	float GetHorizontalEndOffset();
	float GetVerticalEndOffset();
	void ScrollEvent(float wheelDelta, bool isHorizontal);
	void SetWidth(float value, AnimationProperties properties);
	void SetHeight(float value, AnimationProperties properties);
	void SetHorizontalOffset(float value);
	void SetVerticalOffset(float value);
	void SetHorizontalEndOffset(float value, AnimationProperties properties);
	void SetVerticalEndOffset(float value, AnimationProperties properties);

protected:
	// Поля класса
	D2SizeF			size;
	D2SizeF			offset;

	// Дети класса
	ScrollContext	context;
	ScrollBar		scrollBarH;
	ScrollBar		scrollBarV;

	// Конструктор
	ScrollClass(const wchar_t* name, Block parent, D2SizeF size, const ColorRGBA& color);
};