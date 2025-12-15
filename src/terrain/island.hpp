#pragma once

#include "../util/matrix.hpp"
#include "island-pos.hpp"
#include <cstddef>
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace terrain {

struct IslandChecker {
	util::Matrix<uint8_t> &src;
	util::Matrix<uint8_t> dst;
	std::unordered_map<IslandPos, uint8_t, IslandPosHash> visited;

	IslandChecker(util::Matrix<uint8_t> &src, std::size_t x, std::size_t y)
		: src(src), dst(src.w(), src.h()) {
		scan(IslandPos{x, y});
	};

	void scan(IslandPos pos) {
		if (visited.contains(pos)) {
			return;
		}
		visited[pos] = true;

		if (!src(pos.x, pos.y)) {
			return;
		}
		dst[pos.x][pos.y] = 1;
		src[pos.x][pos.y] = 0;

		if (pos.x > 0) {
			scan(IslandPos{pos.x - 1, pos.y});
		}
		if (pos.x + 1 < src.w()) {
			scan(IslandPos{pos.x + 1, pos.y});
		}
		if (pos.y > 0) {
			scan(IslandPos{pos.x, pos.y - 1});
		}
		if (pos.y + 1 < src.h()) {
			scan(IslandPos{pos.x, pos.y + 1});
		}
	}
};

inline std::vector<util::Matrix<uint8_t>> Island(util::Matrix<uint8_t> &m) {
	std::vector<util::Matrix<uint8_t>> li;
	const std::size_t w = m.w();
	const std::size_t h = m.h();

	for (std::size_t x = 0; x < w; ++x) {
		for (std::size_t y = 0; y < h; ++y) {
			if (!m(x, y)) {
				continue;
			}
			IslandChecker checker(m, x, y);
			li.emplace_back(std::move(checker.dst));
		}
	}

	return li;
};
}; // namespace terrain
