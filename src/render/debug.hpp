#pragma once

#include "../tag.hpp"
#include "base.hpp"

namespace render {

struct Debug : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto map = d->window.map;
		if (!map) {
			return;
		}

		const auto &f = d->window.focus;
		const physics::Rect rect = {f.x, f.y, 23.0f, 23.0f};

		SDL_SetRenderDrawColor(d->r, 200, 230, 255, 128);

		map->filterTerrain(rect, [&](const asset::MapCell &t) -> bool {
			auto dst = t.getRect();
			renderFilledRect(dst);
			return false;
		});
	};
};
}; // namespace render
