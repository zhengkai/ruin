#pragma once

#include "../common/pose.hpp"
#include "../game/reg.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
// #include <spdlog/spdlog.h>

namespace render {

struct Sprite : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto view = reg.view<physics::Rect, Pose, asset::SpriteRef>();

		SDL_SetRenderDrawColor(d.r, 64, 128, 255, 192);
		for (auto [_, rect, _2, _3] : view.each()) {
			auto r = rect.getRect();
			renderFilledRect(r);
		}

		SDL_SetRenderDrawColor(d.r, 255, 50, 50, 255);
		for (auto [_, rect, pose, box] : view.each()) {

			auto &s = box.ptr;

			// auto tex = s.get(pose.type).texture(pose.step);
			auto tex = s.sprite.at(pose.type).list[pose.step];

			auto v = s.visual;
			SDL_FRect dst = {.x = rect.x, .y = rect.y, .w = v.w, .h = v.h};

			auto d2 = dst;
			renderRectOutline(d2);

			if (v.offsetY) {
				dst.y += v.offsetY;
			}

			if (pose.facing == Pose::Facing::Left) {
				renderTextureFlipX(tex, dst);
			} else {
				renderTexture(tex, dst);
			}
			// spdlog::info("pose {}.{} {}x{}", dst.x, dst.y, dst.w, dst.h);
		}
	};
};
}; // namespace render
