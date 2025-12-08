#pragma once

#include "../config.hpp"
#include "entity.hpp"
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
	int w = 800;
	int h = 800;
	float wf = 800.0f;
	float hf = 800.0f;
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

private:
	Camera camera = {};
	Boundary boundary = {};
	Boundary focusBoundary = {};

public:
	void setSize(float inW, float inH) {
		wf = inW;
		hf = inH;
		w = static_cast<int>(inW);
		h = static_cast<int>(inH);
	}

	void calcGrid(int winW, int winH, float ww, float wh) {

		w = winW;
		h = winH;
		wf = ww;
		hf = wh;

		float gw = config::gridWF;
		float gh = config::gridHF;

		float gs = std::floor(ww / gw < wh / gh ? ww / gw : wh / gh);

		gridSize = gs;
		spdlog::info("gridSize = {}, win pixel = {}x{}", gs, w, h);

		startX = std::floor((ww - (gs * gw)) / 2);
		startY = std::floor((wh - (gs * gh)) / 2);
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

	void setFocus(Player p) {
		setFocus(p.x, p.y);
	};

	void setBoundary(int x, int y) {
		boundary.right = static_cast<float>(x);
		boundary.up = static_cast<float>(y);
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
		camera.cx = std::round(wf / 2.0f);
		camera.cy = std::round(hf / 2.0f);
		calcBoundary();
	}
	void calcBoundary() {
		auto hw = wf / 2.0f / camera.gridSize - 0.5f;
		focusBoundary.left = hw;
		focusBoundary.right = boundary.right - hw;

		auto hh = hf / 2.0f / camera.gridSize - 0.5f;
		focusBoundary.down = hh;
		focusBoundary.up = boundary.up - hh;
	}
};
}; // namespace context
