#pragma once
#include "BlockClass.h"
#include "Aliases.h"
#include "Graphics.hpp"
#include "RectEx.hpp"
#include "Formula.hpp"
#include "MouseEvents.hpp"
#include "AnimationProperties.hpp"
#include "TemplateAnimation.hpp"

// Определение класса
class BlockClass
{
public:
	// Дружественные классы
	friend Graphics;

	// Фабрика
	static Block Create(const wchar_t* name = DEFAULT_NAME, Block parent = nullptr);

	// Методы класса
	void AddHoverHitbox();
	void AddPressHitbox();
	void AddScrollHitbox();
	void AddCropping();
	void AddEvent(MouseEventType type, Event event);
	const RectEx* GetRect() const;
	const Formula* GetFormula() const;
	Block GetChild(USHORT id) const;
	POINT GetCursorLocalPoint(POINT point);
	void RequestUpdate();
	void ExecuteEvent(MouseEventType type, WPARAM wParam = 0, LPARAM lParam = 0);
	void Destroy();

	// Виртуальные методы
	virtual const BlockType GetType() const;
	virtual Block GetParent() const;
	virtual void SetFormula(const Formula& formula, const AnimationProperties& properties = {});
	
protected:
	// Статические переменнные
	static UDLONG count;

	// Поля класса
	UDLONG id;
	const wchar_t* name;
	RectEx rect;
	Formula* formula;
	Block parent;
	BlockVector* children;
	RectEx* hitbox;
	Block cropper;
	MouseEvents* events;
	bool needUpdate;
	bool handlesHover;
	bool handlesPress;
	bool handlesScroll;
	bool isCropper;

	// Конструктор и деструкотр
	BlockClass(const wchar_t* name, Block parent);
	virtual ~BlockClass();

	// Защищённые методы
	void AddHitbox();
	template <typename T>
	void CreateAnimation(T* reference, const T& newValue, Block sponsor, const AnimationProperties& properties, bool needUpdateSponsor = false);

	// Защищённые виртуальные методы
	virtual void Update();
	virtual void Paint();
};

// Шаблонные методы
template<typename T>
void BlockClass::CreateAnimation(T* reference, const T& newValue, Block sponsor, const AnimationProperties& properties, bool needUpdateSponsor)
{
	// Создание анимации
	Graphics::graphics->CreateAnimationRequest(TemplateAnimation<T>::Create(reference, newValue, sponsor, properties, needUpdateSponsor));
}