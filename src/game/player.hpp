#pragma once

#include "../common/pose.hpp"

class Player {

public:
	int serial = 0;
	Pose pose = {};

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
};
