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

		const auto &player = d->scene.player;
		const auto &pose = player.pose;

		const auto sprite = player.asset->sprite.at(pose.type);

		auto tex = sprite->list[pose.step];

		SDL_FRect dst = {
			.x = player.x, .y = player.y + 0.5f, .w = 3.0f, .h = 3.0f};

		if (player.pose.facing == Pose::Facing::Left) {
			renderTextureFlipX(tex, dst);
		} else {
			renderTexture(tex, dst);
		}
	};
};
}; // namespace render
