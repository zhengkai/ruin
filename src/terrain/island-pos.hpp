#pragma once

namespace terrain {

struct IslandPos {
	std::size_t x;
	std::size_t y;

	bool operator==(const IslandPos &) const = default;
};

struct IslandPosHash {
	size_t operator()(const IslandPos &p) const noexcept {
		return p.x * 1315423911u + p.y;
	}
};
}; // namespace terrain

namespace std {

template <> struct hash<terrain::IslandPos> {
	std::size_t operator()(const terrain::IslandPos &p) const noexcept {
		std::size_t h1 = std::hash<std::size_t>{}(p.x);
		std::size_t h2 = std::hash<std::size_t>{}(p.y);
		return h1 ^ (h2 << 1);
	}
};
}; // namespace std
