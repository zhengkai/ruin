#pragma once

#include "../common/control.hpp"
#include "../context/scene.hpp"
#include "game-dep.hpp"
#include "player.hpp"

class Scene {

private:
	context::Scene &cs;

	GameDep d;
	Player player;
	bool lastRight = true;

public:
	Scene(GameDep &dep, context::Scene &cs) : d(dep), cs(cs) {
		player = Player();
	};
	~Scene() {};

	bool parse(const Control &control) {

		player.next();

		player.parseFacing(control);

		cs.player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
