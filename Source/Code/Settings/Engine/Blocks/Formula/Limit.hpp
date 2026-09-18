#pragma once

// Определение класса
class Limit
{
public:
	// Фабрики
	static Limit CreateNothing();
	static Limit CreateLeftTop(float size = 0);
	static Limit CreateCenter(float size = 0);
	static Limit CreateRightBottom(float size = 0);

	// Операторы класса
	Limit operator+(const Limit& limit) const;
	Limit operator-(const Limit& limit) const;
	Limit operator*(float value) const;

	// Методы класса
	float GetSize() const;
	float GetLeftTop() const;
	float GetCenter() const;
	float GetRightBottom() const;
	float GetNonexistence() const;
private:
	// Поля класса
	float size;
	float leftTop;
	float center;
	float rightBottom;
	float nonexistence;
	
	// Приватные конструкторы
	Limit(float size, float leftTop, float center, float rightBottom, float nonexistence);
	Limit(float size, float leftTop, float center, float rightBottom);
	Limit();
};
