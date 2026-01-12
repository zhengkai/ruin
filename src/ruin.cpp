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

Ruin::~Ruin() {
	stop = true;
};

bool Ruin::init() {

	scene.player.x = 10.0f;
	scene.player.y = 13.0f;

	spdlog::info("ruin start");

	s = createSDL(scene, window, asset);
	if (!s) {
		spdlog::error("sdl create failed");
		return false;
	}
	if (!s->init()) {
		spdlog::error("sdl init failed");
		return false;
	}
	spdlog::trace("sdl init done");

	// w = createWorld(scene, asset);

	g = std::make_unique<game::Game>(asset, window);

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

	parseCommand();

	s->render(g->currentWorld());
};

void Ruin::parseCommand() {

	//	auto &sp = scene.player;
	//
	//	if (!sp.physicsSerial) {
	//		return;
	//	}
	//
	//	auto &b = p->getBody(sp.physicsSerial);
	//	auto &cmd = sp.command;
	//	if (cmd.jump) {
	//		b.vy = config::jumpForce;
	//	}
	//	b.vx = cmd.x;
};
