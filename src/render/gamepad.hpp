#pragma once

#include "../game/reg.hpp"
#include "base.hpp"

namespace render {

struct Gamepad : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &_) override {

		auto &c = d.window.control;

		float x;

		x = 9.25f;
		button(c.btnY, x + 1.25f, 0.0f);
		button(c.btnX, x, 1.25f);
		button(c.btnB, x + 2.5f, 1.25f);
		button(c.btnA, x + 1.25f, 2.5f);

		x = 5.0f;
		button(c.btnU, x + 1.25f, 0.0f);
		button(c.btnL, x, 1.25f);
		button(c.btnR, x + 2.5f, 1.25f);
		button(c.btnD, x + 1.25f, 2.5f);

		button(c.btnLB, 0.0f, 0.0f);
		button(c.btnRB, 16.75f, 0.0f);

		axis(c.axisA, 1.25f);
		axis(c.axisB, 13.0f);

		trigger(c.btnLT, 0.0f);
		trigger(c.btnRT, 16.75f);
	};

	void button(bool v, float x, float y) {

		if (v) {
			colorValid();
		} else {
			colorInvalid();
		}

		SDL_FRect r = {.x = x, .y = y, .w = 1.0f, .h = 1.0f};

		renderGamepad(r);
	}

	void trigger(float v, float x) {

		colorInvalid();
		SDL_FRect r = {.x = x, .y = 1.25f, .w = 1.0f, .h = 2.25f};
		auto ra = r;
		renderGamepad(r);

		if (!v) {
			return;
		}

		colorValid();
		ra.h *= v;
		renderGamepad(ra);
	}

	void axis(const context::ControlAxis &v, float x) {

		colorInvalid();
		SDL_FRect r = {.x = x, .y = 0.0f, .w = 3.5f, .h = 3.5f};
		renderGamepad(r);

		// spdlog::info("Axis x:{:.3f} y:{:.3f}", v.x, v.y);

		if (v.x || v.y) {
			colorValid();
			r = {.x = x + (1 + v.x) * 1.25f,
				.y = (1 - v.y) * 1.25f,
				.w = 1.0f,
				.h = 1.0f};
			renderGamepad(r);
		}
	}

	void colorInvalid() {
		SDL_SetRenderDrawColor(d.r, 50, 70, 90, 192);
	}
	void colorValid() {
		SDL_SetRenderDrawColor(d.r, 200, 230, 255, 255);
	}

	void renderGamepad(SDL_FRect &r) {

		float grid = 50.0f;

		auto w = camera.getWinSize();
		float x = w.w - grid * 18.25f;
		float y = w.h - grid * 4.0f;

		r.x = x + r.x * grid;
		r.y = y + r.y * grid;
		r.w = r.w * grid;
		r.h = r.h * grid;

		renderUIFilledRect(r);
	}
};

}; // namespace render
