#pragma once

#include "base.hpp"

namespace render {

struct Debug : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto zone = d.window.zone;
		if (!zone) {
			return;
		}

		SDL_SetRenderDrawColor(d.r, 200, 230, 255, 128);

		auto rect = camera.focusRect();

		zone->map.filterTerrain(rect, [&](const asset::MapCell &t) {
			auto dst = t.getRect();
			renderFilledRect(dst);
		});
	};
};
}; // namespace render
