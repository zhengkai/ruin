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

		auto fr = d.window.camera.focusRate();

		for (auto &bg : z->background) {
			if (bg.loopCnt) {
				renderLoop(bg, z->map, fr, bg.loopCnt.value());
				continue;
			}
		};
	};

	void renderLoop(const asset::ZoneBackground &bg,
		const asset::Map &m,
		const FocusRate &fr,
		float cnt) {

		const float mh = static_cast<float>(m.h);
		const float mw = static_cast<float>(m.w);

		float ww = mh / bg.h * bg.w;

		float w = mw - ww * cnt;

		for (float i = 0.0f; i < cnt; i += 1.0f) {

			auto dst = SDL_FRect{
				.x = (w * fr.x) + (ww * i) + (ww / 2.0f) - 0.5f,
				.y = mh / 2.0f - 0.5f,
				.w = ww,
				.h = mh,
			};

			renderTexture(bg.texture, dst);
		}
	}
};

}; // namespace render
