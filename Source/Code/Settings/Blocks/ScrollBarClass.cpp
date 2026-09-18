#include "ScrollBarClass.hpp"
#include "BoxClass.hpp"	
#include "ScrollClass.hpp"	
#include "ScrollContextClass.hpp"	
#include <windowsx.h>

// Фабрика
ScrollBar ScrollBarClass::Create(const wchar_t* name, Block parent, const ColorRGB& color, bool isHorizontal) { return new ScrollBarClass{ name, parent, color, isHorizontal }; }

// Методы класса
float ScrollBarClass::GetDragMouse() const { return dragMouse; }
float ScrollBarClass::GetDragOffset() const { return dragOffset; }
float ScrollBarClass::GetRatio() const { return ratio; }
bool ScrollBarClass::IsVisible() const { return isVisible; }
void ScrollBarClass::SetDragMouse(float value) { dragMouse = value; }
void ScrollBarClass::SetDragOffset(float value) { dragOffset = value; }
void ScrollBarClass::SetRatio(float value) { ratio = value; }

// Конструктор
ScrollBarClass::ScrollBarClass(const wchar_t* name, Block parent, const ColorRGB& color, bool isHorizontal) : CompositeClass{ name, parent }, dragMouse(), dragOffset(), ratio(),
	isHorizontal(isHorizontal), isVisible(false)
{
	// Создание фантомной полосы
	phantomBar = BoxClass::Create(L"PhantomBar", this, ColorRGBA{ color, SCROLL_BAR_PHANTOM_ALPHA }, {}, 0, SCROLL_BAR_RADIUS);

	// Создание текущей полосы
	currentBar = BoxClass::Create(L"CurrentBar", this, color, {}, 0, SCROLL_BAR_RADIUS);

	// Установка значений в зависимости от ориентации
	if (isHorizontal)
	{
		// Установка формулы
		SetFormula(Formula
			{
				SideFormula::CreateStatic(SCROLL_BAR_OFFSET + SCROLL_BAR_FRAME_OFFSET),
				SideFormula::CreateGlue(SCROLL_BAR_OFFSET + SCROLL_BAR_SIZE),
				SideFormula::CreateGlue(SCROLL_BAR_OFFSET + SCROLL_BAR_FRAME_OFFSET),
				SideFormula::CreateGlue(SCROLL_BAR_OFFSET)
			});

		// Добавление событий фантомной полосе
		AddEvent(MouseEventType::PRESS, EVENT(block)
		{
			ScrollBar scrollBar = dynamic_cast<ScrollBar>(block);
			Scroll scroll = dynamic_cast<Scroll>(block->GetParent());
			Box currentBar = dynamic_cast<Box>(scrollBar->GetChild(1));
			
			float newEndOffset = scrollBar->GetRect()->GetLeft() - static_cast<float>(GET_X_LPARAM(lParam)) + currentBar->GetRect()->GetWidth() / 2;
			scroll->SetHorizontalEndOffset(newEndOffset / scrollBar->GetRatio(), SCROLL_ANIMATION_PROPERTIES);
		});

		// Добавление событий текущей полосе
		currentBar->AddEvent(MouseEventType::PRESS, EVENT(block)
		{
			// Проверка видимости полосы прокрутки
			ScrollBar scrollBar = dynamic_cast<ScrollBar>(block->GetParent());
			if (!scrollBar->IsVisible())
				return;

			// Установка начальных значений для перетаскивания
			Scroll scroll = dynamic_cast<Scroll>(scrollBar->GetParent());
			scrollBar->SetDragMouse(static_cast<float>(GET_X_LPARAM(lParam))); 
			scrollBar->SetDragOffset(scroll->GetHorizontalEndOffset());
		});
		currentBar->AddEvent(MouseEventType::HOLD, EVENT(block)
		{
			// Проверка видимости полосы прокрутки
			ScrollBar scrollBar = dynamic_cast<ScrollBar>(block->GetParent());
			if (!scrollBar->IsVisible())
				return;

			// Расчёт нового конечного отступа
			Scroll scroll = dynamic_cast<Scroll>(scrollBar->GetParent());
			float mouseOffset = (scrollBar->GetDragMouse() - static_cast<float>(GET_X_LPARAM(lParam)));
			scroll->SetHorizontalEndOffset(scrollBar->GetDragOffset() + mouseOffset / scrollBar->GetRatio(), {});
		});
	}
	else
	{
		// Установка формулы
		SetFormula(Formula
			{
				SideFormula::CreateGlue(SCROLL_BAR_OFFSET + SCROLL_BAR_SIZE),
				SideFormula::CreateStatic(SCROLL_BAR_OFFSET + SCROLL_BAR_FRAME_OFFSET),
				SideFormula::CreateGlue(SCROLL_BAR_OFFSET),
				SideFormula::CreateGlue(SCROLL_BAR_OFFSET + SCROLL_BAR_FRAME_OFFSET)
			});

		// Добавление событий фантомной полосе
		AddEvent(MouseEventType::PRESS, EVENT(block)
		{
			ScrollBar scrollBar = dynamic_cast<ScrollBar>(block);
			Scroll scroll = dynamic_cast<Scroll>(block->GetParent());
			Box currentBar = dynamic_cast<Box>(scrollBar->GetChild(1));

			float newEndOffset = scrollBar->GetRect()->GetTop() - static_cast<float>(GET_Y_LPARAM(lParam)) + currentBar->GetRect()->GetHeight() / 2;
			scroll->SetVerticalEndOffset(newEndOffset / scrollBar->GetRatio(), SCROLL_ANIMATION_PROPERTIES);
		});

		// Добавление событий текущей полосе
		currentBar->AddEvent(MouseEventType::PRESS, EVENT(block)
		{
			// Проверка видимости полосы прокрутки
			ScrollBar scrollBar = dynamic_cast<ScrollBar>(block->GetParent());
			if (!scrollBar->IsVisible())
				return;

			// Установка начальных значений для перетаскивания
			Scroll scroll = dynamic_cast<Scroll>(scrollBar->GetParent());
			scrollBar->SetDragMouse(static_cast<float>(GET_Y_LPARAM(lParam)));
			scrollBar->SetDragOffset(scroll->GetVerticalEndOffset());
		});
		currentBar->AddEvent(MouseEventType::HOLD, EVENT(block)
		{
			// Проверка видимости полосы прокрутки
			ScrollBar scrollBar = dynamic_cast<ScrollBar>(block->GetParent());
			if (!scrollBar->IsVisible())
				return;

			// Расчёт нового конечного отступа
			Scroll scroll = dynamic_cast<Scroll>(scrollBar->GetParent());
			float mouseOffset = (scrollBar->GetDragMouse() - static_cast<float>(GET_Y_LPARAM(lParam)));
			scroll->SetVerticalEndOffset(scrollBar->GetDragOffset() + mouseOffset / scrollBar->GetRatio(), {});
		});
	}
}

