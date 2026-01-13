#pragma once

#include "../game/tag.hpp"
#include "base.hpp"

namespace render {

struct Debug : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		if (!d->scene.map) {
			return;
		}

		SDL_SetRenderDrawColor(d->r, 200, 230, 255, 128);

		auto view = reg.view<physics::Rect, game::AssetMapCell>();
		for (auto [_, rect, _2] : view.each()) {
			auto dst = rect.getRect();
			renderFillRect(dst);
		}
	};
};
}; // namespace render
