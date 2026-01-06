#pragma once

#include "../pb/asset.pb.h"
#include "../physics/pos.hpp"
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

struct MapGate {
	int id = 0;
	float x = 0.0;
	float y = 0.0;
	std::string targetMap = "";
	physics::Pos targetPos = physics::Pos{.x = 0.0f, .y = 0.0f};
};

struct Map {
	std::string name = "";
	int idx = -1;
	int w = 80;
	int h = 20;
	std::vector<MapCell> terrain;
	std::vector<MapGate> gate;
	std::vector<MapGate> exit;
};

}; // namespace asset
