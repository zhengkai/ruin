#pragma once

#include "game-dep.hpp"
#include "player.hpp"

class Scene {

private:
	GameDep d;
	Player player;

public:
	Scene(GameDep &dep) : d(dep) {
		player = Player();
	};
	~Scene() {};

	bool parse() {

		player.next();

		d.scene->player.pose = player.pose;

		return true;
	};
	void loopEvent();
};
