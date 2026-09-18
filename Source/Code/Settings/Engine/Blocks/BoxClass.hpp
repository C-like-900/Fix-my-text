#pragma once
#include "BoxClass.h"
#include "BlockClass.hpp"
#include "ColorRGBA.hpp"

// Определение класса
class BoxClass : public BlockClass
{
public:
	// Фабрика
	static Box Create(const wchar_t* name = DEFAULT_NAME, Block parent = 0, const ColorRGBA& brushColor = {},
		const ColorRGBA& penColor = {}, BYTE penWidth = 0, float radius = 0);

	// Методы класса
	const ColorRGBA* GetBrushColor() const;
	const ColorRGBA* GetPenColor() const;
	void SetBrushColor(const ColorRGB& color, const AnimationProperties& properties = {});
	void SetBrushAlpha(float alpha, const AnimationProperties& properties = {});
	void SetPenColor(const ColorRGB& color, const AnimationProperties& properties = {});
	void SetPenAlpha(float alpha, const AnimationProperties& properties = {});

	// Перегруженные методы
	const BlockType GetType() const override;

protected:
	// Поля класса
	ColorRGBA brushColor;
	ColorRGBA penColor;
	BYTE penWidth;
	float radius;

	// Конструктор
	BoxClass(const wchar_t* name, Block parent, const ColorRGBA& brushColor, const ColorRGBA& penColor, BYTE penWidth, float radius);

	// Перегруженные защищённые методы
	void Paint() override;
};