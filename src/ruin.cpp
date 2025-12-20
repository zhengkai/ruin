#include "ruin.h"
#include "config.hpp"
#include "context/misc.h"
#include "context/window.hpp"
#include "game/game.h"
#include "sdl.h"
#include "util/print.hpp"
#include "util/rand.hpp"
#include "world.hpp"
#include <SDL3/SDL_events.h>
#include <algorithm>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Ruin::Ruin() {
}

Ruin::~Ruin() {
	stop = true;
}

bool Ruin::init() {

	scene.player.x = 10.0f;
	scene.player.y = 13.0f;

	spdlog::info("ruin start");

	s = createSDL(scene, window, misc, asset);
	if (!s) {
		spdlog::error("sdl create failed");
	}
	if (!s->init()) {
		spdlog::error("sdl init failed");
		return false;
	}
	spdlog::trace("sdl init done");

	w = createWorld(scene, asset);

	g = std::make_unique<Game>(scene, window, asset);

	return true;
}

void Ruin::loop() {

	window.global.serial++;

#ifdef __EMSCRIPTEN__
	auto now = steady_clock::now();
	float dt =
		std::min(duration<float>(now - prev).count(), config::fpsDeltaTime);
	prev = now;
#else
	t->tick();
#endif

	g->loopEvent();

	if (!g->parse()) {
		stop = true;
		return;
	}

	w->step();

	s->render();
}
