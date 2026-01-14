#pragma once

#include "asset/map.hpp"

namespace tag {

struct Player {};

struct Monster {};

struct TouchX {
	bool l = false;
	bool r = false;
};

struct TouchDown {};

struct PrevPos {
	float x;
	float y;
};

struct AssetMapCell {
	const asset::MapCell &def;
};

}; // namespace tag
