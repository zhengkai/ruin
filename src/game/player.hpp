#pragma once

#include "../common/control.hpp"
#include "../common/pose.hpp"

class Player {

public:
	int serial = 0;
	Pose pose = {};
	bool lastRight = true;

public:
	Player() {};
	~Player() {};

	void next() {
		serial++;
		if (serial > 20) {
			serial = 0;
			pose.step++;
		}
	}

	void parseFacing(const Control &control) {

		bool right = lastRight;
		if (control.axisA.x > 0.0f) {
			right = true;
		} else if (control.axisA.x < 0.0f) {
			right = false;
		}
		if ((pose.facing == Pose::Facing::Right) != right) {
			lastRight = right;
			pose.step = 0;
			pose.facing = right ? Pose::Facing::Right : Pose::Facing::Left;
		}
	}
};
