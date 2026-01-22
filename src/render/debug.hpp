#pragma once

#include "base.hpp"

namespace render {

struct Debug : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto map = d.window.map;
		if (!map) {
			return;
		}

		SDL_SetRenderDrawColor(d.r, 200, 230, 255, 128);

		map->filterTerrain(
			camera.focusRect(), [&](const asset::MapCell &t) -> bool {
				auto dst = t.getRect();
				renderFilledRect(dst);
				return false;
			});
	};
};
}; // namespace render
