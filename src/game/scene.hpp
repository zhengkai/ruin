#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
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

	bool parse(const context::Control &control) {

		player.parse(control);

		c.player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
