#pragma once

#include "../game/reg.hpp"
#include "base.hpp"
#include <spdlog/spdlog.h>

namespace render {

struct Map : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto zone = d.window.zone;
		if (!zone) {
			return;
		}

		auto rect = camera.focusRect();
		// rect.w -= 2.0f;
		// rect.h -= 2.0f;

		zone->map.filterTerrain(rect, [&](const asset::MapCell &t) {
			auto tile = d.asset.tileset.at(t.tileName).list.at(t.tileID - 1);
			auto dst = t.getRect();
			renderTexture(tile, dst);
		});

		auto v2 = reg.view<asset::MapGate>();
		if (!v2.empty()) {
			SDL_SetRenderDrawColor(d.r, 200, 128, 255, 156);
		}
		for (auto [_, mg] : v2.each()) {
			auto dst = mg.rect.getRect();
			renderFilledRect(dst);
		};
	};
};

}; // namespace render
