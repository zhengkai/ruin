#pragma once

namespace context {

struct ControlAxis {
	int x = 0;
	int y = 0;
	bool hasX = false;
	bool hasY = false;
};

struct Control {

	bool btnA = false;
	bool btnB = false;
	bool btnX = false;
	bool btnY = false;

	ControlAxis axisA = {};
	ControlAxis axisB = {};
};
}; // namespace context
