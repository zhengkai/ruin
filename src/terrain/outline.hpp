#pragma once

#include "island-pos.hpp"
#include <map>
#include <spdlog/spdlog.h>
#include <unordered_map>

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

static std::vector<std::vector<IslandPos>> walkOutline(
	std::unordered_multimap<IslandPos, IslandPos> &adj) {

	std::vector<std::vector<IslandPos>> re;

	// 从某个点开始，遍历轮廓线
	while (!adj.empty()) {
		auto it = adj.begin();
		IslandPos start = it->first;
		IslandPos curr = start;
		std::vector<IslandPos> outline;
		outline.push_back(start);

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

		re.push_back(outline);
	}

	return re;
};

static std::vector<IslandPos> mergeCollinear(const std::vector<IslandPos> &in) {
	if (in.size() <= 2)
		return in;

	std::vector<IslandPos> out;
	out.reserve(in.size());

	out.push_back(in[0]);
	out.push_back(in[1]);

	for (std::size_t i = 2; i < in.size(); ++i) {
		auto &A = out[out.size() - 2];
		auto &B = out[out.size() - 1];
		auto &C = in[i];

		auto dx1 = B.x - A.x;
		auto dy1 = B.y - A.y;
		auto dx2 = C.x - B.x;
		auto dy2 = C.y - B.y;

		// 同方向直线，吞掉 B
		if (dx1 == dx2 && dy1 == dy2) {
			out.back() = C;
		} else {
			out.push_back(C);
		}
	}

	return out;
}

std::vector<IslandPos> Outline(const std::vector<IslandPos> &pl) {

	auto adj = genBasicOutline(pl);

	auto re = walkOutline(adj);

	if (re.size() == 1) {
		return mergeCollinear(re[0]);
	}

	std::size_t maxLen = 0;
	std::size_t maxIdx = 0;
	for (std::size_t i = 0; i < re.size(); ++i) {
		if (re[i].size() > maxLen) {
			maxLen = re[i].size();
			maxIdx = i;
		}
	}
	return mergeCollinear(re[maxIdx]);
};
}; // namespace terrain
