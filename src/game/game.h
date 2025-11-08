#pragma once

#include "../common/control.hpp"
#include "../input.hpp"
#include "game-dep.hpp"
#include "scene.hpp"

class Game {

private:
	GameDep d;
	std::shared_ptr<Input> input;
	Scene scene;

	ControlAxis axisA;
	ControlAxis axisB;
	std::shared_ptr<Control> control;

public:
	Game(GameDep &dep);
	~Game();

	bool parse();
	void loopEvent();

private:
	void parseInput();
	void parseInputAxis(
		const InputAxis &in, ControlAxis &prev, ControlAxis &out);
};
