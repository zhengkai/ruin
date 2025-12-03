#include "game.h"
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

	scene.parse(window.control);

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
	parseInputAxis(input.axisA, prevAxisA, window.control.axisA);
	parseInputAxis(input.axisB, prevAxisB, window.control.axisB);
}

void parseInputButton(InputButton &in, bool &out) {
	if (in.has) {
		out = in.press;
	}
}

void Game::parseInputButton() {

	auto &c = window.control;

	::parseInputButton(input.btnA, c.btnA);
	::parseInputButton(input.btnB, c.btnB);
	::parseInputButton(input.btnX, c.btnX);
	::parseInputButton(input.btnY, c.btnY);
	::parseInputButton(input.btnRB, c.btnRB);
	::parseInputButton(input.btnLB, c.btnLB);
	if (input.btnLT.has) {
		c.btnLT = util::gamepadConvert(input.btnLT.v);
	}
	if (input.btnRT.has) {
		c.btnRT = util::gamepadConvert(input.btnRT.v);
	}
}

void Game::parseInputAxis(const InputAxis &in,
	context::ControlAxis &prev,
	context::ControlAxis &out) {
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
