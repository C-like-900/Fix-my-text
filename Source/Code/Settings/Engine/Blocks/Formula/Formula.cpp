#include "Formula.hpp"
#include "BlockClass.hpp"

// Инициализация статчексих переменных
const Formula Formula::defaultFormula{};


// Конструкторы
Formula::Formula(const SideFormula& left, const SideFormula& top, const SideFormula& right, const SideFormula& bottom,
	const Limit& minWidth, const Limit& minHeight, const Limit& maxWidth, const Limit& maxHeight)
	: left(left), top(top), right(right), bottom(bottom), minWidth(minWidth), minHeight(minHeight), maxWidth(maxWidth), maxHeight(maxHeight) {}
Formula::Formula(const SideFormula& left, const SideFormula& top, const SideFormula& right, const SideFormula& bottom)
	: Formula{ left, top, right, bottom, Limit::CreateNothing(), Limit::CreateNothing(), Limit::CreateNothing(), Limit::CreateNothing() } {}
Formula::Formula() : Formula{ SideFormula::CreateStatic(), SideFormula::CreateStatic(), SideFormula::CreateGlue(), SideFormula::CreateGlue() } {}

// Операторы класса
Formula Formula::operator+(const Formula& formula) const
{
	return Formula
	{
		left + formula.left, top + formula.top,	right + formula.right, bottom + formula.bottom,
		minWidth + formula.minWidth, minHeight + formula.minHeight,	maxWidth + formula.maxWidth, maxHeight + formula.maxHeight
	};
}
Formula Formula::operator-(const Formula& formula) const
{
	return Formula
	{
		left - formula.left, top - formula.top, right - formula.right, bottom - formula.bottom,
		minWidth - formula.minWidth, minHeight - formula.minHeight, maxWidth - formula.maxWidth, maxHeight - formula.maxHeight
	};
}
Formula Formula::operator*(float value) const
{
	return Formula
	{
		left * value, top * value, right * value, bottom * value,
		minWidth * value, minHeight * value, maxWidth * value, maxHeight * value
	};
}

// Методы класса
const SideFormula* Formula::GetSide(SideID id) const { return sides + static_cast<BYTE>(id); }
const SideFormula* Formula::GetLeftSide() const { return &left; }
const SideFormula* Formula::GetTopSide() const { return &top; }
const SideFormula* Formula::GetRightSide() const { return &right; }
const SideFormula* Formula::GetBottomSide() const { return &bottom; }
const Limit* Formula::GetLimit(LimitID id) const { return limits + static_cast<BYTE>(id); }
const Limit* Formula::GetMinWidth() const { return &minWidth; }
const Limit* Formula::GetMinHeight() const { return &minHeight; }
const Limit* Formula::GetMaxWidth() const { return &maxWidth; }
const Limit* Formula::GetMaxHeight() const { return &maxHeight; }
RectEx Formula::Calculate(Block block) const
{
	// Создание результата
	RectEx result{};

	// Просчёт формулы каждой стороны
	RectEx parentRect = *block->GetParent()->GetRect();
	for (BYTE i = 0; i != RECT_SIZE; i++)
	{
		// Получение стороны
		const SideFormula* side = GetSide(static_cast<SideID>(i));

		// Просчёт стороны
		float calculatedSide =	parentRect.GetSide(static_cast<SideID>(i & 0b01)) * side->GetLeftTop() +
								parentRect.GetSide(static_cast<SideID>(i | 0b10)) * side->GetRightBottom() +
								side->GetConstant();

		// Сохранение результата
		result.SetSide(static_cast<SideID>(i), calculatedSide);
	}

	// Просчёт лимитов
	for (BYTE i = 0; i != LIMITS_SIZE; i++)
	{
		// Получение лимита
		const Limit* limit = GetLimit(static_cast<LimitID>(i));

		// Проверка выхода за лимит
		float rectSize = (i & 1) ? result.GetHeight() : result.GetWidth();
		float limitSize = limit->GetSize();		
		if (rectSize == limitSize || (i < 2) == (rectSize > limitSize))
			continue;

		// Получение сторон
		SideID startID = static_cast<SideID>(i & 0b01);	// left || top
		SideID endID = static_cast<SideID>(i | 0b10);	// right || bottom
		float start = result.GetSide(startID);
		float end = result.GetSide(endID);

		// Пересчёт с выравниваниями
		float leftTop = limit->GetLeftTop();
		float center = limit->GetCenter();
		float rightBottom = limit->GetRightBottom();
		float nonexistence = limit->GetNonexistence();
		float difference = (limitSize - rectSize) / 2.f;
		float newStart = leftTop * start + center * (start - difference) + rightBottom * (end - limitSize) + start * nonexistence;
		float newEnd = leftTop * (start + limitSize) + center * (end + difference) + rightBottom * end + end * nonexistence;;

		// Сохранение результата
		result.SetSide(startID, newStart);
		result.SetSide(endID, newEnd);
	}

	// Возврат результата
	return result;
}
void Formula::SetSide(SideID id, const SideFormula& side) { sides[static_cast<BYTE>(id)] = side; }
void Formula::SetLeftSide(const SideFormula& side) { left = side; }
void Formula::SetTopSide(const SideFormula& side) { top = side; }
void Formula::SetRightSide(const SideFormula& side) { right = side; }
void Formula::SetBottomSide(const SideFormula& side) { bottom = side; }
void Formula::SetSides(const SideFormula& left, const SideFormula& top, const SideFormula& right, const SideFormula& bottom)
{
	SetLeftSide(left);
	SetTopSide(top);
	SetRightSide(right);
	SetBottomSide(bottom);
}
void Formula::SetLimit(LimitID id, const Limit& limit) { limits[static_cast<BYTE>(id)] = limit; }
void Formula::SetMinWidth(const Limit& limit) { minWidth = limit; }
void Formula::SetMinHeight(const Limit& limit) { minHeight = limit; }
void Formula::SetMaxWidth(const Limit& limit) { maxWidth = limit; }
void Formula::SetMaxHeight(const Limit& limit) { maxHeight = limit; }