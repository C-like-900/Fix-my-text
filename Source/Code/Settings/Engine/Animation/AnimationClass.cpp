#include "AnimationClass.hpp"

// Методы класса
const Block AnimationClass::GetSponsor() const { return sponsor; }
const AnimationProperties* AnimationClass::GetProperties() const { return &properties; }
TIME AnimationClass::GetLaunch() const { return launch; }
bool AnimationClass::IsNeedUpdateSponsor() const { return needUpdateSponsor; }
void AnimationClass::ExecuteOnEndEvent()
{
	// Получение ивента окончания анимации
	Event onEnd = properties.GetOnEndEvent();

	// Проверка на существование
	if (onEnd)
		onEnd(sponsor, 0, 0);
}
void AnimationClass::SetLaunch(TIME launch) { this->launch = launch; }

// Виртуальные методы класса
void* AnimationClass::GetReference() const { return 0; }
void AnimationClass::Calculate(float progress) {}
void AnimationClass::SetStart() {}

// Приватный конструктор
AnimationClass::AnimationClass(Block sponsor, AnimationProperties properties, bool needUpdateSponsor) : sponsor(sponsor), properties(properties), launch(0), needUpdateSponsor(needUpdateSponsor) {}