#pragma once
#include <Windows.h>

// Определение класса
class ColorRGB
{
public:
	// Фабрика
	static ColorRGB Create(BYTE red, BYTE green, BYTE blue);

	// Конструкторы
	ColorRGB(const ColorRGB& colorRGB);
	ColorRGB();

	// Операторы класса
	bool operator==(const ColorRGB& colorRGB) const;
	ColorRGB operator+(const ColorRGB& colorRGB) const;
	ColorRGB operator-(const ColorRGB& colorRGB) const;
	ColorRGB operator*(float value) const;

	// Методы класса
	BYTE GetRed() const;
	BYTE GetGreen() const;
	BYTE GetBlue() const;
	void SetRed(BYTE value);
	void SetGreen(BYTE value);
	void SetBlue(BYTE value);
	void SetChanels(BYTE red, BYTE green, BYTE blue);

protected:
	// Поля класса
	float red;
	float green;
	float blue;

	// Защищённый конструктор
	ColorRGB(float red, float green, float blue);
};