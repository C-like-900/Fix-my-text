#include "LogFrame.hpp"

// Конструктор
LogFrame::LogFrame(const wchar_t* start, const wchar_t* end) : start(start), end(end) {}

// Методы класса
const wchar_t* LogFrame::GetStart() const { return start; }
const wchar_t* LogFrame::GetEnd() const { return end; }