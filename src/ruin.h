#pragma once

#include "context/misc.h"
#include "context/window.h"
#include "game/game.h"
#include "region.hpp"
#include "sdl.h"
#include "time.hpp"

struct RuinDeps {
	std::shared_ptr<context::BallCluster> ballCluster;
	std::shared_ptr<context::Misc> misc;
	std::shared_ptr<context::Window> window;
};

class Ruin {

private:
	context::Scene cs = {};
	context::Window cw = {};

	RuinDeps d;
	std::unique_ptr<Time> t;
	std::unique_ptr<sdl> s;
	std::unique_ptr<Game> g;
	steady_clock::time_point prev;
	std::vector<std::unique_ptr<Region>> region;

private:
	std::vector<context::Brick> genBrick();

public:
	bool stop;
	Ruin();
	~Ruin();
	void loop();
	void run();
	bool init();
	bool initBrick();
};
