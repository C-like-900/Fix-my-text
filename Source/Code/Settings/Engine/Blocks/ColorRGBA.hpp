#pragma once
#include "ColorRGB.hpp"
#include "D2Defines.h"

// Определение класса
class ColorRGBA : public ColorRGB
{
public:
	// Фабрика
	static ColorRGBA Create(BYTE red, BYTE green, BYTE blue, float alpha = 1.f);
	
	// Конструкторы
	ColorRGBA(const D2Color& color);
	ColorRGBA(const ColorRGB& color, float alpha = 1.f);
	ColorRGBA(const ColorRGBA& colorEx);
	ColorRGBA();

	// Операторы класса
	bool operator==(const ColorRGBA& colorEx) const;
	ColorRGBA operator+(const ColorRGBA& colorEx) const;
	ColorRGBA operator-(const ColorRGBA& colorEx) const;
	ColorRGBA operator*(float value) const;

	// Методы класса
	float GetAlpha() const;
	float* GetAlphaReference();
	D2Color GetD2Color() const;
	void SetAlpha(float value);
	void SetD2Color(const D2Color& color);
	void SetChanels(BYTE red, BYTE green, BYTE blue, float alpha = 1.f);

private:
	// Поля класса
	float alpha;

	// Приватный конструктор
	ColorRGBA(float red, float green, float blue, float alpha);
};