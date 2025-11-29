#include "game.h"
#include "../common/control.hpp"
#include "../config.hpp"
#include "../input.hpp"
#include "../util/event.hpp"
#include "../util/input.hpp"
#include <algorithm>

static std::string speedMsg = "Speed Level: ";

Game::Game(context::Scene &cs,
	context::Window &cw,
	context::Misc &cm,
	const asset::Asset &asset)
	: window(cw), misc(cm), scene(Scene(cs, asset)) {
}

Game::~Game() {
}

bool Game::parse() {

	if (input.quit) {
		return false;
	}
	parseInput();

	scene.parse(control);

	return true;
}

void Game::parseInput() {

	// control speed

	if (input.speed != 0) {
		int slv = misc.speedLevel + input.speed;
		slv = std::max(
			-config::speedLevelMax, std::min(config::speedLevelMax, slv));
		misc.speedLevel = slv;
		misc.speed = std::pow(2, slv);

		std::string m = std::to_string((int)std::pow(2, std::abs(slv))) + "x";
		if (slv < 0) {
			m = "1/" + m;
		}
	}

	// window resize

	if (input.winW > 0 && input.winH > 0) {
		auto &wr = window.winResize;
		wr.w = input.winW;
		wr.h = input.winH;
		wr.trigger = true;
	}

	// show ball

	if (input.space) {
		window.showBall = !window.showBall;
	}

	// fullscreen toggle

	if (input.fullscreen) {
		spdlog::info("toggling fullscreen");
		window.toggleFullscreen = true;
	}

	// gamepad

	parseInputButton();
	parseInputAxis(input.axisA, axisA, control.axisA);
	parseInputAxis(input.axisB, axisB, control.axisB);
}

void Game::parseInputButton() {
	if (input.hasBtnA) {
		control.btnA = input.btnA;
	}
	if (input.hasBtnB) {
		control.btnB = input.btnB;
	}
	if (input.hasBtnX) {
		control.btnX = input.btnX;
	}
	if (input.hasBtnY) {
		control.btnY = input.btnY;
	}
}

void Game::parseInputAxis(
	const InputAxis &in, ControlAxis &prev, ControlAxis &out) {
	if (!in.hasX && !in.hasY) {
		return;
	}
	if (in.hasX) {
		out.x = prev.x = util::gamepadConvert(in.x);
	}
	if (in.hasY) {
		out.y = prev.y = util::gamepadConvert(in.y);
	}
	if (prev.x != 0.0f && prev.y != 0.0f) {
		float speed = std::sqrt(prev.x * prev.x + prev.y * prev.y);
		if (speed > 1.0f) {
			out.x = prev.x / speed;
			out.y = prev.y / speed;
		}
	}
}

void Game::loopEvent() {
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
}
