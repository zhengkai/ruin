#pragma once

#include "../common/control.hpp"
#include "../input.hpp"
#include "game-dep.hpp"
#include "scene.hpp"

class Game {

private:
	GameDep d;
	Input input = {};
	Scene scene;

	ControlAxis axisA;
	ControlAxis axisB;
	Control control = {};

public:
	Game(GameDep &dep);
	~Game();

	bool parse();
	void loopEvent();

private:
	void parseInput();
	void parseInputButton();
	void parseInputAxis(
		const InputAxis &in, ControlAxis &prev, ControlAxis &out);
};
