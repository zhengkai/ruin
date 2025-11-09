#pragma once

#include "../common/control.hpp"
#include "../context/scene.hpp"
#include "player.hpp"

class Scene {

private:
	context::Scene &cs;

	Player player;
	bool lastRight = true;

public:
	Scene(context::Scene &cs) : cs(cs) {
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
