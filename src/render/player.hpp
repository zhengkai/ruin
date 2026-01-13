#pragma once

// #include "../common/pose.hpp"
#include "../common/pose.hpp"
#include "../game/reg.hpp"
#include "base.hpp"
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

namespace render {

struct Player : base {

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

		// auto &e = reg.getPlayer();
		//
		// auto &rect = reg.get<physics::Rect>(e);
		//
		// SDL_SetRenderDrawColor(d->r, 200, 230, 255, 128);
		// auto r = rect.getRect();
		// renderFillRect(r);

		/*
		const auto &p = d->scene.player;
		const auto &po = p.pose;

		const auto sprite = p.asset->sprite.at(po.type);

		auto tex = sprite->list[po.step];

		SDL_FRect dst = {.x = p.x, .y = p.y + 0.375f, .w = 2.25f, .h
		= 2.25f};

		text(std::format("x={:.3f}, y={:.3f}", p.x, p.y), dst);

		if (po.facing == Pose::Facing::Left) {
			renderTextureFlipX(tex, dst);
		} else {
			renderTexture(tex, dst);
		}
 */
	};
};
}; // namespace render
