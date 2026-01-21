#pragma once

#include "../asset/map.hpp"
#include "../config.hpp"
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

struct Focus {
	float x = 10.0f;
	float y = 10.0f;

	float offsetX = 0.0f; // 实际转视角的偏差修正
	float offsetY = 0.0f;
};

struct Camera {
	float zoom = 3.5f;
	float cx = 0.0f; // cx/cy (中心像素的坐标)
	float cy = 0.0f;
	float gridSize = 1.0f;
};

struct Boundary {
	float down = 0.0f;
	float up = 100.0f;
	float left = 0.0f;
	float right = 100.0f;
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

public:
	float w = 800.0f;
	float h = 800.0f;
	float scale = 1.0f;
	float cellSize = 0.0f;
	float startX = 0.0f;
	float startY = 0.0f;
	float gridSize = 0.0f;
	WinResize winResize = {};
	bool showBall = true;
	bool toggleFullscreen = false;
	Control control = {};
	Global global = {};
	Focus focus = {};
	Camera camera = {};
	const asset::Map *map = nullptr;

private:
	Boundary boundary = {};
	Boundary focusBoundary = {};

public:
	void setSize(float inW, float inH) {
		w = inW;
		h = inH;
	}

	void calcGrid(float w, float h) {

		this->w = w;
		this->h = h;

		float gw = config::gridWF;
		float gh = config::gridHF;

		float gs = std::floor(w / gw < h / gh ? w / gw : h / gh);

		gridSize = gs;
		spdlog::info("gridSize = {}, win pixel = {}x{}", gs, w, h);

		startX = std::floor((w - (gs * gw)) / 2);
		startY = std::floor((h - (gs * gh)) / 2);
		spdlog::info("cell num: {}x{}={}, grid pixel: {}x{},  start: x={},y={}",
			config::gridW,
			config::gridH,
			config::gridW * config::gridH,
			gs * config::gridW,
			gs * config::gridH,
			startX,
			startY);

		calcCamera();
	}

	void setFocus(float x, float y) {
		if (x < focusBoundary.left) {
			x = focusBoundary.left;
		} else if (x > focusBoundary.right) {
			x = focusBoundary.right;
		}
		focus.x = x;

		if (y < focusBoundary.down) {
			y = focusBoundary.down;
		} else if (y > focusBoundary.up) {
			y = focusBoundary.up;
		}
		focus.y = y;
	};

	void setFocus(physics::Pos &pos) {
		setFocus(pos.x, pos.y);
	};

	void setBoundary(int x, int y) {
		boundary.right = static_cast<float>(x - 1);
		boundary.up = static_cast<float>(y - 1);
		calcBoundary();
	};

	void calcCameraOffset(SDL_FRect &r) {
		r.x = camera.cx -
			camera.gridSize *
				(focus.x + focus.offsetX * config::focusRange - r.x +
					r.w / 2.0f);
		r.y = camera.cy +
			camera.gridSize *
				(focus.y + focus.offsetY * config::focusRange - r.y -
					r.h / 2.0f);
		r.w *= camera.gridSize;
		r.h *= camera.gridSize;
	};

	void zoomIn() {
		if (camera.zoom < 5.0f) {
			camera.zoom += 0.125f;
			calcCamera();
		}
	};
	void zoomOut() {
		if (camera.zoom > 1.0f) {
			camera.zoom -= 0.125f;
			calcCamera();
		}
	};

private:
	void calcCamera() {
		camera.gridSize = gridSize * camera.zoom;
		camera.cx = std::round(w / 2.0f);
		camera.cy = std::round(h / 2.0f);
		calcBoundary();
	}
	void calcBoundary() {
		auto hw = w / 2.0f / camera.gridSize - 0.5f;
		focusBoundary.left = hw;
		focusBoundary.right = boundary.right - hw;

		auto hh = h / 2.0f / camera.gridSize - 0.5f;
		focusBoundary.down = hh;
		focusBoundary.up = boundary.up - hh;
	}
};
}; // namespace context
