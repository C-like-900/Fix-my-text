#pragma once
#include <Windows.h>
#include <AnimationProperties.hpp>

// Магические значения
const BYTE SCROLL_FORCE = 50U;
const BYTE SCROLL_ANIMATION_DURATION = 200U;
const AnimationProperties SCROLL_ANIMATION_PROPERTIES{ 0, SCROLL_ANIMATION_DURATION, AnimationCurve::EASE_OUT_QUAD };

// Предварительное объявление класса
class ScrollClass;

// Создание псевдонимов класса
using Scroll = ScrollClass*;