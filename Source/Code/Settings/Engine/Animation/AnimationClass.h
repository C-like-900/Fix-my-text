#pragma once
#include <vector>

// Магические числа
const float ANIMATION_PROGRESS_START = 0.f;
const float ANIMATION_PROGRESS_END = 1.f;

// Предварительное объявление класса
class AnimationClass;

// Создание псевдонимов класса
using Animation = AnimationClass*;
using AnimationVector = std::vector<Animation>;