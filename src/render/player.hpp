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

		float size = 500.0f;

		const auto &player = d->scene.player;
		const auto &pose = player.pose;

		const auto sprite = player.asset->sprite.at(pose.type);

		auto tex = sprite->list[pose.step];

		SDL_FRect dst = {200.0f, 200.0f, size, size};

		if (d->scene.player.pose.facing == Pose::Facing::Left) {
			SDL_FlipMode flip = SDL_FLIP_HORIZONTAL;
			SDL_RenderTextureRotated(
				d->r, tex, nullptr, &dst, 0.0, nullptr, flip);
		} else {
			SDL_RenderTexture(d->r, tex, nullptr, &dst);
		}
	};
};
}; // namespace render
