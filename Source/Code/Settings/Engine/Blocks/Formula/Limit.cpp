#include "Limit.hpp"

#include <stdexcept>

// Фабрики
Limit Limit::CreateNothing() { return Limit{}; }
Limit Limit::CreateLeftTop(float size) { return Limit{ size, 1, 0, 0 }; }
Limit Limit::CreateCenter(float size) { return Limit{ size, 0, 1, 0 }; }
Limit Limit::CreateRightBottom(float size) { return Limit{ size, 0, 0, 1 }; }

// Операторы класса
Limit Limit::operator+(const Limit& limit) const
{
	return Limit{ size + limit.size, leftTop + limit.leftTop, center + limit.center, rightBottom + limit.rightBottom, nonexistence + limit.nonexistence };
}
Limit Limit::operator-(const Limit& limit) const
{
	return Limit{ size - limit.size, leftTop - limit.leftTop, center - limit.center, rightBottom - limit.rightBottom, nonexistence - limit.nonexistence };
}
Limit Limit::operator*(float value) const
{
	return Limit{ size * value, leftTop * value, center * value, rightBottom * value, nonexistence * value };
}

// Методы класса
float Limit::GetSize() const { return size; }
float Limit::GetLeftTop() const { return leftTop; }
float Limit::GetCenter() const { return center; }
float Limit::GetRightBottom() const { return rightBottom; }
float Limit::GetNonexistence() const { return nonexistence; }

// Приватные конструкторы
Limit::Limit(float size, float leftTop, float center, float rightBottom, float nonexistence) : size(size), leftTop(leftTop), center(center),
	rightBottom(rightBottom), nonexistence(nonexistence) {}
Limit::Limit(float size, float leftTop, float center, float rightBottom) : leftTop(leftTop), center(center), rightBottom(rightBottom), nonexistence(0)
{ 
	if (size < 0)
		throw std::runtime_error("Negative input");
	this->size = size;
}
Limit::Limit() : Limit{ 0, 0, 0, 0, 1 } {}
