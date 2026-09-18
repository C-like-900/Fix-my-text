#pragma once
#include <Windows.h>

// Магические значения
const BYTE LIMITS_SIZE = 4;

// Связанные типы данных
enum class LimitID : BYTE { MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT };

// Предварительное объявление класса
class Formula;