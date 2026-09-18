#pragma once
#include "Formula.h"
#include "SideFormula.hpp"
#include "BlockClass.h"
#include "RectEx.hpp"
#include "Limit.hpp"

// Определение класса
class Formula
{
public:
	// Статические переменнные
	const static Formula defaultFormula;

	// Конструкторы
	Formula(const SideFormula& left, const SideFormula& top, const SideFormula& right, const SideFormula& bottom,
		const Limit& minWidth, const Limit& minHeight, const Limit& maxWidth, const Limit& maxHeight);
	Formula(const SideFormula& left, const SideFormula& top, const SideFormula& right, const SideFormula& bottom);
	Formula();

	// Операторы класса
	Formula operator+(const Formula& formula) const;
	Formula operator-(const Formula& formula) const;
	Formula operator*(float value) const;

	// Методы класса
	const SideFormula* GetSide(SideID id) const;
	const SideFormula* GetLeftSide() const;
	const SideFormula* GetTopSide() const;
	const SideFormula* GetRightSide() const;
	const SideFormula* GetBottomSide() const;
	const Limit* GetLimit(LimitID id) const;
	const Limit* GetMinWidth() const;
	const Limit* GetMinHeight() const;
	const Limit* GetMaxWidth() const;
	const Limit* GetMaxHeight() const;
	RectEx Calculate(Block block) const;
	void SetSide(SideID id, const SideFormula& side);
	void SetLeftSide(const SideFormula& side);
	void SetTopSide(const SideFormula& side);
	void SetRightSide(const SideFormula& side);
	void SetBottomSide(const SideFormula& side);
	void SetSides(const SideFormula& left, const SideFormula& top, const SideFormula& right, const SideFormula& bottom);
	void SetLimit(LimitID id, const Limit& limit);
	void SetMinWidth(const Limit& limit);
	void SetMinHeight(const Limit& limit);
	void SetMaxWidth(const Limit& limit);
	void SetMaxHeight(const Limit& limit);

private:
	// Поля класса
	union
	{
		SideFormula sides[RECT_SIZE];
		struct
		{
			SideFormula left;
			SideFormula top;
			SideFormula right;
			SideFormula bottom;
		};
	};
	union
	{
		Limit limits[LIMITS_SIZE];
		struct
		{
			Limit minWidth;
			Limit minHeight;
			Limit maxWidth;
			Limit maxHeight;
		};
	};
};
