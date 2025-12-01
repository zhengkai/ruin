#pragma once

#include "../config.hpp"
#include "entity.hpp"
#include <spdlog/spdlog.h>

namespace context {

struct WinResize {
	int w = 0;
	int h = 0;
	bool trigger = false;
};

struct Focus {
	float x = 10.0f;
	float y = 10.0f;
};

struct Camera {
	float zoom = 1.5f;
	float cx = 0.0f; // cx/cy (中心像素的坐标)
	float cy = 0.0f;
	float gridSize = 1.0f;
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
	int serial = 0;
	WinResize winResize = {};
	bool showBall = true;
	bool toggleFullscreen = false;

private:
	Focus focus = {};
	Camera camera = {};

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
		float gh = config::gridHF + cfgPaddingTop;

		float gs = std::floor(ww / gw < wh / gh ? ww / gw : wh / gh);

		gridSize = gs;
		spdlog::info("gridSize = {}, win = {}x{}", gs, winW, winH);

		startX = std::floor((ww - (gs * gw)) / 2);
		startY = std::floor((wh - (gs * gh)) / 2 + (gs * cfgPaddingTop));
		spdlog::info("grid1 {}x{}={}, grid pixel: {}x{}, win pixel: {}x{}, "
					 "start: x={},y={}",
			config::gridW,
			config::gridH,
			config::gridW * config::gridH,
			gs * config::gridW,
			gs * config::gridH,
			w,
			h,
			startX,
			startY);

		calcCamera();
	}

	void setFocus(float x, float y) {
		focus.x = x;
		focus.y = y;
	};
	void setFocus(Player p) {
		setFocus(p.x, p.y);
	}

	void calcCameraOffset(SDL_FRect &r) {
		r.x = camera.cx - camera.gridSize * (focus.x - r.x);
		r.y = camera.cy + camera.gridSize * (focus.y - r.y);
		r.w *= camera.gridSize;
		r.h *= camera.gridSize;
	};

private:
	void calcCamera() {
		camera.gridSize = gridSize * camera.zoom;
		camera.cx = std::round(wf / 2.0f);
		camera.cy = std::round(hf / 2.0f);
	}
};
}; // namespace context
