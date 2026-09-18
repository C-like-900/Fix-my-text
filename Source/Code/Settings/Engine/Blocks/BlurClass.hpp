#pragma once
#include "BlurClass.h"
#include "BlockClass.hpp"

// Определение класса
class BlurClass : public BlockClass
{
public:
	// Фабрика
	static Blur Create(const wchar_t* name = DEFAULT_NAME, Block parent = 0, float sigma = DEFAULT_SIGMA);

	// Методы класса
	float GetSigma() const;
	void SetSigma(float value, const AnimationProperties& properties = {});

	// Перегруженные методы
	const BlockType GetType() const override;

protected:
	// Поля класса
	float sigma;

	// Конструктор
	BlurClass(const wchar_t* name, Block parent, float sigma);

	// Перегруженные защищённые методы
	void Paint() override;
};