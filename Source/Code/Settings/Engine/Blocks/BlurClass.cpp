#include <initguid.h>
#include <d2d1effects.h>
#include "BlurClass.hpp"
#include <cmath>
#include <stdexcept>

// Фабрика
Blur BlurClass::Create(const wchar_t* name, Block parent, float sigma) { return new BlurClass{ name, parent, sigma }; }

// Методы класса
float BlurClass::GetSigma() const { return sigma; }
void BlurClass::SetSigma(float value, const AnimationProperties& properties) { CreateAnimation(&sigma, value, this, properties); }

// Перегруженные методы
const BlockType BlurClass::GetType() const { return BlockType::BLUR; }

// Конструктор
BlurClass::BlurClass(const wchar_t* name, Block parent, float sigma) : BlockClass(name, parent), sigma(sigma) {}

// Перегруженные защищённые методы
void BlurClass::Paint()
{
	// Получение DPI значений
	float dpi = Graphics::graphics->GetDpiRatio() * USER_DEFAULT_SCREEN_DPI;
	float dpiRatio = Graphics::graphics->GetDpiRatio();

	// Получение координат области для размытия
	D2Rect sourceRect
	{
		static_cast<UINT>(rect.GetLeft() * dpiRatio),
		static_cast<UINT>(rect.GetTop() * dpiRatio),
		static_cast<UINT>(std::ceil(rect.GetRight() * dpiRatio)),
		static_cast<UINT>(std::ceil(rect.GetBottom() * dpiRatio))
	};

	// Создание битмапа для размытия
	D2TargetBitmap sourceBitmap = nullptr;
	D2DeviceContext deviceContext = Graphics::graphics->GetDeviceContext();
	if (FAILED(deviceContext->CreateBitmap({ sourceRect.right - sourceRect.left, sourceRect.bottom - sourceRect.top }, nullptr, 0, BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE,
		PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi, dpi), &sourceBitmap)))
		throw std::runtime_error("CreateBitmap ERROR");

	// Копирование данных в битмап
	if (FAILED(sourceBitmap->CopyFromRenderTarget(0, deviceContext, &sourceRect)))
		throw std::runtime_error("CopyFromRenderTarget ERROR");

	// Применение эффекта размытия
	D2Effect blurEffect = nullptr;
	if (FAILED(deviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &blurEffect)))
		throw std::runtime_error("CreateEffect ERROR");
	blurEffect->SetInput(0, sourceBitmap);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, sigma);
	blurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
	sourceBitmap->Release();

	// Отрисовка результата
	deviceContext->PushAxisAlignedClip(rect.GetRect(), D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	deviceContext->DrawImage(blurEffect, { static_cast<float>(sourceRect.left), static_cast<float>(sourceRect.top) }, D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_OVER);
	deviceContext->PopAxisAlignedClip();
	blurEffect->Release();
}