// Перегруженные защищённые методы
void ScrollBarClass::Update()
{
	// Базовое обновление
	CompositeClass::Update();

	// Вычисление переменных в зависимости от ориентации
	Scroll scroll = dynamic_cast<Scroll>(parent);
	float scrollSize, scrollOffset, contextSize, scrollBarSize;
	if (isHorizontal)
	{
		scrollSize = scroll->GetRect()->GetWidth();
		scrollOffset = scroll->GetHorizontalOffset();
		contextSize = scroll->GetWidth();
		scrollBarSize = rect.GetWidth();
	}
	else
	{
		scrollSize = scroll->GetRect()->GetHeight();
		scrollOffset = scroll->GetVerticalOffset();
		contextSize = scroll->GetHeight();
		scrollBarSize = rect.GetHeight();
	}
		
	// Определение видимости полосы прокрутки
	bool isVisible = scrollSize < contextSize;
	if (isVisible != this->isVisible)
	{
		this->isVisible = isVisible;
		phantomBar->SetBrushAlpha(isVisible ? SCROLL_BAR_PHANTOM_ALPHA : 0, SCROLL_ANIMATION_PROPERTIES);
		currentBar->SetBrushAlpha(isVisible ? 1.f : 0, SCROLL_ANIMATION_PROPERTIES);
	}

	// Вычисление соотношения между размером полосы прокрутки и размером контекста
	if (!isVisible)
		contextSize = scrollSize;
	float scrollRatio = scrollBarSize / contextSize;
	SetRatio(scrollRatio);
	
	// Вычисление положения текущей полосы прокрутки
	float currentBarWidth = scrollSize * scrollRatio;
	float currentBarOffset = -scrollOffset * scrollRatio;

	// Установка новых сторон новой формуле
	Formula newFormula = *currentBar->GetFormula();
	BYTE sideID = static_cast<BYTE>(isHorizontal ? SideID::LEFT : SideID::TOP);
	newFormula.SetSide(static_cast<SideID>(sideID), SideFormula::CreateStatic(currentBarOffset));
	newFormula.SetSide(static_cast<SideID>(sideID + 2), SideFormula::CreateStatic(currentBarOffset + currentBarWidth));
	currentBar->SetFormula(newFormula);
}