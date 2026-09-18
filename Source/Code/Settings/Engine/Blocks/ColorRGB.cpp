#include "ColorRGB.hpp"

// Фабрика
ColorRGB ColorRGB::Create(BYTE red, BYTE green, BYTE blue)
{
    return ColorRGB{ static_cast<float>(red) / 255.f, static_cast<float>(green) / 255.f, static_cast<float>(blue) / 255.f };
}

// Конструкторы
ColorRGB::ColorRGB(const ColorRGB& colorEx) : red(colorEx.red), green(colorEx.green), blue(colorEx.blue) {}
ColorRGB::ColorRGB() : red(0.f), green(0.f), blue(0.f) {}

// Операторы класса
bool ColorRGB::operator==(const ColorRGB& colorEx) const { return (red == colorEx.red) && (green == colorEx.green) && (blue == colorEx.blue); }
ColorRGB ColorRGB::operator+(const ColorRGB& colorEx) const { return ColorRGB{ red + colorEx.red, green + colorEx.green, blue + colorEx.blue }; }
ColorRGB ColorRGB::operator-(const ColorRGB& colorEx) const { return ColorRGB{ red - colorEx.red, green - colorEx.green, blue - colorEx.blue }; }
ColorRGB ColorRGB::operator*(float value) const { return ColorRGB{ red * value, green * value, blue * value }; }

// Методы класса
BYTE ColorRGB::GetRed() const { return static_cast<BYTE>(red * 255.f); }
BYTE ColorRGB::GetGreen() const { return static_cast<BYTE>(green * 255.f); }
BYTE ColorRGB::GetBlue() const { return static_cast<BYTE>(blue * 255.f); }
void ColorRGB::SetRed(BYTE value) { red = static_cast<float>(value) / 255.f; }
void ColorRGB::SetGreen(BYTE value) { green = static_cast<float>(value) / 255.f; }
void ColorRGB::SetBlue(BYTE value) { blue = static_cast<float>(value) / 255.f; }
void ColorRGB::SetChanels(BYTE red, BYTE green, BYTE blue)
{
    this->red = static_cast<float>(red) / 255.f;
    this->green = static_cast<float>(green) / 255.f;
    this->blue = static_cast<float>(blue) / 255.f;
}

// Защищённый конструктор
ColorRGB::ColorRGB(float red, float green, float blue) : red(red), green(green), blue(blue) {}