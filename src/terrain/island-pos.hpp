#pragma once

namespace terrain {

struct IslandPos {
	std::size_t x;
	std::size_t y;

	auto operator<=>(const IslandPos &) const = default;
};

struct IslandPosHash {
	size_t operator()(const IslandPos &p) const noexcept {
		return p.x * 1315423911u + p.y;
	}
};

struct Edge {
	IslandPos a, b;

	Edge(IslandPos a_, IslandPos b_) : a(a_), b(b_) {
		if (b < a) {
			std::swap(a, b);
		}
	}
};
}; // namespace terrain

namespace std {

template <> struct hash<terrain::IslandPos> {
	size_t operator()(const terrain::IslandPos &p) const noexcept {
		return p.x * 1315423911u + p.y;
	}
};
}; // namespace std
