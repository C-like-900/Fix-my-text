#include "BoxClass.hpp"
 
// Фабрика
Box BoxClass::Create(const wchar_t* name, Block parent, const ColorRGBA& brushColor, const ColorRGBA& penColor, BYTE penWidth, float radius)
{
	return new BoxClass{ name, parent, brushColor, penColor, penWidth, radius };
}

// Методы класса
const ColorRGBA* BoxClass::GetBrushColor() const { return &brushColor; }
const ColorRGBA* BoxClass::GetPenColor() const { return &penColor; }
void BoxClass::SetBrushColor(const ColorRGB& color, const AnimationProperties& properties) { CreateAnimation(&dynamic_cast<ColorRGB&>(this->brushColor), color, this, properties); }
void BoxClass::SetBrushAlpha(float alpha, const AnimationProperties& properties) { CreateAnimation(this->brushColor.GetAlphaReference(), alpha, this, properties); }
void BoxClass::SetPenColor(const ColorRGB& color, const AnimationProperties& properties) { CreateAnimation(&dynamic_cast<ColorRGB&>(this->penColor), color, this, properties); }
void BoxClass::SetPenAlpha(float alpha, const AnimationProperties& properties){ CreateAnimation(this->penColor.GetAlphaReference(), alpha, this, properties); }

// Перегруженные методы
const BlockType BoxClass::GetType() const { return BlockType::BOX; }

// Конструктор и деструкотр
BoxClass::BoxClass(const wchar_t* name, Block parent, const ColorRGBA& brushColor, const ColorRGBA& penColor, BYTE penWidth, float radius)
	: BlockClass(name, parent), brushColor(brushColor), penColor(penColor), penWidth(penWidth), radius(radius) {}

// Перегруженные защищённые методы
void BoxClass::Paint()
{
	if (brushColor.GetAlpha())
	{
		Graphics::graphics->GetBrush()->SetColor(brushColor.GetD2Color());
		if (radius)
			Graphics::graphics->GetDeviceContext()->FillRoundedRectangle({ *rect.GetRect(), radius, radius }, Graphics::graphics->GetBrush());
		else Graphics::graphics->GetDeviceContext()->FillRectangle(rect.GetRect(), Graphics::graphics->GetBrush());
	}
	if (penColor.GetAlpha() && penWidth)
	{
		Graphics::graphics->GetBrush()->SetColor(penColor.GetD2Color());
		if (radius)
			Graphics::graphics->GetDeviceContext()->DrawRoundedRectangle({ *rect.GetRect(), radius, radius }, Graphics::graphics->GetBrush(), penWidth, Graphics::graphics->GetPenStyle());
		else Graphics::graphics->GetDeviceContext()->DrawRectangle(rect.GetRect(), Graphics::graphics->GetBrush(), penWidth, Graphics::graphics->GetPenStyle());
	}
}