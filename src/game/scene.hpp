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

		auto &em = c.enterMap;
		if (em.name != "") {
			c.map = asset.map.at(em.name);
			c.player.x = em.x;
			c.player.y = em.y + c.player.h - 0.5f; // 修正体型变化导致的高度差
			em = {};
			return true;
		}

		player.parse();

		c.player.pose = player.pose;

		return true;
	};

	void loopEvent();
};
