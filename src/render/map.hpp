#pragma once
#include "base.hpp"

namespace render {

struct Map : base {

	using base::base;

	void init() override {};
	void render() override {
		for (const auto &t : d->asset.map.cell) {
			auto tile = d->asset.tileset.at(t.tileName)->list.at(t.tileID - 1);
			SDL_FRect dst = {t.x, t.y, 1.0f, 1.0f};
			d->window.calcCameraOffset(dst);
			SDL_RenderTexture(d->r, tile, nullptr, &dst);
		}
	};
};

}; // namespace render
