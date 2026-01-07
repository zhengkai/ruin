#pragma once

#include "pos.hpp"
#include <SDL3/SDL_rect.h>
#include <spdlog/spdlog.h>

namespace physics {

struct Rect : Pos {
	float w;
	float h;

	Rect(Pos pos, float w, float h) : Pos{pos}, w(w), h(h) {};

	Rect(Pos pos, float size) : Pos{pos}, w(size), h(size) {};

	Rect(Pos pos) : Pos{pos}, w(0.5f), h(0.5f) {};

	Rect(float x, float y, float w, float h) : Pos{x, y}, w(w), h(h) {};

	Rect(float x, float y, float size) : Pos{x, y}, w(size), h(size) {
		spdlog::info("Rect({}, {}, {}, {})", x, y, w, h);
	};

	Rect() : Pos{0.0f, 0.0f}, w(0.5f), h(0.5f) {};

	SDL_FRect getRect() const {
		return SDL_FRect{
			.x = Pos::x, .y = Pos::y, .w = w * 2.0f, .h = h * 2.0f};
	};

	bool isOverlap(const Rect &other) const {

		if (x > other.x) {
			if (x - w > other.x + other.w) {
				return false;
			}
		} else if (x < other.x) {
			if (other.x - other.w > x + w) {
				return false;
			}
		}

		if (y > other.y) {
			if (y - h > other.y + other.h) {
				return false;
			}
		} else if (y < other.y) {
			if (other.y - other.h > y + h) {
				return false;
			}
		}

		return true;
	}
};

}; // namespace physics
