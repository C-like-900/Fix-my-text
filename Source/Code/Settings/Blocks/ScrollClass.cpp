#include "ScrollClass.hpp"
#include "ScrollContextClass.hpp"
#include "ScrollBarClass.hpp"

// Фабрика
Scroll ScrollClass::Create(const wchar_t* name, Block parent, D2SizeF size, const ColorRGBA& color) { return new ScrollClass{ name, parent, size, color }; }

// Методы класса
ScrollContext ScrollClass::GetContext() const { return context; }
float ScrollClass::GetWidth() const { return size.width; }
float ScrollClass::GetHeight() const { return size.height; }
float ScrollClass::GetHorizontalOffset() const { return offset.width; }
float ScrollClass::GetVerticalOffset() const { return offset.height; }
float ScrollClass::GetHorizontalEndOffset() { return context->GetHorizontalEndOffset(); }
float ScrollClass::GetVerticalEndOffset() { return context->GetVerticalEndOffset(); }
void ScrollClass::ScrollEvent(float wheelDelta, bool isHorizontal)
{
	// Расчёт силы прокрутки
	float scrollForce = wheelDelta * SCROLL_FORCE;

	// Применение силы прокрутки
	isHorizontal ? SetHorizontalEndOffset(scrollForce + GetHorizontalEndOffset(), SCROLL_ANIMATION_PROPERTIES) : SetVerticalEndOffset(scrollForce + GetVerticalEndOffset(), SCROLL_ANIMATION_PROPERTIES);
}
void ScrollClass::SetWidth(float value, AnimationProperties properties) { CreateAnimation(&size.width, value, this, properties, true); }
void ScrollClass::SetHeight(float value, AnimationProperties properties) { CreateAnimation(&size.height, value, this, properties, true); }
void ScrollClass::SetHorizontalOffset(float value) { offset.width = value; }
void ScrollClass::SetVerticalOffset(float value) { offset.height = value; }
void ScrollClass::SetHorizontalEndOffset(float value, AnimationProperties properties)
{
	// Изменение конечного отступа
	context->SetHorizontalEndOffset(value);

	// Создание анимации отсутпа
	CreateAnimation(&offset.width, value, this, properties, true);
}
void ScrollClass::SetVerticalEndOffset(float value, AnimationProperties properties)
{
	// Изменение конечного отступа
	context->SetVerticalEndOffset(value);
	
	// Создание анимации отсутпа
	CreateAnimation(&offset.height, value, this, properties, true);
}

// Конструктор
ScrollClass::ScrollClass(const wchar_t* name, Block parent, D2SizeF size, const ColorRGBA& color) : CompositeClass{ name, parent }, size(size), offset()
{
	// Создание контекста прокрутки
	context = ScrollContextClass::Create(L"Scroll Context", this, size);
	
	// Создание полос прокруток
	scrollBarH = ScrollBarClass::Create(L"Horizontal ScrollBar", this, color, true);
	scrollBarV = ScrollBarClass::Create(L"Vertical ScrollBar", this, color, false);

	// Добавление обрезки
	AddCropping();

	// Создание ивента прокрутки мыши
	AddEvent(MouseEventType::SCROLL, EVENT(block)
	{
		// Получение прокрутки
		Scroll scroll = dynamic_cast<Scroll>(block);
		
		// Вызов события прокрутки
		scroll->ScrollEvent(static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA, GET_KEYSTATE_WPARAM(wParam) & MK_SHIFT);
	});
}