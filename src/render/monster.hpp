#pragma once

#include "../common/pose.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace render {

struct Monster : base {

	using base::base;

	void init() override {};
	void render() override {

		for (auto &m : d->scene.monster) {

			const auto &po = m.pose;

			const auto sprite = m.asset->sprite.at(po.type);

			auto tex = sprite->list[po.step];

			SDL_FRect dst = m.getRect();

			text(std::format("x={:.3f}, y={:.3f}", m.x, m.y), dst);

			if (po.facing == Pose::Facing::Left) {
				renderTextureFlipX(tex, dst);
			} else {
				renderTexture(tex, dst);
			}
		}
	};
};
}; // namespace render
