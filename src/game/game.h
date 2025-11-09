#pragma once

#include "../common/control.hpp"
#include "../context/scene.hpp"
#include "../context/window.h"
#include "../input.hpp"
#include "game-dep.hpp"
#include "scene.hpp"

class Game {

private:
	context::Scene &cs;
	context::Window &cw;

	GameDep d;
	Input input = {};
	Scene scene;

	ControlAxis axisA;
	ControlAxis axisB;
	Control control = {};

public:
	Game(GameDep &dep, context::Scene &cs, context::Window &cw);
	~Game();

	bool parse();
	void loopEvent();

private:
	void parseInput();
	void parseInputButton();
	void parseInputAxis(
		const InputAxis &in, ControlAxis &prev, ControlAxis &out);
};
