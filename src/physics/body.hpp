#pragma once

#include "common.hpp"
#include <cstddef>

namespace physics {

struct Body : Box {
	std::size_t serial;
	bool enable = true;
	IntersectResult intersect = {};
	float vx = 0.0f;
	float vy = 0.0f;
};

inline Body newBody(std::size_t serial, float x, float y, float w, float h) {
	return Body{Box{x, y, w, h}, serial};
}

}; // namespace physics
