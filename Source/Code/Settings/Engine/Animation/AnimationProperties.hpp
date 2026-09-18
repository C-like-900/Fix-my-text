#pragma once
#include "AnimationProperties.h"
#include "Event.h"

// Определение класса
class AnimationProperties
{
public:
	// Конструкторы
	AnimationProperties(TIME delay = 0, TIME duration = 0, AnimationCurve curve = AnimationCurve::LINEAR, Event onEnd = nullptr);

	// Методы класса
	TIME GetDelay() const;
	TIME GetDuration() const;
	AnimationCurve GetCurve() const;
	Event GetOnEndEvent() const;
	float CurveCalculate(float progress) const;
	void SetDelay(TIME delay);
	void SetDuration(TIME duration);
	void SetCurve(AnimationCurve curve);
	void SetOnEndEvent(Event onEnd);

private:
	// Поля класса
	TIME delay;
	TIME duration;
	AnimationCurve curve;
	Event onEnd;
};
