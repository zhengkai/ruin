#pragma once

#include "../util/matrix.hpp"
#include "island-pos.hpp"
#include <cstddef>
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace terrain {

struct IslandChecker {
	util::Matrix<uint8_t> &src;
	std::unordered_map<IslandPos, uint8_t, IslandPosHash> visited;

	IslandChecker(util::Matrix<uint8_t> &src_) : src(src_) {};

	void scan(std::size_t x, std::size_t y, std::vector<IslandPos> &li) {
		scan(IslandPos{x, y}, li);
	};

	void scan(IslandPos pos, std::vector<IslandPos> &li) {
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
			scan(pos.x - 1, pos.y, li);
		}
		if (pos.x + 1 < src.w()) {
			scan(pos.x + 1, pos.y, li);
		}
		if (pos.y > 0) {
			scan(pos.x, pos.y - 1, li);
		}
		if (pos.y + 1 < src.h()) {
			scan(pos.x, pos.y + 1, li);
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
			auto &item = li.emplace_back();
			auto checker = IslandChecker{m};
			checker.scan(x, y, item);
		}
	}

	return li;
};
}; // namespace terrain
