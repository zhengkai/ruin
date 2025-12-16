#pragma once

#include "../util/matrix.hpp"
#include "island-pos.hpp"
#include <cstddef>
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace terrain {

struct IslandChecker {
	util::Matrix<uint8_t> &src;
	std::vector<IslandPos> li;
	std::unordered_map<IslandPos, uint8_t, IslandPosHash> visited;

	IslandChecker(util::Matrix<uint8_t> &src, std::size_t x, std::size_t y)
		: src(src) {
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
		li.push_back(pos);

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

inline std::vector<std::vector<IslandPos>> Island(util::Matrix<uint8_t> &m) {
	std::vector<std::vector<IslandPos>> li;
	const std::size_t w = m.w();
	const std::size_t h = m.h();

	for (std::size_t x = 0; x < w; ++x) {
		for (std::size_t y = 0; y < h; ++y) {
			if (!m(x, y)) {
				continue;
			}
			IslandChecker checker(m, x, y);
			li.emplace_back(std::move(checker.li));
		}
	}

	return li;
};
}; // namespace terrain
