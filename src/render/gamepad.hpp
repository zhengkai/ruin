#pragma once

#include "base.hpp"

namespace render {

struct Gamepad : base {

	using base::base;

	void init() override {};
	void render() override {

		auto &c = d->window.control;

		trigger(c.btnLT, 150.0f);
		trigger(c.btnRT, 100.0f);
	};

	void trigger(float t, float x) {

		auto &w = d->window;

		SDL_SetRenderDrawColor(d->r, 200, 230, 255, 192);

		SDL_FRect r = {
			.x = w.wf - x, .y = w.hf - 100.0f, .w = 30.0f, .h = 80.0f};
		renderUIFillRect(r);

		SDL_SetRenderDrawColor(d->r, 100, 130, 255, 192);
		r.h = 80.0f * t;
		renderUIFillRect(r);
	}
};

}; // namespace render
