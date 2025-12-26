#pragma once

#include "../pb/asset.pb.h"
#include <SDL3/SDL_rect.h>

namespace asset {

struct MapCell {
	int id = 0;
	pb::Tileset_Name tileName = pb::Tileset_Name_unknown;
	int tileID = 1;
	float x = 0.0;
	float y = 0.0;

	SDL_FRect getRect() const {
		return SDL_FRect{.x = x, .y = y, .w = 1.0f, .h = 1.0f};
	}
};

struct Map {
	int w = 80;
	int h = 20;
	std::vector<MapCell> cell;
};

}; // namespace asset
