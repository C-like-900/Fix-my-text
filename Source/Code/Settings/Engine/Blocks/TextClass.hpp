#pragma once
#include "TextClass.h"
#include "BlockClass.hpp"
#include "ColorRGBA.hpp"

// Определение класса
class TextClass : public BlockClass
{
public:
	// Фабрика
	static Text Create(const wchar_t* name = DEFAULT_NAME, Block parent = nullptr, const ColorRGBA& color = {}, const wchar_t* text = DEFAULT_TEXT,
		USHORT tHeight = DEFAULT_TEXT_HEIGHT, DWRITE_FONT_WEIGHT tWeight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_TEXT_ALIGNMENT tAligment = DWRITE_TEXT_ALIGNMENT_CENTER,
		bool isMultiLine = false);

	// Методы класса
	void SetColor(const ColorRGB& color, const AnimationProperties& properties = {});
	void SetAlpha(float alpha, const AnimationProperties& properties = {});

	// Перегруженные методы
	const BlockType GetType() const override;

protected:
	// Поля класса
	const wchar_t*	text;
	UINT			length;
	ColorRGBA		color;
	bool			isMultiLine;
	DWTextLayout	layout;

	// Конструктор и деструкотр
	TextClass(const wchar_t* name, Block parent, const ColorRGBA& color, const wchar_t* text, USHORT tHeight, DWRITE_FONT_WEIGHT tWeight, DWRITE_TEXT_ALIGNMENT tAligment, bool isMultiLine);
	~TextClass();

	// Перегруженные методы
	void Update() override;
	void Paint() override;
};