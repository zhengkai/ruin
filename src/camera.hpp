#pragma once

#include "asset/common.hpp"
#include "asset/map.hpp"
#include "config.hpp"
#include "physics/pos.hpp"
#include "physics/rect.hpp"
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>
#include <tuple>

struct Boundary {
	float d = 0.0f;
	float u = 100.0f;
	float l = 0.0f;
	float r = 100.0f;
};

struct Focus {
	float x = 0.0f;
	float y = 0.0f;
	float offsetX = 0.0f; // 实际转视角的偏差修正
	float offsetY = 0.0f;

	float finalX = 0.0f; // 计算用的最终偏差，给 calcCameraOffset 等函数
	float finalY = 0.0f;
};

static inline void calcFocusAxis(float &focus,
	float &final,
	float input,
	float rect,
	float map,
	float min,
	float max,
	float offset) {

	if (rect > map) {
		focus = map;
	} else {
		if (input < min) {
			focus = min;
		} else if (input > max) {
			focus = max;
		} else {
			focus = input;
		}
	}
	final = focus + offset * config::focusRange;
};

class Camera {

private:
	Focus focus = {};
	float prevFocusX = 0.0f;
	float prevFocusY = 0.0f;

	Boundary boundary = {};
	Boundary focusBoundary = {};

	int zoom = 100;
	float zoomRate = 1.0f;

	float winW = 800.0f;
	float winH = 600.0f;
	float scale = 1.0f;
	float winPixelW = 800.0f;
	float winPixelH = 600.0f;

	float baseGridSize = 1.0f;
	float gridSize = 1.0f;

	// 在 grid 系统里的上下宽高
	float rectPixelW = 0.0f;
	float rectPixelH = 0.0f;
	float rectW = 0.0f;
	float rectH = 0.0f;

	// map size
	float mapW = 1.0f;
	float mapH = 1.0f;

public:
	Camera() {
		calcZoom();
	};

	void calcOffset(SDL_FRect &r) const {

		r.x = rectPixelW - gridSize * (focus.finalX - r.x + r.w / 2.0f);
		r.y = rectPixelH + gridSize * (focus.finalY - r.y - r.h / 2.0f);

		r.w *= gridSize;
		r.h *= gridSize;
	}

	float getGridSize() const {
		return gridSize;
	};

	void setWinSize(const float &w, const float &h) {
		if (winW == w && winH == h) {
			return;
		}
		winW = w;
		winH = h;
		calcGrid();
	};

	void setWinPixelSize(const float &w, const float &h) {
		if (winPixelW == w && winPixelH == h) {
			return;
		}
		winPixelW = w;
		winPixelH = h;
		calcGrid();
	};

	void setScale(const float &s) {
		scale = s;
		calcGrid();
	};

	float getScale() const {
		return scale;
	};

	asset::Size getWinSize() const {
		return {winW, winH};
	};

	asset::Size getWinPixelSize() const {
		return {winPixelW, winPixelH};
	};

	void zoomIn() {
		if (zoom < 200) {
			zoom++;
			calcZoom();
		}
	};

	void zoomOut() {
		if (zoom > 0) {
			zoom--;
			calcZoom();
		}
	};

	std::tuple<int, float, float> getZoom() const {
		return {zoom, zoomRate, gridSize};
	};

	void calcGrid() {
		baseGridSize =
			std::floor(winPixelW / config::gridWF < winPixelH / config::gridHF
					? winPixelW / config::gridWF
					: winPixelH / config::gridHF);
		calc();
	}

	float fontSize(float &size) const {
		return gridSize * size;
	};

	physics::Rect focusRect() const {
		return {focus.x, focus.y, rectW, rectH};
	};

	void setFocus(float x, float y) {

		if (x == prevFocusX && y == prevFocusY) {
			return;
		}
		prevFocusX = x;
		prevFocusY = y;

		// 地图边缘移动角色，而不是始终对准人物（从而画面有大量地图外空白区域）

		calcFocusAxis(focus.x,
			focus.finalX,
			x,
			rectW,
			mapW,
			focusBoundary.l,
			focusBoundary.r,
			focus.offsetX);

		calcFocusAxis(focus.y,
			focus.finalY,
			y,
			rectH,
			mapH,
			focusBoundary.d,
			focusBoundary.u,
			focus.offsetY);
	};

	void setFocus(physics::Pos &pos) {
		setFocus(pos.x, pos.y);
	};

	void setFocusOffset(const float &axisX, const float &axisY) {
		if (focus.offsetX != axisX || focus.offsetY != axisY) {
			float x = focus.offsetX - axisX;
			float y = focus.offsetY - axisY;
			float dist = std::sqrt(x * x + y * y);
			if (dist > config::focusSpeed) {
				float ratio = config::focusSpeed / dist;
				focus.offsetX -= x * ratio;
				focus.offsetY -= y * ratio;
			} else {
				focus.offsetX = axisX;
				focus.offsetY = axisY;
			}
		};
	};

	void setMapSize(const asset::Map &m) {
		float w = static_cast<float>(m.w);
		float h = static_cast<float>(m.h);
		boundary.r = w - 1.0f;
		boundary.u = h - 1.0f;
		mapW = boundary.r / 2.0f;
		mapH = boundary.u / 2.0f;

		prevFocusX = -1000.0f;
		prevFocusY = -1000.0f;

		calcBoundary();
	};

private:
	void calcZoom() {

		zoomRate = static_cast<float>(zoom) / 200.0f * 4.0f + 1.0f;

		calc();
	}

	void calc() {

		gridSize = baseGridSize * zoomRate;
		rectPixelW = std::round(winPixelW / 2.0f);
		rectPixelH = std::round(winPixelH / 2.0f);
		calcBoundary();
	};

	void calcBoundary() {
		rectW = winPixelW / 2.0f / gridSize - 0.5f;
		focusBoundary.l = rectW;
		focusBoundary.r = boundary.r - rectW;

		rectH = winPixelH / 2.0f / gridSize - 0.5f;
		focusBoundary.d = rectH;
		focusBoundary.u = boundary.u - rectH;
	};
};
