#pragma once

#include "brick.hpp"
#include <box2d/box2d.h>

namespace context {

struct Misc {
	int hit = 0;
	float speed = 1.0f;
	int speedLevel = 0;
	std::vector<Brick> brick;
	float gamepadX = 0.0f;
	float gamepadY = 0.0f;
};
}; // namespace context
