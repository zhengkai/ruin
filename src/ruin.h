#pragma once

#include "context/window.hpp"
#include "game/game.hpp"
#include "sdl.h"
#include "time.hpp"
#include <entt/entt.hpp>

class Ruin {

private:
	context::Window window = {};
	asset::Asset asset = {};

	std::unique_ptr<Time> t = std::make_unique<Time>();
	std::unique_ptr<sdl> s;
	std::unique_ptr<game::Game> g;
	steady_clock::time_point prev;

public:
	bool stop = false;
	Ruin();
	~Ruin();
	void loop();
	void run();
	bool init();
	void syncPos();
};
