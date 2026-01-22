#pragma once

#include "../input.hpp"

namespace util {

inline float gamepadConvert(const int v) {
	float r = static_cast<float>(v) / 32768.0f;
	if (r > -0.1f && r < 0.1f) {
		r = 0.0f;
	} else if (r > 0.9f) {
		r = 1.0f;
	} else if (r < -0.9f) {
		r = -1.0f;
	}
	return r;
};
}; // namespace util
