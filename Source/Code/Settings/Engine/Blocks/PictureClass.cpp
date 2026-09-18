#include "PictureClass.hpp"

// Фабрика
Picture PictureClass::Create(const wchar_t* name, Block parent, const wchar_t* path) { return new PictureClass{ name, parent, path }; }

// Перегруженные методы
const BlockType PictureClass::GetType() const { return BlockType::PICTURE; }

// Конструктор и деструкотр
PictureClass::PictureClass(const wchar_t* name, Block parent, const wchar_t* path) : BlockClass(name, parent), bitmap(nullptr)
{
	// Преобразование файла картинки в битмап
	DIBitmapDecoder decoder = nullptr;
	if (SUCCEEDED(Graphics::graphics->GetImageFactory()->CreateDecoderFromFilename(path, 0, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder)))
	{
        // Если декодер создан успешно – получаем первый кадр изображения
        DIBitmapFrame frame = nullptr;
        if (SUCCEEDED(decoder->GetFrame(0, &frame)))
        {
            // Кадр получен, теперь нужен конвертер форматов
            DIFormatConverter converter = nullptr;
            if (SUCCEEDED(Graphics::graphics->GetImageFactory()->CreateFormatConverter(&converter)))
            {
                // Инициализируем конвертер: преобразуем кадр в формат 32bpp PBGRA
                if (SUCCEEDED(converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, 0, 0, WICBitmapPaletteTypeMedianCut)))
                {
                    // Создаём битмап из сконвертированного WIC-изображения
                    Graphics::graphics->GetDeviceContext()->CreateBitmapFromWicBitmap(converter, 0, &bitmap);
                }
                // Освобождаем конвертер
                converter->Release();
            }
            // Освобождаем кадр
            frame->Release();
        }
        // Освобождаем декодер
        decoder->Release();
	}
	else Graphics::graphics->GetDeviceContext()->CreateBitmap({ 1, 1 }, BitmapProperties(PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &bitmap);
}
PictureClass::~PictureClass() { if (bitmap) bitmap->Release(); }

// Методы класса
void PictureClass::Paint() { Graphics::graphics->GetDeviceContext()->DrawBitmap(bitmap, rect.GetRect()); }