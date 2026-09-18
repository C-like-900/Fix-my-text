#pragma once
#include "RootClass.h"
#include "BlockClass.hpp"
#include "Formula.hpp"
#include "AnimationProperties.hpp"
#include "ColorRGBA.hpp"

// Определение класса
class RootClass : public BlockClass
{
public:
	// Дружба с графикой
	friend class Graphics;

	// Методы класса
	void SetColor(const ColorRGB& color, const AnimationProperties& properties = {});
	void SetAlpha(float alpha, const AnimationProperties& properties = {});

	// Перегруженные методы
	const BlockType GetType() const override;
	void SetFormula(const Formula& formula, const AnimationProperties& properties = {}) override;

protected:
	// Поля класса
	ColorRGBA bgColor;

	// Конструктор и деструкотр
	RootClass(const ColorRGBA& bgColor);

	// Перегруженные защищённые методы
	void Update() override;
	void Paint() override;
};