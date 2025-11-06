#pragma once

#include "context/misc.h"
#include "context/window.h"
#include "input.hpp"

struct GameDep {
	std::shared_ptr<context::Misc> misc;
	std::shared_ptr<context::Window> window;
};

class Game {

private:
	GameDep d;
	std::shared_ptr<Input> input;

public:
	Game(GameDep dep);
	~Game();

	bool parse();
	void loopEvent();
};
