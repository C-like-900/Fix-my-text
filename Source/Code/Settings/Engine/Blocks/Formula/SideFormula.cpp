#include "SideFormula.hpp"

// Фабрики
SideFormula SideFormula::CreateStatic(float offset) { return SideFormula{ 1, 0, offset }; }
SideFormula SideFormula::CreateGlue(float offset) { return SideFormula{ 0, 1, -offset }; }
SideFormula SideFormula::CreatePercent(float percent) { return SideFormula{ 1 - percent / 100.f, percent / 100.f, 0 }; }

// Операторы класса
SideFormula SideFormula::operator+(const SideFormula& side) const { return SideFormula{ leftTop + side.leftTop, rightBottom + side.rightBottom, constant + side.constant }; }
SideFormula SideFormula::operator-(const SideFormula& side) const { return SideFormula{ leftTop - side.leftTop, rightBottom - side.rightBottom, constant - side.constant }; }
SideFormula SideFormula::operator*(float value) const { return SideFormula{ leftTop * value, rightBottom * value, constant * value }; }

// Методы класса
float SideFormula::GetLeftTop() const { return leftTop; }
float SideFormula::GetRightBottom() const { return rightBottom; }
float SideFormula::GetConstant() const { return constant; }

// Приватный конструктор
SideFormula::SideFormula(float leftTop, float rightBottom, float constant) : leftTop(leftTop), rightBottom(rightBottom), constant(constant) {}