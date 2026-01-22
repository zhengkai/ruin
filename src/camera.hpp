#pragma once

#include "asset/common.hpp"
#include "config.hpp"
#include "physics/pos.hpp"
#include "physics/rect.hpp"
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>

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

class Camera {

private:
	Focus focus = {};
	Boundary boundary = {};
	Boundary focusBoundary = {};

	float x = 0.0f;
	float y = 0.0f;
	float zoom = 3.5f;

	float winW = 800.0f;
	float winH = 600.0f;
	float gridSize = 1.0f;

public:
	void calcOffset(SDL_FRect &r) const {
		r.x = x - gridSize * (focus.finalX - r.x + r.w / 2.0f);
		r.y = y + gridSize * (focus.finalY - r.y - r.h / 2.0f);
		r.w *= gridSize;
		r.h *= gridSize;
	}

	float getGridSize() const {
		return gridSize;
	};

	void setWinSize(const float &w, const float &h) {
		winW = w;
		winH = h;
	};

	asset::Size getWinSize() const {
		return {winW, winH};
	};

	void zoomIn() {
		if (zoom < 5.0f) {
			zoom += 0.125f;
			calc();
		}
	};

	void zoomOut() {
		if (zoom > 1.0f) {
			zoom -= 0.125f;
			calc();
		}
	};

	void calcGrid(float w, float h) {

		winW = w;
		winH = h;

		float &gw = config::gridWF;
		float &gh = config::gridHF;

		float gs = std::floor(w / gw < h / gh ? w / gw : h / gh);

		gridSize = gs;
		spdlog::info("gridSize = {}, win pixel = {}x{}", gs, w, h);

		calc();
	}

	void calc() {
		gridSize = gridSize * zoom;
		spdlog::info("camera zoom: {} {}", zoom, gridSize);
		x = std::round(winW / 2.0f);
		y = std::round(winH / 2.0f);
		calcBoundary();
	};

	const float fontSize(float &size) const {
		return gridSize * size;
	};

	const physics::Rect focusRect() {
		return {focus.x, focus.y, 23.0f, 23.0f};
	};

	void prepareFocus() {
		focus.finalX = focus.x + focus.offsetX * config::focusRange;
		focus.finalY = focus.y + focus.offsetY * config::focusRange;
	};

	void setBoundary(int x, int y) {
		boundary.r = static_cast<float>(x - 1);
		boundary.u = static_cast<float>(y - 1);
		calcBoundary();
	};

	void setFocus(float x, float y) {
		if (x < focusBoundary.l) {
			x = focusBoundary.l;
		} else if (x > focusBoundary.r) {
			x = focusBoundary.r;
		}
		focus.x = x;

		if (y < focusBoundary.d) {
			y = focusBoundary.d;
		} else if (y > focusBoundary.u) {
			y = focusBoundary.u;
		}
		focus.y = y;
	};

	void setFocus(physics::Pos &pos) {
		setFocus(pos.x, pos.y);
	};

	void parseFocus(const float &axisX, const float &axisY) {
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
	}

private:
	void calcBoundary() {
		auto hw = winW / 2.0f / gridSize - 0.5f;
		focusBoundary.l = hw;
		focusBoundary.r = boundary.r - hw;

		auto hh = winH / 2.0f / gridSize - 0.5f;
		focusBoundary.d = hh;
		focusBoundary.u = boundary.u - hh;
	};
};
