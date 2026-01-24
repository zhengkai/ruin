#pragma once

#include "../asset/map.hpp"
#include "../camera.hpp"
#include "monitor.hpp"
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>

namespace context {

struct ControlAxis {
	float x = 0.0f;
	float y = 0.0f;
};

struct Control {

	bool btnA = false;
	bool btnB = false;
	bool btnX = false;
	bool btnY = false;

	bool btnU = false;
	bool btnD = false;
	bool btnL = false;
	bool btnR = false;

	bool btnRB = false;
	bool btnLB = false;

	float btnLT = 0.0f;
	float btnRT = 0.0f;

	ControlAxis axisA = {};
	ControlAxis axisB = {};
};

struct WinResize {
	int w = 0;
	int h = 0;
	bool trigger = false;
};

struct Event {
	bool fullscreeen = false;
};

struct Global {
	int serial = 0;

public:
	bool counter(int &cnt, int add) const {
		if (cnt > serial) {
			return false;
		}
		cnt = serial + add;
		return true;
	}
};

struct Window {
	FrameMonitor frameMonitor = {};
	Event event = {};
	bool quit = false;
	bool focus = true;
	bool toggleFullscreen = false;
	Control control = {};
	Global global = {};
	Camera camera = {};
	const asset::Map *map = nullptr;
	asset::MapTarget enterMap = {};
};
}; // namespace context
