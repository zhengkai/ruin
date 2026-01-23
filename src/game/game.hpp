#pragma once

#include "../asset/asset.hpp"
#include "../context/window.hpp"
#include "../input.hpp"
#include "../util/event.hpp"
#include "../util/input.hpp"
#include "event.hpp"
#include "reg.hpp"
#include "zone.hpp"
#include <algorithm>
#include <entt/entt.hpp>
#include <memory>
#include <vector>

static void parseInputButton(InputButton &in, bool &out) {
	if (in.has) {
		out = in.press;
	}
}

namespace game {

class Game {

private:
	context::Window &window;
	const asset::Asset &asset;

	std::vector<std::unique_ptr<Zone>> zone;

	Event event = {};
	Input input = {};

	context::ControlAxis prevAxisA;
	context::ControlAxis prevAxisB;

	int cdZoom = 0;
	int cdFullscreen = 0;

public:
	Game(context::Window &window_, const asset::Asset &asset_)
		: window(window_), asset(asset_) {

		window.enterMap = asset.config.zoneStart;

		window.camera.calcGrid();
	};
	~Game() {

	};

	void step() {

		if (window.quit) {
			return;
		}

		if (!window.focus) {
			return;
		}

		checkEnterMap();

		parseEvent();
		parseInput();

		parseControl();

		auto &z = zone[0];
		z->step(window.control);
		prepareRender(z->getReg());

		return;
	}

	void loopEvent() {
		SDL_Event e;
		input.Reset();
		while (SDL_PollEvent(&e)) {
			util::SDLEventLog(e.type);
			if (e.type == SDL_EVENT_QUIT) {
				window.quit = true;
				break;
			}
			handleInput(e);
		}
	};

	const Reg &getReg() const {
		return zone[0]->getReg();
	};

private:
	void eventWindowResize(const std::optional<SDL_WindowEvent> &we) {
		float winW = static_cast<float>(we->data1);
		float winH = static_cast<float>(we->data2);
		spdlog::info("set resize window: {}x{}", winW, winH);
		window.camera.setWinSize(winW, winH);
	};

	void eventWindowPixel(const std::optional<SDL_WindowEvent> &we) {
		float winW = static_cast<float>(we->data1);
		float winH = static_cast<float>(we->data2);
		spdlog::info("set pixel window: {}x{}", winW, winH);
		window.camera.setWinPixelSize(winW, winH);
	};

	void eventWindowScale(const std::optional<SDL_WindowEvent> &we) {
		auto win = SDL_GetWindowFromID(we->windowID);
		float scale = SDL_GetWindowDisplayScale(win);
		window.camera.setScale(scale);
	};

	void prepareRender(const Reg &reg) const {

		// 同步角色 camera 位置
		auto view = reg.view<physics::Rect, tag::Player>();
		if (auto entity = view.front(); entity != entt::null) {
			auto &rect = view.get<physics::Rect>(entity);
			window.camera.setFocus(rect.x, rect.y);
		}
	};

	void parseEvent() {

		window.event = {};

		window.event.fullscreeen = event.toggleFullscreen &&
			window.global.counter(cdFullscreen, config::cdFullscreen);

		if (event.resize) {
			eventWindowResize(event.resize);
		}
		if (event.pixel) {
			eventWindowPixel(event.pixel);
		}
		if (event.scale) {
			eventWindowScale(event.scale);
		}

		event = {};
	};

	void parseInput() {

		if (input.quit) {
			window.quit = true;
			return;
		}

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
		window.camera.setFocusOffset(c.axisB.x, c.axisB.y);

		if (c.btnLB) {
			window.camera.zoomOut();
		} else if (c.btnRB) {
			window.camera.zoomIn();
		}
	};

	void parseZoom() {
		const auto &c = window.control;
		if (c.btnU) {
			if (window.global.counter(cdZoom, config::cdZoom)) {
				window.camera.zoomIn();
			}
		} else if (c.btnD && window.global.counter(cdZoom, config::cdZoom)) {
			window.camera.zoomOut();
		}
	};

	void checkEnterMap() {
		std::string &name = window.enterMap.name;
		if (name == "") {
			return;
		}

		window.map = &asset.map.at(name);

		auto it = std::ranges::find_if(zone,
			[&](const std::unique_ptr<Zone> &w) { return w->name == name; });

		if (it != zone.end()) {
			if (it != zone.begin()) {
				std::rotate(zone.begin(), it, it + 1);
			}
		} else {
			spdlog::info("new zone {}", name);
			zone.insert(
				zone.begin(), std::make_unique<Zone>(name, asset, window));
		}

		zone[0]->enter(window.enterMap);
		if (zone.size() > 5) {
			zone.erase(zone.begin() + 5, zone.end());
		}
		for (std::size_t i = 1, j = zone.size(); i < j; ++i) {
			Zone &w = *zone[i];
			w.leave();
		}

		window.enterMap.name = "";
	};

	void handleInput(SDL_Event &e) {

		// SDL_Gamepad *gamepad;

		switch (e.type) {
		case SDL_EVENT_KEY_DOWN:
			input.key(e.key, true, event);
			break;
		case SDL_EVENT_KEY_UP:
			input.key(e.key, false, event);
			break;
		case SDL_EVENT_WINDOW_RESIZED:
			event.resize = e.window;
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
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			event.pixel = e.window;
			break;
		case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
			event.scale = e.window;
			break;
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
			window.focus = true;
			break;
		case SDL_EVENT_WINDOW_FOCUS_LOST:
			window.focus = false;
			break;
		case SDL_EVENT_MOUSE_MOTION:
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_WINDOW_MOUSE_ENTER:
		case SDL_EVENT_WINDOW_MOUSE_LEAVE:
		case SDL_EVENT_CLIPBOARD_UPDATE:
		case SDL_EVENT_KEYMAP_CHANGED:
		case SDL_EVENT_WINDOW_MOVED:
		case SDL_EVENT_JOYSTICK_ADDED:
		case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
		case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
		case SDL_EVENT_WINDOW_EXPOSED:
		case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
		case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
		case SDL_EVENT_WINDOW_SHOWN:
		case SDL_EVENT_WINDOW_HIDDEN:
			// do nothing
			// spdlog::info("known event type: {}",
			// util::getSDLEventName(e.type));
			break;
		default:
			spdlog::info(
				"unhandled event type: {}", util::getSDLEventName(e.type));
			break;
		}
	};
};
}; // namespace game
