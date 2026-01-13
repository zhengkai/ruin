#pragma once

// #include "../common/pose.hpp"
#include "../common/pose.hpp"
#include "../game/reg.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace render {

struct Sprite : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto view =
			reg.view<physics::Rect, Pose, std::shared_ptr<asset::SpriteBox>>();
		for (auto [_, rect, pose, box] : view.each()) {

			auto tex = box->sprite.at(pose.type)->list[pose.step];

			SDL_FRect dst = {
				.x = rect.x, .y = rect.y + 0.375f, .w = 2.25, .h = 2.25f};

			if (pose.facing == Pose::Facing::Left) {
				renderTextureFlipX(tex, dst);
			} else {
				renderTexture(tex, dst);
			}
		}
	};
};
}; // namespace render
