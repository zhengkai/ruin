#pragma once

#include "../pb/asset.pb.h"
#include "../physics/rect.hpp"
#include "monster.hpp"
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

struct MapMonster : physics::Rect {
	Monster &def;

	MapMonster(float x, float y, float w, float h, Monster &m)
		: Rect{Pos{x, y}, w, h}, def(m) {
	}
};

struct Map {
	std::string name = "";
	std::size_t w = 80;
	std::size_t h = 20;
	std::vector<MapCell> staticTerrain;
	std::vector<MapCell> terrain;
	std::vector<MapGate> gate;
	std::vector<MapGate> exit;
	std::vector<MapMonster> monster;

	template <typename F> void filterTerrain(const physics::Rect &re, F &&f) {

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
				if (f(staticTerrain[y * w + x])) {
					return;
				}
			}
		}
	}
};

}; // namespace asset
