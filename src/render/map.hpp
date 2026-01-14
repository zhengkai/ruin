#pragma once

#include "../game/reg.hpp"
#include "../tag.hpp"
#include "base.hpp"

namespace render {

struct Map : base {

	using base::base;

	void init() override {};
	void render(const game::Reg &reg) override {

		auto view = reg.view<physics::Rect, tag::AssetMapCell>();
		for (auto [_, rect, mc] : view.each()) {
			auto t = mc.def;
			auto tile = d->asset.tileset.at(t.tileName)->list.at(t.tileID - 1);
			auto dst = rect.getRect();
			renderTexture(tile, dst);
		}
	};
};

}; // namespace render
