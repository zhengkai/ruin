#pragma once

#include <random>

namespace util {

static std::bernoulli_distribution distBool(0.5);

inline std::mt19937 &rng() {

	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}

inline bool randBool() {
	return distBool(rng());
}
} // namespace util
