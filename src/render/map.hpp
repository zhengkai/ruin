#pragma once

#include "../game/tag.hpp"
#include "../game/world.hpp"
#include "base.hpp"

namespace render {

struct Map : base {

	using base::base;

	void init() override {};
	void render(const game::World &world) override {

		auto view = world.view<physics::Rect, game::AssetMapCell>();
		for (auto [_, rect, mc] : view.each()) {
			auto t = mc.def;
			auto tile = d->asset.tileset.at(t.tileName)->list.at(t.tileID - 1);
			auto dst = rect.getRect();
			renderTexture(tile, dst);
		}

		/*
		if (!m) {
			return;
		}

		for (const auto &t : m->terrain) {
			auto tile = d->asset.tileset.at(t.tileName)->list.at(t.tileID - 1);
			auto dst = t.getRect();
			renderTexture(tile, dst);
		}

		SDL_SetRenderDrawColor(d->r, 255, 130, 205, 128);
		for (const auto &g : m->gate) {
			SDL_FRect p = g.rect.getRect();
			renderFillRect(p);
		}
	 */
	};
};

}; // namespace render
