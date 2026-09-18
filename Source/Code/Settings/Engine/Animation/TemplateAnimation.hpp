#pragma once
#include "AnimationClass.hpp"
#include "BlockClass.hpp"

// Определение класса
template <typename T>
class TemplateAnimation : public AnimationClass
{
public:
	// Фабрика
	static TemplateAnimation<T>* Create(T* reference, const T& newValue, Block sponsor, const AnimationProperties& properties, bool needUpdateSponsor);

	// Перегруженные методы класса
	void* GetReference() const override;
	void Calculate(float progress) override;
	void SetStart() override;

private:
	// Поля класса
	T* reference;
	T start;
	T end;

	// Конструтор
	TemplateAnimation(T* reference, const T& newValue, Block sponsor, const AnimationProperties& properties, bool needUpdateSponsor);
};

// Шаблонная фабрика
template<typename T>
TemplateAnimation<T>* TemplateAnimation<T>::Create(T* reference, const T& newValue, Block sponsor, const AnimationProperties& properties, bool needUpdateSponsor)
{
	return new TemplateAnimation<T>{ reference, newValue, sponsor, properties, needUpdateSponsor };
}

// Шаблонные методы
template<typename T>
void* TemplateAnimation<T>::GetReference() const { return reinterpret_cast<void*>(reference); }
template<typename T>
void TemplateAnimation<T>::Calculate(float progress)
{
	// Вычисление текущего значения
	*reference = start + (end - start) * progress;

	// Обновление спонсора, если необходимо
	if (needUpdateSponsor)
		sponsor->RequestUpdate();
}
template<typename T>
void TemplateAnimation<T>::SetStart() { start = *reference; }

// Шаблонный конструктор
template <typename T>
TemplateAnimation<T>::TemplateAnimation(T* reference, const T& newValue, Block sponsor, const AnimationProperties& properties, bool needUpdateSponsor)
	: AnimationClass(sponsor, properties, needUpdateSponsor), reference(reference), start(), end(newValue) {}
