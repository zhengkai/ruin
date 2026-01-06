#pragma once

#include "base.hpp"

namespace render {

struct Map : base {

	using base::base;

	void init() override {};
	void render() override {
		for (const auto &t : d->scene.map->terrain) {
			auto tile = d->asset.tileset.at(t.tileName)->list.at(t.tileID - 1);
			auto dst = t.getRect();
			renderTexture(tile, dst);
		}
	};
};

}; // namespace render
