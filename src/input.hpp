#pragma once

#include "config.hpp"
#include "util/event.hpp"
#include <SDL3/SDL_events.h>

struct InputButton {
	bool press = false;
	bool has = false;
};

struct InputAxis {
	int x = 0;
	int y = 0;
	bool hasX = false;
	bool hasY = false;
};

struct InputTrigger {
	int v = 0;
	bool has = false;
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

	InputButton btnA = {};
	InputButton btnB = {};
	InputButton btnX = {};
	InputButton btnY = {};

	InputButton btnU = {};
	InputButton btnD = {};
	InputButton btnL = {};
	InputButton btnR = {};

	InputButton btnRB = {};
	InputButton btnLB = {};
	InputTrigger btnLT = {};
	InputTrigger btnRT = {};
	InputAxis axisL = {};
	InputAxis axisR = {};

	void Reset() {
		*this = {};
	};

	void winResize(const SDL_WindowEvent &w) {
		winW = w.data1;
		winH = w.data2;
	}

	void key(const SDL_KeyboardEvent &e, bool down) {
		switch (e.key) {
		case SDLK_ESCAPE:
			if (down && !cfgWASM) {
				quit = true;
			}
			break;
		case SDLK_RIGHT:
			axisL.x = down ? 32767 : 0;
			axisL.hasX = true;
			break;
		case SDLK_LEFT:
			axisL.x = down ? -32768 : 0;
			axisL.hasX = true;
			break;
		case SDLK_UP:
			btnU = {down, true};
			break;
		case SDLK_DOWN:
			btnD = {down, true};
			break;
		case SDLK_SPACE:
			btnA = {down, true};
			break;
		case SDLK_F11:
			if (down) {
				fullscreen = true;
			}
			break;
		case SDLK_RETURN:
		case SDLK_RETURN2:
		case SDLK_KP_ENTER:
			if (down && (e.mod & SDL_KMOD_ALT)) {
				fullscreen = true;
			}
			break;
		}
	};

	void gamepadAxis(const SDL_GamepadAxisEvent &e) {
		switch (e.axis) {
		case 0:
			axisL.x = e.value;
			axisL.hasX = true;
			break;
		case 1:
			axisL.y = e.value;
			axisL.hasY = true;
			break;
		case 2:
			axisR.x = e.value;
			axisR.hasX = true;
			break;
		case 3:
			axisR.y = e.value;
			axisR.hasY = true;
			break;
		case 4:
			btnLT.v = e.value;
			btnLT.has = true;
			break;
		case 5:
			btnRT.v = e.value;
			btnRT.has = true;
			break;
		default:
			spdlog::info("gamepad axis {} {}", e.axis, e.value);
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

			switch (e.button) {
			case SDL_GAMEPAD_BUTTON_NORTH:
				btnY = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_SOUTH:
				btnA = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_WEST:
				btnX = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_EAST:
				btnB = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_DPAD_UP:
				btnU = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
				btnD = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
				btnL = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
				btnR = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
				btnLB = {down, true};
				break;
			case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
				btnRB = {down, true};
				break;
			default:
				spdlog::info("gamepad button {}", s);
				break;
			}

			break;
		}
	};
};
