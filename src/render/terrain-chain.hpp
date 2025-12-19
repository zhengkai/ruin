#pragma once

#include "../terrain/island.hpp"
#include "../terrain/outline.hpp"
#include "../util/matrix.hpp"
#include "base.hpp"

namespace render {

struct TerrainChain : base {

	std::vector<std::vector<terrain::IslandPos>> outline;

	using base::base;
	void init() override {
		auto &map = d->asset.map;
		auto m = util::Matrix<uint8_t>(map.w, map.h, 0);
		for (const auto &c : map.cell) {
			int x = static_cast<int>(c.x);
			int y = static_cast<int>(c.y);
			m[x][y] = c.tileName > 0 ? 1 : 0;
		};
		auto island = terrain::Island(m);
		auto size = island.size();
		outline.reserve(size);
		for (auto &is : island) {
			outline.emplace_back(terrain::Outline(is));
		}
	};
	void render() override {
		SDL_SetRenderDrawColor(d->r, 0, 0, 0, 255);
		for (auto &o : outline) {
			std::vector<SDL_FPoint> pts;
			for (auto &q : o) {
				pts.push_back(SDL_FPoint{
					100.0f + static_cast<float>(q.x * 30),
					d->window.h - 100.0f - static_cast<float>(q.y * 30),
				});
			}
			SDL_RenderLines(d->r, pts.data(), static_cast<int>(pts.size()));
		}
	};
};

}; // namespace render
