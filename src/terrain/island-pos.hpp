#pragma once

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace terrain {

struct IslandPos {
	std::size_t x;
	std::size_t y;

	auto operator<=>(const IslandPos &) const = default;

	IslandPos dir(const IslandPos &b) {
		return {b.x - x, b.y - y};
	}

	bool sameDir(const IslandPos &b) {
		return x == b.x && y == b.y;
	}
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

namespace fmt {

template <> struct formatter<terrain::IslandPos> {
	constexpr auto parse(format_parse_context &ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const terrain::IslandPos &p, FormatContext &ctx) const {
		return format_to(ctx.out(), "({}, {})", p.x, p.y);
	}
};
}; // namespace fmt
