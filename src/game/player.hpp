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
		if (serial > 10) {
			serial = 0;
			pose.step++;
		}
	}

	void parse(const Control &control) {
		parseFacing(control);
		parseAttack(control);
	}

	void parseAttack(const Control &control) {
		if (pose.type == Pose::Type::Attack1) {
		}
		if (!control.btnA) {
			return;
		}
		if (pose.type == Pose::Type::Attack1) {

		} else {
			pose.type = Pose::Type::Attack1;
			pose.step = 0;
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
