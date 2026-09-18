#include "ScrollContextClass.hpp"
#include "ScrollClass.hpp"

// Фабрика
ScrollContext ScrollContextClass::Create(const wchar_t* name, Block parent, D2SizeF size) { return new ScrollContextClass{ name, parent, size }; }

// Методы класса
float ScrollContextClass::GetHorizontalEndOffset() const { return endOffset.width; }
float ScrollContextClass::GetVerticalEndOffset() const { return endOffset.height; }
void ScrollContextClass::SetHorizontalEndOffset(float value) { endOffset.width = value; }
void ScrollContextClass::SetVerticalEndOffset(float value) { endOffset.height = value; }

// Конструктор
ScrollContextClass::ScrollContextClass(const wchar_t* name, Block parent, D2SizeF size) : CompositeClass{ name, parent }, endOffset()
{
	// Установка формулы
	SetFormula(Formula
	{
		SideFormula::CreateStatic(0),
		SideFormula::CreateStatic(0),
		SideFormula::CreateStatic(size.width),
		SideFormula::CreateStatic(size.height)
	});
}

// Перегруженные защищённые методы
void ScrollContextClass::Update()
{
	// Получение значений скролла
	Scroll scroll = dynamic_cast<Scroll>(parent);
	float parentWidth = scroll->GetRect()->GetWidth();
	float parentHeight = scroll->GetRect()->GetHeight();

	// Получение размеров контента
	float contentWidth = scroll->GetWidth();
	float contentHeight = scroll->GetHeight();
	if (contentWidth < parentWidth)
		contentWidth = parentWidth;
	if (contentHeight < parentHeight)
		contentHeight = parentHeight;

	// Корректировка конечного смещения
	D2SizeF newEndOffset = endOffset;
	FixOffset(&newEndOffset.width, parentWidth, contentWidth);
	FixOffset(&newEndOffset.height, parentHeight, contentHeight);
	if (newEndOffset.width != endOffset.width)
		endOffset.width = newEndOffset.width;
	if (newEndOffset.height != endOffset.height)
		endOffset.height = newEndOffset.height;
	
	// Корректировка текущего смещения
	D2SizeF newOffset = { scroll->GetHorizontalOffset(), scroll->GetVerticalOffset() };
	FixOffset(&newOffset.width, parentWidth, contentWidth);
	FixOffset(&newOffset.height, parentHeight, contentHeight);
	if (newOffset.width != scroll->GetHorizontalOffset())
		scroll->SetHorizontalOffset(newOffset.width);
	if (newOffset.height != scroll->GetVerticalOffset())
		scroll->SetVerticalOffset(newOffset.height);

	// Уснановка новых значений для формулы
	formula->SetLeftSide(SideFormula::CreateStatic(newOffset.width));
	formula->SetTopSide(SideFormula::CreateStatic(newOffset.height));
	formula->SetRightSide(SideFormula::CreateStatic(newOffset.width + contentWidth));
	formula->SetBottomSide(SideFormula::CreateStatic(newOffset.height + contentHeight));

	// Вызов базового метода
	CompositeClass::Update();
}

// Приватные методы класса
void ScrollContextClass::FixOffset(float* offset, float parentSize, float contentSize)
{
	// Пропуск, если весь контент помещается или смещение положительное
	if (*offset <= 0)
	{
		// Корректировка смещения, если оно выходит за размеры контента
		float maxOffset = parentSize - contentSize;
		if (*offset < maxOffset)
			*offset = maxOffset;
	}
	else *offset = 0;
}