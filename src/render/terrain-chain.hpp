#pragma once

#include "../terrain/island.hpp"
#include "../util/matrix.hpp"
#include "base.hpp"

namespace render {

struct TerrainChain : base {

	std::vector<util::Matrix<uint8_t>> island;

	using base::base;
	void init() override {
		auto &map = d->asset.map;
		auto m = util::Matrix<uint8_t>(map.w, map.h, 0);
		for (const auto &c : map.cell) {
			int x = static_cast<int>(c.x);
			int y = static_cast<int>(c.y);
			m[x][y] = c.tileName > 0 ? 1 : 0;
		};
		island = terrain::Island(m);
	};
	void render() override {};
};

}; // namespace render
