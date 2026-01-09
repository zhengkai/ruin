#pragma once

#include "../pb/asset.pb.h"
#include "../physics/rect.hpp"
#include "monster.hpp"
#include <SDL3/SDL_rect.h>

namespace asset {

struct MapCell {
	int id = 0;
	pb::Tileset_Name tileName = pb::Tileset_Name_unknown;
	int tileID = 1;
	physics::Pos pos = {};

	SDL_FRect getRect() const {
		return SDL_FRect{.x = pos.x, .y = pos.y, .w = 1.0f, .h = 1.0f};
	}
};

struct MapTarget : physics::Pos {
	std::string name = "";
};

struct MapGate : MapTarget {
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
	int idx = -1;
	int w = 80;
	int h = 20;
	std::vector<MapCell> terrain;
	std::vector<MapGate> gate;
	std::vector<MapGate> exit;
	std::vector<MapMonster> monster;
};

}; // namespace asset
