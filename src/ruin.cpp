#include "ruin.h"
#include "config.hpp"
#include "context/ball.h"
#include "context/brick.hpp"
#include "context/entity.h"
#include "context/window.h"
#include "game.h"
#include "region.hpp"
#include "sdl-dep.hpp"
#include "sdl.h"
#include "util/ball.hpp"
#include "util/print.hpp"
#include "util/rand.hpp"
#include <SDL3/SDL_events.h>
#include <algorithm>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

Ruin::Ruin() : stop(false), t(std::make_unique<Time>()) {
}

Ruin::~Ruin() {
	stop = true;
}

bool Ruin::init() {

	auto e = std::make_shared<context::Entity>();
	auto w = std::make_shared<context::Window>();
	auto bc = std::make_shared<context::BallCluster>();

	bc->group =
		util::genBallGroupList(config::gridWF, config::gridHF, config::region);

	e->brick = genBrick();

	d = {
		.ballCluster = bc,
		.entity = e,
		.window = w,
	};

	for (auto &b : bc->group) {
		region.push_back(std::make_unique<Region>(e, b));
	}

	g = std::make_unique<Game>(GameDep{
		.entity = e,
		.window = w,
	});

	auto sd = sdlDep{
		.ballCluster = bc,
		.entity = e,
		.window = w,
	};
	s = std::make_unique<sdl>(sd);

	spdlog::info("ruin start");

	if (!s->init()) {
		spdlog::error("sdl init failed");
		return false;
	}
	spdlog::trace("sdl init done");

	spdlog::info("region num = {}, list = {}",
		config::regionNum,
		util::joinVector(config::region));

	return true;
}

std::vector<context::Brick> Ruin::genBrick() {

	std::vector<context::Brick> li;
	for (int y = 0; y < config::gridH; y++) {
		for (int x = 0; x < config::gridW; x++) {
			auto b = context::genBrick(x, y);
			li.push_back(b);
		}
	}
	return li;
}

void Ruin::loop() {

	d.window->serial++;

#ifdef __EMSCRIPTEN__
	auto now = steady_clock::now();
	float dt =
		std::min(duration<float>(now - prev).count(), config::fpsDeltaTime);
	prev = now;
#else
	float dt = config::fpsDeltaTime;
	t->tick();
#endif

	g->loopEvent();

	if (!g->parse()) {
		stop = true;
		return;
	}

	spdlog::trace("dt = {:.6f}s", dt);
	for (int i = 0; i < cfgPhyLoop; i++) {
		std::ranges::shuffle(region.begin(), region.end(), util::rng());
		for (auto &r : region) {
			r->update(dt);
		}
	}

	s->render();
}
