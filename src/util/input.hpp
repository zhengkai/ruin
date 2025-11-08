#pragma once

#include "../input.hpp"

namespace util {

inline void handleInput(SDL_Event &e, Input &input) {

	SDL_Gamepad *gamepad;

	switch (e.type) {
	case SDL_EVENT_KEY_DOWN:
		input.key(e.key);
		break;
	case SDL_EVENT_WINDOW_RESIZED:
		input.winResize(e.window);
		break;
	case SDL_EVENT_GAMEPAD_AXIS_MOTION:
		input.gamepadAxis(e.gaxis);
		break;
	case SDL_EVENT_GAMEPAD_ADDED:
		SDL_OpenGamepad(e.gdevice.which);
		break;
	case SDL_EVENT_GAMEPAD_REMOVED:
		SDL_CloseGamepad(SDL_GetGamepadFromID(e.gdevice.which));
		break;
	case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		input.gamepadButton(e.gbutton, true);
		break;
	case SDL_EVENT_GAMEPAD_BUTTON_UP:
		input.gamepadButton(e.gbutton, false);
		break;
	default:
		// spdlog::info(
		// "unhandled event type: {}", util::getSDLEventName(e->type));
		break;
	}
};

inline float gamepadConvert(const int v) {
	float r = static_cast<float>(v) / 32768.0f;
	if (r > -0.1f && r < 0.1f) {
		r = 0.0f;
	} else if (r > 0.9f) {
		r = 1.0f;
	} else if (r < -0.9f) {
		r = -1.0f;
	}
	return r;
};
}; // namespace util
