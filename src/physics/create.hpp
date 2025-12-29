#pragma once

#include "../asset/asset.hpp"
#include "../context/scene.hpp"
#include "physics.hpp"

namespace physics {

class Create {

private:
	context::Scene &scene;
	const asset::Asset &asset;
	std::unique_ptr<Physics> p;

public:
	Create(context::Scene &scene, const asset::Asset &asset)
		: scene(scene), asset(asset) {

		p = std::make_unique<Physics>();

		p->setSize(static_cast<float>(asset.map.w + 2),
			static_cast<float>(asset.map.h + 20));

		initPlayer();
		initTile();
	};
	~Create() {};
	std::unique_ptr<Physics> get() {
		return std::move(p);
	};

private:
	void initPlayer() {
		auto &sp = scene.player;
		float x = config::posResetX;
		float y = config::posResetY;
		float w = sp.w / 2.0f;
		float h = sp.h / 2.0f;
		sp.physicsSerial = p->addBody(x, y, w, h);
	};

	void initTile() {
		auto &cl = asset.map.cell;
		if (!cl.size()) {
			spdlog::warn("No map cells to init tiles.");
			return;
		}
		for (const auto &c : cl) {
			p->addTile(c.x, c.y);
		}
	};
};

}; // namespace physics
