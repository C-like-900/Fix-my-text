#include "BlockClass.hpp"
#include "RootClass.hpp"
#include <stdexcept>

// Инициализация статчексих переменных
UDLONG BlockClass::count = 0;

// Фабрика
Block BlockClass::Create(const wchar_t* name, Block parent) { return new BlockClass{ name, parent }; }

// Методы класса
void BlockClass::AddHoverHitbox()
{
	// Проверка на существование
	if (handlesHover)
		throw std::runtime_error("Duplicating the HoverHitbox");
	handlesHover = true;

	// Создание хитбокса
	AddHitbox();
}
void BlockClass::AddPressHitbox()
{
	// Проверка на существование
	if (handlesPress)
		throw std::runtime_error("Duplicating the PressHitbox");
	handlesPress = true;

	// Создание хитбокса
	AddHitbox();
}
void BlockClass::AddScrollHitbox()
{
	// Проверка на существование
	if (handlesScroll)
		throw std::runtime_error("Duplicating the ScrollHitbox");
	handlesScroll = true;

	// Создание хитбокса
	AddHitbox();
}
void BlockClass::AddCropping()
{
	// Проверка на существование
	if (isCropper)
		throw std::runtime_error("Duplicating the cropping");
	isCropper = true;
	
	// Создание хитбокса
	if (!hitbox)
		hitbox = new RectEx{};

	// Пометка о пересчёте вектора блоков
	Graphics::graphics->RebuildRequest();
}
void BlockClass::AddEvent(MouseEventType type, Event event)
{
	// Создание ивентов
	if (!this->events)
		this->events = new MouseEvents{};

	switch (type)
	{
	case MouseEventType::ENTER:
	case MouseEventType::STAY:
	case MouseEventType::EXIT:
	{
		if (!handlesHover)
			AddHoverHitbox();
		break;
	}
	case MouseEventType::PRESS:
	case MouseEventType::HOLD:
	case MouseEventType::RELEASE:
	{
		if (!handlesPress)
			AddPressHitbox();
		break;
	}
	case MouseEventType::SCROLL:
	{
		if (!handlesScroll)
			AddScrollHitbox();
		break;
	}
	default: break;
	}

	// Установка ивента
	this->events->SetEvent(type, event);
}
const RectEx* BlockClass::GetRect() const { return &rect; }
const Formula* BlockClass::GetFormula() const { return formula ? formula : &Formula::defaultFormula; }
Block BlockClass::GetChild(USHORT id) const
{
	// Проверки на доступ
	if (!children)
		throw std::runtime_error("The block has no children");

	// Получение ребёнка
	if (id >= children->size())
		throw std::runtime_error("Child with this id does not exist");

	// Возврат ребёнка
	return (*children)[id];
}
POINT BlockClass::GetCursorLocalPoint(POINT point) { return { static_cast<LONG>(point.x - rect.GetLeft()), static_cast<LONG>(point.y - rect.GetTop()) }; }
void BlockClass::RequestUpdate() { Graphics::graphics->UpdateRequest(this); }
void BlockClass::ExecuteEvent(MouseEventType type, WPARAM wParam, LPARAM lParam)
{
	if(events)
		events->ExecuteEvent(type, this, wParam, lParam);
}
void BlockClass::Destroy() { Graphics::graphics->DestroyBlockRequest(this); }

// Виртуальные методы
const BlockType BlockClass::GetType() const { return BlockType::BLOCK; }
Block BlockClass::GetParent() const { return parent; }
void BlockClass::SetFormula(const Formula& formula, const AnimationProperties& properties)
{
	// Установка стандартной формулы
	if (!this->formula)
		this->formula = new Formula{};

	// Создание анимации
	CreateAnimation(this->formula, formula, this, properties, true);
}

// Конструктор и деструкотр
BlockClass::BlockClass(const wchar_t* name, Block parent) : id(count++), name(name), rect(), formula(nullptr), parent(parent), children(nullptr),
	hitbox(nullptr), cropper(nullptr), events(nullptr), needUpdate(false), handlesHover(false), handlesPress(false), handlesScroll(false), isCropper(false)
{
	// Установка родительского компонента
	if (!parent)
		this->parent = Graphics::graphics->GetRoot();

	// Добавление ребёнка родителю
	if (this->parent)
	{
		if (this->parent->children)
			this->parent->children->push_back(this);
		else this->parent->children = new BlockVector{ this };
	}
	
	// Пометка о пересчёте вектора блоков
	if (Graphics::graphics->GetWindow())
		Graphics::graphics->RebuildRequest();
}
BlockClass::~BlockClass()
{
	// Удаление всех динамических переменных
	if (formula) delete formula;
	if (children) delete children;
	if (hitbox) delete hitbox;
	if (events) delete events;
}

// Защищённые методы
void BlockClass::AddHitbox()
{
	// Создание хитбокса
	if (!hitbox)
		hitbox = new RectEx{};

	// Запрос на обновления верхего блока
	Graphics::graphics->HoverUpdateRequest();
}

// Защищённые виртуальные методы
void BlockClass::Update()
{
	// Просчёт формулы
	rect = (formula ? formula : &Formula::defaultFormula)->Calculate(this);

	// Расчёт хитбокса
	if (hitbox)
	{
		*hitbox = rect;
		if (cropper)
			hitbox->Intersect(cropper->GetRect());
	}
}
void BlockClass::Paint() {}	
