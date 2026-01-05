#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "../context/window.hpp"
#include "player.hpp"

class Scene {

private:
	const context::Global &global;
	const context::Control &control;
	context::Scene &c;
	const asset::Asset &asset;

	Player player;
	bool lastRight = true;

public:
	Scene(const context::Global &global,
		const context::Control &control,
		context::Scene &c,
		const asset::Asset &asset)
		: global(global), control(control), c(c), asset(asset),
		  player(global, control, c, asset) {};
	~Scene() {};

	bool parse() {

		if (control.btnRB) {
			c.map = asset.map.at("ruin-2");
			return true;
		}

		if (control.btnLB) {
			c.map = asset.map.at("ruin-3");
			return true;
		}

		player.parse();

		c.player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
