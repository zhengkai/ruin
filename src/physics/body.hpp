#pragma once

#include "common.hpp"
#include "rect.hpp"
#include <cstddef>

namespace physics {

struct Body : Rect, Speed {
	std::size_t serial;
	bool enable = true;
	Touch touch = {};
	IntersectEvent eventOverall = {};
	float vx = 0.0f;
	float vy = 0.0f;
	bool gravity = true;
};

inline Body newBody(std::size_t serial, float x, float y, float w, float h) {
	return Body{Rect{Pos{x, y}, w, h}, Speed{}, serial};
}

}; // namespace physics
