#pragma once

#include "../config.hpp"
#include "../tag.hpp"
#include "common.hpp"

namespace physics {

struct Body {
	Touch touch = {};
	float speed;
	float vx = 0.0f;
	float vy = 0.0f;
	bool gravity = true;

	Body(const tag::Speed &s = tag::defaultWalkSpeed) {
		setSpeed(s);
	};

	void setSpeed(const tag::Speed &s) {
		speed = s.v * config::deltaTime;
	};
};

}; // namespace physics
