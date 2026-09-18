#include "ColorRGBA.hpp"

// Фабрика
ColorRGBA ColorRGBA::Create(BYTE red, BYTE green, BYTE blue, float alpha)
{
    return ColorRGBA{ static_cast<float>(red) / 255.f, static_cast<float>(green) / 255.f, static_cast<float>(blue) / 255.f, alpha };
}

// Конструкторы
ColorRGBA::ColorRGBA(const D2Color& color) : ColorRGB{ color.r, color.g, color.b }, alpha(color.a) {}
ColorRGBA::ColorRGBA(const ColorRGB& color, float alpha) : ColorRGB{ color }, alpha(alpha) {}
ColorRGBA::ColorRGBA(const ColorRGBA& color) : ColorRGB(color), alpha(color.alpha) {}
ColorRGBA::ColorRGBA() : ColorRGB(), alpha(1.f) {}

// Операторы класса
bool ColorRGBA::operator==(const ColorRGBA& colorEx) const { return (red == colorEx.red) && (green == colorEx.green) && (blue == colorEx.blue) && (alpha == colorEx.alpha); }
ColorRGBA ColorRGBA::operator+(const ColorRGBA& colorEx) const { return ColorRGBA{ red + colorEx.red, green + colorEx.green, blue + colorEx.blue, alpha + colorEx.alpha }; }
ColorRGBA ColorRGBA::operator-(const ColorRGBA& colorEx) const { return ColorRGBA{ red - colorEx.red, green - colorEx.green, blue - colorEx.blue, alpha - colorEx.alpha }; }
ColorRGBA ColorRGBA::operator*(float value) const { return ColorRGBA{ red * value, green * value, blue * value, alpha * value }; }

// Методы класса
float ColorRGBA::GetAlpha() const { return alpha; }
float* ColorRGBA::GetAlphaReference() { return &alpha; }
D2Color ColorRGBA::GetD2Color() const { return D2Color{ red, green, blue, alpha }; }
void ColorRGBA::SetAlpha(float value) { alpha = value; }
void ColorRGBA::SetD2Color(const D2Color& color)
{
	red = color.r;
	green = color.g;
	blue = color.b;
	alpha = color.a;
}
void ColorRGBA::SetChanels(BYTE red, BYTE green, BYTE blue, float alpha)
{
    ColorRGB::SetChanels(red, green, blue);
    this->alpha = alpha;
}

// Приватный конструктор
ColorRGBA::ColorRGBA(float red, float green, float blue, float alpha) : ColorRGB{ red, green, blue }, alpha(alpha) {}