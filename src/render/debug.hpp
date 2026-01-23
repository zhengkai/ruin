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

		auto rect = camera.focusRect();

		map->filterTerrain(rect, [&](const asset::MapCell &t) {
			auto dst = t.getRect();
			renderFilledRect(dst);
		});
	};
};
}; // namespace render
