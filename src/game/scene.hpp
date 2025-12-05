#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "player.hpp"

class Scene {

private:
	const context::Control &control;
	context::Scene &c;
	const asset::Asset &asset;

	Player player;
	bool lastRight = true;

public:
	Scene(const context::Control &control,
		context::Scene &c,
		const asset::Asset &asset)
		: control(control), c(c), asset(asset), player(control, c, asset) {};
	~Scene() {};

	bool parse() {

		player.parse();

		c.player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
