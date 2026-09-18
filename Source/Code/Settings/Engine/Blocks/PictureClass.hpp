#pragma once
#include "PictureClass.h"
#include "BlockClass.hpp"

// Определение класса
class PictureClass : public BlockClass
{
public:
	// Фабрика
	static Picture Create(const wchar_t* name = DEFAULT_NAME, Block parent = 0, const wchar_t* path = L"");

	// Перегруженные методы
	const BlockType GetType() const override;

protected:
	// Поля класса
	D2Bitmap bitmap;

	// Конструктор и деструкотр
	PictureClass(const wchar_t* name, Block parent, const wchar_t* path);
	~PictureClass();

	// Перегруженные методы
	void Paint() override;
};

