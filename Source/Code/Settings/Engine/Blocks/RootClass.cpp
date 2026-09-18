#include "RootClass.hpp"

#include <stdexcept>

// Конструктор
RootClass::RootClass(const ColorRGBA& bgColor) : BlockClass(L"ROOT", 0), bgColor(bgColor) {}

// Методы класса
void RootClass::SetColor(const ColorRGB& color, const AnimationProperties& properties) { CreateAnimation(&dynamic_cast<ColorRGB&>(this->bgColor), color, this, properties); }
void RootClass::SetAlpha(float alpha, const AnimationProperties & properties) { CreateAnimation(this->bgColor.GetAlphaReference(), alpha, this, properties); }

// Перегруженные методы класса
const BlockType RootClass::GetType() const { return BlockType::ROOT; }
void RootClass::SetFormula(const Formula& formula, const AnimationProperties& properties) { throw std::runtime_error("Roots can't be edited"); }
void RootClass::Update()
{
	// Установка координат всего окна
	RECT clientRect;
	GetClientRect(Graphics::graphics->GetWindow(), &clientRect);

	// Установка размера корня
	rect.SetRight(static_cast<float>(clientRect.right) / Graphics::graphics->GetDpiRatio());
	rect.SetBottom(static_cast<float>(clientRect.bottom) / Graphics::graphics->GetDpiRatio());
}
void RootClass::Paint() { Graphics::graphics->GetDeviceContext()->Clear(bgColor.GetD2Color()); }
