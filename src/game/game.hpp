#pragma once

#include "../asset/asset.hpp"
#include "../context/game.hpp"
#include "../context/window.hpp"
#include "../input.hpp"
#include "../util/event.hpp"
#include "../util/input.hpp"
#include "world.hpp"
#include <algorithm>
#include <memory>
#include <vector>
// #include "scene.hpp"
#include <entt/entt.hpp>

static void parseInputButton(InputButton &in, bool &out) {
	if (in.has) {
		out = in.press;
	}
}

namespace game {

class Game {

private:
	const asset::Asset &asset;
	context::Window &window;

	context::Game ctx;

	std::vector<std::unique_ptr<World>> world;

	Input input = {};

	context::ControlAxis prevAxisA;
	context::ControlAxis prevAxisB;

	int cdZoom = 0;

public:
	Game(const asset::Asset &asset_, context::Window &window_)
		: asset(asset_), window(window_) {

		ctx.enterMap.name = "ruin-2";
		ctx.enterMap.x = config::posResetX;
		ctx.enterMap.y = config::posResetY;
	};
	~Game() {};

	bool parse() {

		if (input.quit) {
			return false;
		}
		parseInput();

		parseControl();

		checkEnterMap();
		// scene.parse();

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

	const World &currentWorld() const {
		return *world[0];
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

	void checkEnterMap() {
		std::string &name = ctx.enterMap.name;
		if (ctx.enterMap.name == "") {
			return;
		}

		auto it = std::ranges::find_if(world,
			[&](const std::unique_ptr<World> &w) { return w->name == name; });

		if (it != world.end()) {
			if (it != world.begin()) {
				std::rotate(world.begin(), it, it + 1);
			}
		} else {
			spdlog::info("new map {}", name);
			world.insert(
				world.begin(), std::make_unique<World>(World{name, asset}));
		}

		world[0]->enter(ctx.enterMap);

		if (world.size() > 5) {
			world.erase(world.begin() + 5, world.end());
		}

		ctx.enterMap.name = "";
	};
};
}; // namespace game
