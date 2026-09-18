#pragma once

// Определение класса
class LogFrame
{
public:
	// Конструктор
	LogFrame(const wchar_t* start = L"", const wchar_t* end = L"");

	// Методы класса
	const wchar_t* GetStart() const;
	const wchar_t* GetEnd() const;

private:
	// Поля класса
	const wchar_t* start;
    const wchar_t* end;
};