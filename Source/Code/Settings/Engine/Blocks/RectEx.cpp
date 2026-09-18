#include "RectEx.hpp"

// Конструкторы
RectEx::RectEx(float left, float top, float right, float bottom) { SetSides(left, top, right, bottom); }
RectEx::RectEx(D2RectF rect) : rect(rect) {}
RectEx::RectEx() : rect() {}

// Операторы класса
bool RectEx::operator==(const RectEx& rectEx) const { return !memcmp(&rect, &rectEx.rect, sizeof(rect)); }
RectEx RectEx::operator+(const RectEx& rectEx) const
{
	return RectEx
	{
		rect.left + rectEx.rect.left,
		rect.top + rectEx.rect.top,
		rect.right + rectEx.rect.right,
		rect.bottom + rectEx.rect.bottom
	};
}
RectEx RectEx::operator-(const RectEx& rectEx) const
{
	return RectEx
	{
		rect.left - rectEx.rect.left,
		rect.top - rectEx.rect.top,
		rect.right - rectEx.rect.right,
		rect.bottom - rectEx.rect.bottom
	};
}
RectEx RectEx::operator*(float value) const
{
	return RectEx
	{
		rect.left * value,
		rect.top * value,
		rect.right * value,
		rect.bottom * value
	};
}

// Методы класса
float RectEx::GetSide(SideID id) const { return sides[static_cast<BYTE>(id)]; }
float RectEx::GetLeft() const { return rect.left; }
float RectEx::GetTop() const { return rect.top; }
float RectEx::GetRight() const { return rect.right; }
float RectEx::GetBottom() const { return rect.bottom; }
const D2RectF* RectEx::GetRect() const { return &rect; }
float RectEx::GetWidth() const { return rect.right - rect.left; }
float RectEx::GetHeight() const { return rect.bottom - rect.top; }
void RectEx::Offset(float width, float height)
{
	if (width)
	{
		rect.left += width;
		rect.right += width;
	}
	if (height)
	{
		rect.top += height;
		rect.bottom += height;
	}
}
void RectEx::Inflate(float width, float height)
{
	if (width)
	{
		width /= 2;
		rect.left -= width;
		rect.right += width;
	}
	if (height)
	{
		height /= 2;
		rect.top -= height;
		rect.bottom += height;
	}
}
void RectEx::Intersect(const RectEx* mask)
{
	const D2RectF* maskRect = &mask->rect;
	if (rect.left >= maskRect->right || rect.top >= maskRect->bottom || rect.right <= maskRect->left || rect.bottom <= maskRect->top)
	{
		rect = D2RectF{};
		return;
	}
	if (rect.left < maskRect->left)
		rect.left = maskRect->left;
	if (rect.top < maskRect->top)
		rect.top = maskRect->top;
	if (rect.right > maskRect->right)
		rect.right = maskRect->right;
	if (rect.bottom > maskRect->bottom)
		rect.bottom = maskRect->bottom;
}
bool RectEx::IsPointInside(POINT point) { return point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom; }
void RectEx::SetSide(SideID id, float value) { sides[static_cast<BYTE>(id)] = value; }
void RectEx::SetLeft(float value) { rect.left = value; }
void RectEx::SetTop(float value) { rect.top = value; }
void RectEx::SetRight(float value) { rect.right = value; }
void RectEx::SetBottom(float value) { rect.bottom = value; }
void RectEx::SetRect(D2RectF rect) { this->rect = rect; }
void RectEx::SetSides(float left, float top, float right, float bottom) { SetRect(D2RectF{ left, top, right, bottom }); }