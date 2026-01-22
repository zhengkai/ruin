#pragma once

#include "../game/reg.hpp"
#include "base.hpp"
#include <spdlog/spdlog.h>

namespace render {

struct Map : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto map = d->window.map;
		if (!map) {
			return;
		}

		map->filterTerrain(
			camera.focusRect(), [&](const asset::MapCell &t) -> bool {
				auto tile =
					d->asset.tileset.at(t.tileName).list.at(t.tileID - 1);
				// spdlog::info("cell {} {} {}",
				// 	t.tileID,
				// 	tile == nullptr,
				// 	d->asset.tileset.at(t.tileName).list.size());
				auto dst = t.getRect();
				renderTexture(tile, dst);
				return false;
			});

		auto v2 = reg.view<asset::MapGate>();
		if (!v2.empty()) {
			SDL_SetRenderDrawColor(d->r, 200, 128, 255, 156);
		}
		for (auto [_, mg] : v2.each()) {
			auto dst = mg.rect.getRect();
			renderFilledRect(dst);
		};
	};
};

}; // namespace render
