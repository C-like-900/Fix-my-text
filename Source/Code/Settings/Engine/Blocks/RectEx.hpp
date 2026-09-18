#pragma once
#include "RectEx.h"
#include <Windows.h>
#include "D2Defines.h"

// Определение класса
class RectEx
{
public:
	// Конструкторы
	RectEx(float left, float top, float right, float bottom);
	RectEx(D2RectF rect);
	RectEx();

	// Операторы класса
	bool operator==(const RectEx& rectEx) const;
	RectEx operator+(const RectEx& rectEx) const;
	RectEx operator-(const RectEx& rectEx) const;
	RectEx operator*(float value) const;

	// Методы класса
	float GetSide(SideID id) const;
	float GetLeft() const;
	float GetTop() const;
	float GetRight() const;
	float GetBottom() const;
	const D2RectF* GetRect() const;
	float GetWidth() const;
	float GetHeight() const;
	void Offset(float width, float height);
	void Inflate(float width, float height);
	void Intersect(const RectEx* mask);
	bool IsPointInside(POINT point);
	void SetSide(SideID id, float value);
	void SetLeft(float value);
	void SetTop(float value);
	void SetRight(float value);
	void SetBottom(float value);
	void SetRect(D2RectF rect);
	void SetSides(float left, float top, float right, float bottom);

private:
	// Поля класса
	union
	{
		float sides[RECT_SIZE];
		D2RectF rect;
	};
};