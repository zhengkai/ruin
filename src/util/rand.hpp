#pragma once

#include <random>

namespace util {

static std::bernoulli_distribution distBool(0.5);
static std::uniform_real_distribution<float> distFloat(0.0f, 1.0f);

inline std::mt19937 &rng() {

	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
};

inline bool randBool() {
	return distBool(rng());
};

inline float randFloat() {
	return distFloat(rng());
};
} // namespace util
