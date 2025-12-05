#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "../input.hpp"
#include "scene.hpp"

class Game {

private:
	context::Window &window;

	Input input = {};
	Scene scene;

	context::ControlAxis prevAxisA;
	context::ControlAxis prevAxisB;

public:
	Game(context::Scene &cs, context::Window &cw, const asset::Asset &asset);
	~Game();

	bool parse();
	void loopEvent();

private:
	void parseInput();
	void parseInputButton();
	void parseInputAxis(const InputAxis &in,
		context::ControlAxis &prev,
		context::ControlAxis &out);
};
