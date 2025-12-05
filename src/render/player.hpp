#pragma once

#include "../common/pose.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace render {

struct Player : base {

	using base::base;

	void init() override {};
	void render() override {

		const auto &p = d->scene.player;
		const auto &po = p.pose;

		const auto sprite = p.asset->sprite.at(po.type);

		auto tex = sprite->list[po.step];

		SDL_FRect dst = {.x = p.x, .y = p.y + 0.375f, .w = 2.25f, .h = 2.25f};

		if (po.facing == Pose::Facing::Left) {
			renderTextureFlipX(tex, dst);
		} else {
			renderTexture(tex, dst);
		}
	};
};
}; // namespace render
