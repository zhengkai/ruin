#pragma once

#include "../game/reg.hpp"
#include "base.hpp"
#include <spdlog/spdlog.h>

namespace render {

struct Background : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &_) override {

		auto z = d.window.zone;
		if (!z) {
			return;
		}

		auto focus = d.window.camera.focusRate();

		for (auto &bg : z->background) {

			float h = static_cast<float>(z->map.h);

			float ww = h / bg.h * bg.w;

			float w = static_cast<float>(z->map.w) - ww;

			auto dst = SDL_FRect{
				.x = w * focus.x + ww / 2.0f - 0.5f,
				.y = h / 2.0f - 0.5f,
				.w = ww,
				.h = h,
			};

			// spdlog::info("bg dst: {} {} {} {}", dst.x, dst.y, dst.w, dst.h);

			renderTexture(bg.texture, dst);
		};
	};
};

}; // namespace render
