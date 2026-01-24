#include "ruin.h"
#include "context/window.hpp"
#include "game/game.hpp"
#include "sdl.h"
#include <SDL3/SDL_events.h>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Ruin::Ruin() {};

Ruin::~Ruin() {};

bool Ruin::init() {

	spdlog::info("ruin start");

	s = createSDL(window, asset);
	if (!s) {
		spdlog::error("sdl create failed");
		return false;
	}
	if (!s->init()) {
		spdlog::error("sdl init failed");
		return false;
	}
	spdlog::trace("sdl init done");

	g = std::make_unique<game::Game>(window, asset);

	return true;
}

bool Ruin::loop() {

	if (window.focus) {
		window.global.serial++;
	}

#ifdef __EMSCRIPTEN__
	auto now = steady_clock::now();
	float dt =
		std::min(duration<float>(now - prev).count(), config::fpsDeltaTime);
	prev = now;
#else
	t->tick(window.frameMonitor);
#endif

	g->loopEvent();

	g->step();

	s->render(g->getReg());

	return !window.quit;
};
