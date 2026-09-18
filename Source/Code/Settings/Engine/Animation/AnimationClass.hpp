#pragma once
#include "AnimationClass.h"
#include "BlockClass.h"
#include "AnimationProperties.hpp"

// Определение класса
class AnimationClass
{
public:
	// Деструктор
	virtual ~AnimationClass() = default;

	// Методы класса
	const Block GetSponsor() const;
	const AnimationProperties* GetProperties() const;
	TIME GetLaunch() const;
	bool IsNeedUpdateSponsor() const;
	void ExecuteOnEndEvent();
	void SetLaunch(TIME launch);

	// Виртуальные методы класса
	virtual void* GetReference() const;
	virtual void Calculate(float progress);
	virtual void SetStart();

protected:
	// Защищённые переменные класса
	Block sponsor;
	AnimationProperties properties;
	TIME launch;
	bool needUpdateSponsor;

	// Приватный конструктор
	AnimationClass(Block sponsor, AnimationProperties properties, bool needUpdateSponsor);
};