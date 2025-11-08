#pragma once

#include "config.hpp"
#include "util/event.hpp"
#include <SDL3/SDL_events.h>

struct InputAxis {
	int x = 0;
	int y = 0;
	bool hasX = false;
	bool hasY = false;
};

struct Input {

public:
	bool quit = false;
	bool stop = false;
	bool space = false;
	float x = 0.0f;
	float y = 0.0f;
	int winW = 0;
	int winH = 0;
	int speed = 0;
	bool fullscreen = false;
	InputAxis axisA = {};
	InputAxis axisB = {};

	void Reset() {
		*this = {};
	};

	void winResize(const SDL_WindowEvent &w) {
		winW = w.data1;
		winH = w.data2;
	}

	void key(const SDL_KeyboardEvent &e) {
		switch (e.key) {
		case SDLK_ESCAPE:
			if (!cfgWASM) {
				quit = true;
			}
			break;
		case SDLK_UP:
		case SDLK_RIGHT:
			speed = 1;
			break;
		case SDLK_DOWN:
		case SDLK_LEFT:
			speed = -1;
			break;
		case SDLK_SPACE:
			space = true;
			break;
		case SDLK_F11:
			fullscreen = true;
			break;
		case SDLK_RETURN:
		case SDLK_RETURN2:
		case SDLK_KP_ENTER:
			if (e.mod & SDL_KMOD_ALT) {
				fullscreen = true;
			}
			break;
		}
	};

	void gamepadAxis(const SDL_GamepadAxisEvent &e) {
		switch (e.axis) {
		case 0:
			axisA.x = e.value;
			axisA.hasX = e.value;
			break;
		case 1:
			axisA.y = e.value;
			axisA.hasY = e.value;
			break;
		case 2:
			axisB.x = e.value;
			axisB.hasX = e.value;
			break;
		case 3:
			axisB.y = e.value;
			axisB.hasY = e.value;
			break;
		}
	}

	void gamepadButton(const SDL_GamepadButtonEvent &e, bool down) {
		switch (e.button) {
		case SDL_GAMEPAD_BUTTON_START:
			quit = true;
			break;
		default:
			std::string s = util::getSDLGamepadBtnName(e.button);
			spdlog::info("gamepad button {}", s);
			break;
		}
	};
};
