#pragma once

#include "context/misc.h"
#include "context/window.h"
#include "game/game.h"
#include "region.hpp"
#include "sdl.h"
#include "time.hpp"

class Ruin {

private:
	context::Scene scene = {};
	context::Window window = {};
	context::Misc misc = {};
	context::BallCluster cb = {};

	std::unique_ptr<Time> t = std::make_unique<Time>();
	std::unique_ptr<sdl> s;
	std::unique_ptr<Game> g;
	steady_clock::time_point prev;
	std::vector<std::unique_ptr<Region>> region;

private:
	std::vector<context::Brick> genBrick();

public:
	bool stop = false;
	Ruin();
	~Ruin();
	void loop();
	void run();
	bool init();
	bool initBrick();
};
