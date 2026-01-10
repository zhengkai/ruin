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
		  player(global, control, c, asset) {

		c.enterMap = {
			{.x = config::posResetX, .y = config::posResetY},
			"ruin-2",
		};
	};
	~Scene() {};

	void parse() {

		if (control.btnRB) {
			c.map = asset.map.at("ruin-2");
			return;
		}

		if (control.btnLB) {
			c.map = asset.map.at("ruin-3");
			return;
		}

		if (parseEnterMap()) {
			return;
		}

		player.parse();

		return;
	};

	bool parseEnterMap() {

		auto &em = c.enterMap;
		if (em.name == "") {
			return false;
		}

		c.map = asset.map.at(em.name);
		// window.setBoundary(c.map->w, c.map->h);
		c.player.x = em.x;
		c.player.y = em.y + c.player.h - 0.5f; // 修正体型变化导致的高度差
		em = {};

		c.monster.clear();
		for (auto &m : c.map->monster) {
			context::Monster cm;
			cm.x = m.x;
			cm.y = m.y;
			cm.w = m.w;
			cm.h = m.h;
			cm.asset = m.def.sprite;
			cm.pose.type = pb::Pose_Type::Pose_Type_walk;
			c.monster.emplace_back(cm);
		}

		return true;
	}

	void loopEvent();
};
