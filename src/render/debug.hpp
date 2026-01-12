#pragma once

#include "base.hpp"

namespace render {

struct Debug : base {

	using base::base;

	void init() override {};
	void render(const game::World &world) override {

		if (!d->scene.map) {
			return;
		}

		SDL_SetRenderDrawColor(d->r, 200, 230, 255, 128);

		auto view = world.view<physics::Rect>();
		for (auto [_, rect] : view.each()) {
			auto dst = rect.getRect();
			renderFillRect(dst);
		}
	};
};
}; // namespace render
