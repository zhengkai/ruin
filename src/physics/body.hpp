#pragma once

#include "common.hpp"

namespace physics {

struct Body {
	std::size_t serial;
	bool enable = true;
	Position pos;
	AABB box;
};

inline Body newBody(std::size_t serial, float x, float y, float w, float h) {
	return Body{
		.serial = serial, .pos = {.x = x, .y = y}, .box = {.w = w, .h = h}};
}

}; // namespace physics
