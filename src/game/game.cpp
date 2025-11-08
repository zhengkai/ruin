#include "game.h"
#include "../common/control.hpp"
#include "../config.hpp"
#include "../input.hpp"
#include "../util/event.hpp"
#include "../util/input.hpp"
#include <algorithm>

static std::string speedMsg = "Speed Level: ";

Game::Game(GameDep &dep) : d(dep), scene(Scene(d)) {
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
		int slv = d.misc->speedLevel + input.speed;
		slv = std::max(
			-config::speedLevelMax, std::min(config::speedLevelMax, slv));
		d.misc->speedLevel = slv;
		d.misc->speed = std::pow(2, slv);

		context::ControlMsg *cm = d.window->controlMsg;
		if (cm == nullptr) {
			cm = new context::ControlMsg();
			d.window->controlMsg = cm;
		}

		std::string m = std::to_string((int)std::pow(2, std::abs(slv))) + "x";
		if (slv < 0) {
			m = "1/" + m;
		}
		cm->msg = speedMsg + m;
		cm->expireSerial = d.window->serial + config::fps * 2;
	}

	// window resize

	if (input.winW > 0 && input.winH > 0) {
		auto wr = d.window->winResize;
		if (wr == nullptr) {
			wr = new context::WinResize();
			d.window->winResize = wr;
		}
		wr->w = input.winW;
		wr->h = input.winH;
	}

	// show ball

	if (input.space) {
		d.window->showBall = !d.window->showBall;
	}

	// fullscreen toggle

	if (input.fullscreen) {
		spdlog::info("toggling fullscreen");
		d.window->toggleFullscreen = true;
	}

	// gamepad

	parseInputAxis(input.axisA, axisA, control.axisA);
	parseInputAxis(input.axisB, axisB, control.axisB);
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
