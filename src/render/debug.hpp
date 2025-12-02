#pragma once

#include "base.hpp"

namespace render {

struct Debug : base {

	using base::base;

	void init() override {};
	void render() override {

		SDL_SetRenderDrawColor(d->r, 200, 230, 255, 128);

		for (const auto &t : d->asset.map.cell) {
			SDL_FRect rect = t.getRect();
			renderFillRect(rect);
		}

		SDL_SetRenderDrawColor(d->r, 100, 130, 255, 128);

		SDL_FRect p = d->scene.player.getRect();
		renderFillRect(p);
	};
};
}; // namespace render
