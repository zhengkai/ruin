#pragma once

#include "../common/control.hpp"
#include "game-dep.hpp"
#include "player.hpp"

class Scene {

private:
	GameDep d;
	Player player;
	bool lastRight = true;

public:
	Scene(GameDep &dep) : d(dep) {
		player = Player();
	};
	~Scene() {};

	bool parse(const Control &control) {

		player.next();
		player.parseFacing(control);

		d.scene->player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
