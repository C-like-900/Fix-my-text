#pragma once
#include <Windows.h>
#include "Aliases.h"

// Связанные типы данных
using TIME = DLONG;
enum class AnimationCurve : BYTE { LINEAR, EASE_OUT_QUAD, EASE_IN_OUT_QUARTIC, EASE_OUT_OVERSHOOT };

// Предварительное объявление класса
class AnimationProperties;