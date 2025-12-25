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

		initPlayer();
	};
	~Create() {};
	std::unique_ptr<Physics> get() {
		return std::move(p);
	};

private:
	void initPlayer() {
		float x = 10.0f;
		float y = 13.0f;
		float w = scene.player.w / 2.0f;
		float h = scene.player.h / 2.0f;
		p->addBody(x, y, w, h);
	};

	void initTile() {
		for (const auto &b : asset.map.cell) {
			p->addTile(b.x, b.y);
		}
	};
};

}; // namespace physics
