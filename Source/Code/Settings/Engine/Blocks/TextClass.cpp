#include "TextClass.hpp"

// Фабрика
Text TextClass::Create(const wchar_t* name, Block parent, const ColorRGBA& color, const wchar_t* text, USHORT tHeight, DWRITE_FONT_WEIGHT tWeight,
	DWRITE_TEXT_ALIGNMENT tAligment, bool isMultiLine)
{
	return new TextClass{ name, parent, color, text, tHeight, tWeight, tAligment, isMultiLine };
}

// Методы класса
void TextClass::SetColor(const ColorRGB& color, const AnimationProperties& properties) { CreateAnimation(&dynamic_cast<ColorRGB&>(this->color), color, this, properties); }
void TextClass::SetAlpha(float alpha, const AnimationProperties & properties) { CreateAnimation(this->color.GetAlphaReference(), alpha, this, properties); }

// Перегруженные методы
const BlockType TextClass::GetType() const { return BlockType::TEXT; }

// Конструктор и деструкотр
TextClass::TextClass(const wchar_t* name, Block parent, const ColorRGBA& color, const wchar_t* text, USHORT tHeight, DWRITE_FONT_WEIGHT tWeight,
	DWRITE_TEXT_ALIGNMENT tAligment, bool isMultiLine) : BlockClass(name, parent), color(color), text(text),
	isMultiLine(isMultiLine), length((UINT)wcslen(text)), layout(0)
{
	// Определение текстового макета
	DWTextFormat textFormat = nullptr;
	Graphics::graphics->GetWriteFactory()->CreateTextFormat(L"Calibri", 0, tWeight, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, tHeight, L"", &textFormat);
	if (textFormat)
	{
		// Установка выравнивания
		textFormat->SetTextAlignment(tAligment);
		textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		Graphics::graphics->GetWriteFactory()->CreateTextLayout(text, length, textFormat, 0, 0, &layout);
		if (layout)
		{
			// Установка усечения
			DWInlineObject ellipseSign = nullptr;
			Graphics::graphics->GetWriteFactory()->CreateEllipsisTrimmingSign(layout, &ellipseSign);
			if (ellipseSign)
			{
				DWTrimming trimm = { DWRITE_TRIMMING_GRANULARITY_CHARACTER };
				layout->SetTrimming(&trimm, ellipseSign);
				ellipseSign->Release();
			}

			// Установка переноса
			if (!isMultiLine)
				layout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		}
		textFormat->Release();
	}
}
TextClass::~TextClass() { if (layout) layout->Release(); }

// Методы класса
void TextClass::Update()
{
	// Стандартное обновление
	BlockClass::Update();

	// Установка макета
	if (layout)
	{
		layout->SetMaxWidth(rect.GetWidth());
		layout->SetMaxHeight(rect.GetHeight());
	}
}
void TextClass::Paint()
{
	if (color.GetAlpha())
	{
		Graphics::graphics->GetBrush()->SetColor(color.GetD2Color());
		Graphics::graphics->GetDeviceContext()->DrawTextLayout({rect.GetLeft(), rect.GetTop()}, layout, Graphics::graphics->GetBrush());
	}
}