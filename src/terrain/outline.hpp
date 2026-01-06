#pragma once

#include "../asset/asset.hpp"
#include "../util/matrix.hpp"
#include "island-pos.hpp"
#include "island.hpp"
#include <algorithm>
#include <map>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vector>

namespace terrain {

static std::unordered_multimap<IslandPos, IslandPos> genBasicOutline(
	const std::vector<IslandPos> &pl) {

	std::map<Edge, int, bool (*)(const Edge &, const Edge &)> cnt(
		[](const Edge &e1, const Edge &e2) {
			return e1.a < e2.a || (!(e2.a < e1.a) && e1.b < e2.b);
		});

	// 描出所有边（每个方块四个边）
	for (auto &c : pl) {
		cnt[Edge({c.x, c.y}, {c.x, c.y + 1})]++;		 // left
		cnt[Edge({c.x + 1, c.y}, {c.x + 1, c.y + 1})]++; // right
		cnt[Edge({c.x, c.y}, {c.x + 1, c.y})]++;		 // top
		cnt[Edge({c.x, c.y + 1}, {c.x + 1, c.y + 1})]++; // bottom
	}

	// 只保留出现一次的边（轮廓线）
	std::unordered_multimap<IslandPos, IslandPos> adj;
	for (auto &[e, n] : cnt) {
		if (n == 1) {
			adj.emplace(e.a, e.b);
			adj.emplace(e.b, e.a);
		}
	}

	return adj;
};

static std::vector<IslandPos> walkOutline(
	std::unordered_multimap<IslandPos, IslandPos> &adj) {

	std::vector<IslandPos> re;

	// 从某个点开始，遍历轮廓线
	while (!adj.empty()) {
		auto it = adj.begin();
		IslandPos start = it->first;
		IslandPos curr = start;
		std::vector<IslandPos> outline;
		// outline.push_back(start);

		do {
			auto range = adj.equal_range(curr);
			IslandPos next;
			if (range.first == range.second) {
				break; // no next
			} else {
				next = range.first->second;
			}

			outline.push_back(next);
			adj.erase(range.first); // remove used edge
			// also remove the reverse edge
			auto rev_range = adj.equal_range(next);
			for (auto rev_it = rev_range.first; rev_it != rev_range.second;
				++rev_it) {
				if (rev_it->second == curr) {
					adj.erase(rev_it);
					break;
				}
			}

			curr = next;
		} while (curr != start);

		if (outline.size() > re.size()) {
			re = outline;
		}
	}

	return re;
};

static std::size_t minXYIndex(const std::vector<IslandPos> &v) {
	auto it = std::min_element(
		v.begin(), v.end(), [](const IslandPos &a, const IslandPos &b) {
			if (a.x != b.x) {
				return a.x < b.x;
			}
			return a.y > b.y; // y 大的在前（左上角优先）
		});
	return static_cast<std::size_t>(it - v.begin());
};

// 以左上角点开始，防止随机截断导致合并路径出错
static void rotateOutline(std::vector<IslandPos> &v) {
	auto idx = minXYIndex(v);
	if (idx == 0) {
		return;
	}
	std::rotate(v.begin(), v.begin() + idx, v.end());
};

// 将一条直线上的所有点合并为两个端点
static std::vector<IslandPos> mergeCollinear(std::vector<IslandPos> &in) {

	if (in.size() <= 2) {
		return in;
	}

	in.push_back(in[0]); // 闭合

	std::vector<IslandPos> out;
	out.reserve(in.size());

	out.push_back(in[0]);
	out.push_back(in[1]);

	for (std::size_t i = 2; i < in.size(); ++i) {
		auto &A = out[out.size() - 2];
		auto &B = out[out.size() - 1];
		auto &C = in[i];

		auto a1 = A.x == B.x;
		auto b1 = B.x == C.x;

		// 同方向直线，吞掉 B
		if (a1 == b1) {
			out.back() = C;
		} else {
			out.push_back(C);
		}
	}

	out.pop_back(); // 解除闭合

	return out;
}

inline std::vector<IslandPos> Outline(const std::vector<IslandPos> &pl) {

	auto adj = genBasicOutline(pl);

	auto re = walkOutline(adj);

	rotateOutline(re);

	return mergeCollinear(re);
};

inline std::vector<std::vector<terrain::IslandPos>> MapOutline(
	const std::shared_ptr<asset::Map> map) {

	auto m = util::Matrix<uint8_t>(map->w, map->h, 0);
	for (const auto &c : map->terrain) {
		int x = static_cast<int>(c.pos.x);
		int y = static_cast<int>(c.pos.y);
		m[x][y] = c.tileName > 0 ? 1 : 0;
	};
	auto island = Island(m);
	auto size = island.size();

	std::vector<std::vector<terrain::IslandPos>> outline;
	outline.reserve(size);
	for (auto &is : island) {
		outline.emplace_back(terrain::Outline(is));
	}

	return outline;
};

}; // namespace terrain
