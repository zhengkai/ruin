#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "../input.hpp"
#include "../util/event.hpp"
#include "../util/input.hpp"
#include "scene.hpp"

static void parseInputButton(InputButton &in, bool &out) {
	if (in.has) {
		out = in.press;
	}
}

class Game {

private:
	context::Window &window;

	Input input = {};
	Scene scene;

	context::ControlAxis prevAxisA;
	context::ControlAxis prevAxisB;

	int cdZoom = 0;

public:
	Game(context::Scene &cs, context::Window &cw, const asset::Asset &asset)
		: window(cw), scene(Scene(cw.global, cw.control, cs, asset)) {};
	~Game() {};

	bool parse() {

		if (input.quit) {
			return false;
		}
		parseInput();

		parseControl();

		scene.parse();

		return true;
	}

	void loopEvent() {
		SDL_Event e;
		input.Reset();
		while (SDL_PollEvent(&e)) {
			util::SDLEventLog(e.type);
			if (e.type == SDL_EVENT_QUIT) {
				input.quit = true;
				break;
			}
			util::handleInput(e, input);
		}
	};

private:
	void parseInput() {

		// window resize

		if (input.winW > 0 && input.winH > 0) {
			auto &wr = window.winResize;
			wr.w = input.winW;
			wr.h = input.winH;
			wr.trigger = true;
		}

		// fullscreen toggle

		if (input.fullscreen) {
			spdlog::info("toggling fullscreen");
			window.toggleFullscreen = true;
		}

		// gamepad

		parseInputButton();
		parseInputAxis(input.axisL, prevAxisA, window.control.axisA);
		parseInputAxis(input.axisR, prevAxisB, window.control.axisB);
	};

	void parseInputButton() {

		auto &c = window.control;

		::parseInputButton(input.btnA, c.btnA);
		::parseInputButton(input.btnB, c.btnB);
		::parseInputButton(input.btnX, c.btnX);
		::parseInputButton(input.btnY, c.btnY);

		::parseInputButton(input.btnU, c.btnU);
		::parseInputButton(input.btnD, c.btnD);
		::parseInputButton(input.btnL, c.btnL);
		::parseInputButton(input.btnR, c.btnR);

		::parseInputButton(input.btnRB, c.btnRB);
		::parseInputButton(input.btnLB, c.btnLB);
		if (input.btnLT.has) {
			c.btnLT = util::gamepadConvert(input.btnLT.v);
		}
		if (input.btnRT.has) {
			c.btnRT = util::gamepadConvert(input.btnRT.v);
		}
	};

	void parseInputAxis(const InputAxis &in,
		context::ControlAxis &prev,
		context::ControlAxis &out) {
		if (!in.hasX && !in.hasY) {
			return;
		}
		if (in.hasX) {
			out.x = prev.x = util::gamepadConvert(in.x);
		}
		if (in.hasY) {
			out.y = prev.y = util::gamepadConvert(-in.y);
		}
		if (prev.x != 0.0f && prev.y != 0.0f) {
			float speed = std::sqrt(prev.x * prev.x + prev.y * prev.y);
			if (speed > 1.0f) {
				out.x = prev.x / speed;
				out.y = prev.y / speed;
			}
		}
	};

	void parseControl() {
		const auto &c = window.control;

		// parseZoom();

		if (window.focus.offsetX != c.axisB.x ||
			window.focus.offsetY != c.axisB.y) {

			float x = window.focus.offsetX - c.axisB.x;
			float y = window.focus.offsetY - c.axisB.y;
			float dist = std::sqrt(x * x + y * y);
			if (dist > config::focusSpeed) {
				float ratio = config::focusSpeed / dist;
				window.focus.offsetX -= x * ratio;
				window.focus.offsetY -= y * ratio;
			} else {
				window.focus.offsetX = c.axisB.x;
				window.focus.offsetY = c.axisB.y;
			}
		};
	};

	void parseZoom() {
		const auto &c = window.control;
		if (c.btnU) {
			if (window.global.counter(cdZoom, config::cdZoom)) {
				window.zoomIn();
			}
		} else if (c.btnD && window.global.counter(cdZoom, config::cdZoom)) {
			window.zoomOut();
		}
	};
};
