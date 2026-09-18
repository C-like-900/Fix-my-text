#pragma once

// Определение класса
class SideFormula
{
public:
	// Фабрики
	static SideFormula CreateStatic(float offset = 0);
	static SideFormula CreateGlue(float offset = 0);
	static SideFormula CreatePercent(float percent = 0);

	// Операторы класса
	SideFormula operator+(const SideFormula& side) const;
	SideFormula operator-(const SideFormula& side) const;
	SideFormula operator*(float value) const;

	// Методы класса
	float GetLeftTop() const;
	float GetRightBottom() const;
	float GetConstant() const;

private:
	// Поля класса
	float leftTop;
	float rightBottom;
	float constant;

	// Приватный конструктор
	SideFormula(float leftTop, float rightBottom, float constant);
};