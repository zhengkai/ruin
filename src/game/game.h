#pragma once

#include "../input.hpp"
#include "game-dep.hpp"
#include "scene.hpp"

class Game {

private:
	GameDep d;
	std::shared_ptr<Input> input;
	Scene scene;

public:
	Game(GameDep &dep);
	~Game();

	bool parse();
	void loopEvent();

private:
	void parseInput();
};
