#pragma once

#include "../asset/asset.hpp"
#include "../common/control.hpp"
#include "../context/scene.hpp"
#include "player.hpp"

class Scene {

private:
	context::Scene &c;
	const asset::Asset &asset;

	Player player;
	bool lastRight = true;

public:
	Scene(context::Scene &c, const asset::Asset &asset)
		: c(c), asset(asset), player(c, asset) {};
	~Scene() {};

	bool parse(const Control &control) {

		player.parse(control);

		c.player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
