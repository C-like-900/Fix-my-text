#pragma once
#include <d2d1_1.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <dwrite.h>
#include <wincodec.h>
using namespace D2D1;

// Переопределение Direct2D типов данных
using D2Factory =				ID2D1Factory1*;
using D2Device =				ID2D1Device*;
using D2DeviceContext =			ID2D1DeviceContext*;
using D2TargetBitmap =			ID2D1Bitmap1*;
using D2BitmapRender =			ID2D1BitmapRenderTarget*;
using D2Brush =					ID2D1SolidColorBrush*;
using D2PenStyle =				ID2D1StrokeStyle*;
using D2Bitmap =				ID2D1Bitmap*;
using D2Effect =				ID2D1Effect*;
using D2Size =					D2D1_SIZE_U;
using D2SizeF =					D2D1_SIZE_F;
using D2Rect =					D2D1_RECT_U;
using D2RectF =					D2D1_RECT_F;
using D2Color =					D2D1_COLOR_F;

// Переопределение Direct3D типов данных
using D3Device =				ID3D11Device*;
using D3DeviceContext =			ID3D11DeviceContext*;

// Переопределение DirectX типов данных
using DXDevice =				IDXGIDevice*;
using DXAdapter =				IDXGIAdapter*;
using DXFactory =				IDXGIFactory2*;
using DXSwapChain =				IDXGISwapChain1*;
using DXSurface =				IDXGISurface*;
using DXSwapChainDescription =	DXGI_SWAP_CHAIN_DESC1;

// Переопределение DirectWrite типов данных
using DWFactory =				IDWriteFactory*;
using DWTextFormat =			IDWriteTextFormat*;
using DWInlineObject =			IDWriteInlineObject*;
using DWTextLayout =			IDWriteTextLayout*;
using DWTrimming =				DWRITE_TRIMMING;

// Переопределение DirectImage типов данных
using DIFactory =				IWICImagingFactory*;
using DIBitmapDecoder =			IWICBitmapDecoder*;
using DIBitmapFrame =			IWICBitmapFrameDecode*;
using DIFormatConverter =		IWICFormatConverter*;