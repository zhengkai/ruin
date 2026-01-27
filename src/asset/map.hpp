#pragma once

#include "../pb/asset.pb.h"
#include "../physics/rect.hpp"
#include "../util/format.hpp"
#include "mob.hpp"
#include <SDL3/SDL_rect.h>
#include <algorithm>

static const std::size_t sizeTZero = 0;

namespace asset {

struct MapCell {
	pb::Tileset_Name tileName = pb::Tileset_Name_unknown;
	int tileID = 1;
	physics::Pos pos = {};

	physics::Rect phyRect() const {
		return physics::Rect{
			pos,
			0.5f,
		};
	}

	SDL_FRect getRect() const {
		return SDL_FRect{.x = pos.x, .y = pos.y, .w = 1.0f, .h = 1.0f};
	}
};

struct MapTarget : physics::Pos {
	std::string name = "";
};

struct MapGate {
	int id = 0;
	physics::Rect rect = {};
	MapTarget target = {};
};

struct MapMob : physics::Rect {
	const Mob &def;

	MapMob(float x, float y, float w, float h, Mob &m)
		: Rect{Pos{x, y}, w, h}, def(m) {
	}
};

struct Map {
	std::string name = "";
	std::size_t w = 0;
	std::size_t h = 0;
	std::vector<MapCell> terrain;
	std::vector<MapGate> gate;
	std::vector<MapGate> exit;
	std::vector<MapMob> mob;

	float checkMove(const physics::Rect &r, float move) const {

		float tx = r.x + move;
		if (move < 0.0f) {
			tx -= r.w;
		} else {
			tx += r.w;
		}

		auto x = static_cast<std::size_t>(std::round(tx));
		if (x < 0) {
			x = 0;
		} else if (x >= w) {
			x = w - 1;
		}

		auto y = static_cast<std::size_t>(std::round(r.y - r.h - 0.5f));
		if (y < 0) {
			y = 0;
		} else if (y >= h) {
			y = h - 1;
		}

		bool isExists = terrain[y * w + x].tileName != pb::Tileset_Name_unknown;
		if (isExists) {
			return move;
		}

		if (move < 0.0f) {
			return std::ceil(static_cast<float>(x)) + 0.5f - (r.x - r.w);
		} else if (move > 0.0f) {
			return std::floor(static_cast<float>(x)) - 0.5f - (r.x + r.w);
		}

		return 0.0f;
	};

	template <typename F>
		requires(std::same_as<std::invoke_result_t<F, const MapCell &>, bool> ||
			std::same_as<std::invoke_result_t<F, const MapCell &>, void>)
	void filterTerrain(const physics::Rect &re, F &&f) const {

		float fl = std::floor(re.x - re.w);
		float fd = std::floor(re.y - re.h);

		std::size_t l = static_cast<std::size_t>(fl < 1.0f ? 1.0f : fl) - 1;
		std::size_t r = std::min(
			w - 1, static_cast<std::size_t>(std::ceil(re.x + re.w)) + 1);
		std::size_t u = std::min(
			h - 1, static_cast<std::size_t>(std::ceil(re.y + re.h)) + 1);
		std::size_t d = static_cast<std::size_t>(fd < 1.0f ? 1.0f : fd) - 1;

		for (std::size_t y = d; y <= u; ++y) {
			for (std::size_t x = l; x <= r; ++x) {
				auto &t = terrain[y * w + x];
				if (t.tileName == pb::Tileset_Name_unknown) {
					continue;
				}
				if constexpr (std::is_same_v<
								  std::invoke_result_t<F, const MapCell &>,
								  bool>) {
					if (f(t)) {
						return;
					}
				} else {
					f(t);
				}
			}
		}
	}
};

}; // namespace asset
