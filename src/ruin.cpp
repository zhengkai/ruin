#include "ruin.h"
#include "context/window.hpp"
#include "game/game.hpp"
#include "physics/create.hpp"
#include "sdl.h"
#include "world.hpp"
#include <SDL3/SDL_events.h>
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

	s = createSDL(scene, window, asset);
	if (!s) {
		spdlog::error("sdl create failed");
	}
	if (!s->init()) {
		spdlog::error("sdl init failed");
		return false;
	}
	spdlog::trace("sdl init done");

	// w = createWorld(scene, asset);

	g = std::make_unique<Game>(scene, window, asset);

	p = physics::Create(scene, asset).get();

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

	if (w) {
		w->step();
	}
	if (p) {
		p->step();
	}

	syncPos();

	s->render();
};

void Ruin::parseCommand() {

	auto &sp = scene.player;

	auto &b = p->getBody(sp.physicsSerial);
	auto &cmd = sp.command;
	if (cmd.jump) {
		b.vy = config::jumpForce;
	}
	b.vx = cmd.x;
	spdlog::warn("player vx: {}, player x: {}, y: {}, serial: {}",
		b.vx,
		b.x,
		b.y,
		b.serial);
};

void Ruin::syncPos() {
	auto &sp = scene.player;
	auto pos = p->getBody(sp.physicsSerial);
	sp.x = pos.x;
	sp.y = pos.y;
};
