#pragma once

#include "../pb/asset.pb.h"

namespace asset {

struct MapCell {
	int id = 0;
	pb::Tileset_Name tileName = pb::Tileset_Name_unknown;
	int tileID = 1;
	float x = 0.0;
	float y = 0.0;
};

struct Map {
	int w = 80;
	int h = 20;
	std::vector<MapCell> cell;
};

}; // namespace asset
