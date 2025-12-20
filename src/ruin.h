#pragma once

#include "context/window.hpp"
#include "game/game.h"
#include "sdl.h"
#include "time.hpp"
#include "world.hpp"

class Ruin {

private:
	context::Scene scene = {};
	context::Window window = {};
	asset::Asset asset = {};

	std::unique_ptr<Time> t = std::make_unique<Time>();
	std::unique_ptr<sdl> s;
	std::unique_ptr<Game> g;
	std::unique_ptr<World> w;
	steady_clock::time_point prev;

public:
	bool stop = false;
	Ruin();
	~Ruin();
	void loop();
	void run();
	bool init();
};
