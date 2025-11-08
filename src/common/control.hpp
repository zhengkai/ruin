#pragma once

struct ControlAxis {
	float x = 0.0f;
	float y = 0.0f;
};

struct Control {

	bool btnA = false;
	bool btnB = false;
	bool btnX = false;
	bool btnY = false;

	ControlAxis axisA = {};
	ControlAxis axisB = {};
};
