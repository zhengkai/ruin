#pragma once

#include "../asset/asset.hpp"
#include "../common/control.hpp"
#include "../context/misc.h"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "../input.hpp"
#include "scene.hpp"

class Game {

private:
	context::Window &window;
	context::Misc &misc;

	Input input = {};
	Scene scene;

	ControlAxis axisA;
	ControlAxis axisB;
	Control control = {};

public:
	Game(context::Scene &cs,
		context::Window &cw,
		context::Misc &cm,
		const asset::Asset &asset);
	~Game();

	bool parse();
	void loopEvent();

private:
	void parseInput();
	void parseInputButton();
	void parseInputAxis(
		const InputAxis &in, ControlAxis &prev, ControlAxis &out);
};
