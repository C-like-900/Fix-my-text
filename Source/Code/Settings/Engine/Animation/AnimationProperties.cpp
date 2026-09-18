#include "AnimationProperties.hpp"
#include <cmath>

// Конструкторы
AnimationProperties::AnimationProperties(TIME delay, TIME duration, AnimationCurve curve, Event onEnd) : delay(delay), duration(duration), curve(curve), onEnd(onEnd) {}

// Методы класса
TIME AnimationProperties::GetDelay() const { return delay; }
TIME AnimationProperties::GetDuration() const { return duration; }
AnimationCurve AnimationProperties::GetCurve() const { return curve; }
Event AnimationProperties::GetOnEndEvent() const { return onEnd; }
float AnimationProperties::CurveCalculate(float x) const
{
	// Просчёт кривой анимации
	switch (curve)
	{
	case AnimationCurve::EASE_OUT_QUAD:
	{
		x--;
		return 1 - powf(x, 2);
	}
	case AnimationCurve::EASE_IN_OUT_QUARTIC:
		return x < 0.5f ? 32 * powf(x, 6) : 1 - 32 * powf(x - 1, 6);
	case AnimationCurve::EASE_OUT_OVERSHOOT:
	{
		x--;
		return 1 + 2.7f * powf(x, 3) + 1.7f * powf(x, 2);
	}
	default:
		return x;
	}
}
void AnimationProperties::SetDelay(TIME delay) { this->delay = delay; }
void AnimationProperties::SetDuration(TIME duration) { this->duration = duration; }
void AnimationProperties::SetCurve(AnimationCurve curve) { this->curve = curve; }
void AnimationProperties::SetOnEndEvent(Event onEnd) { this->onEnd = onEnd; }
