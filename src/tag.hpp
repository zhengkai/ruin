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

struct Jump {};

struct PrevPos {
	float x;
	float y;
};

struct Speed {
	float v;
};
const Speed defaultWalkSpeed = {.v = 13.0f};

struct AssetMapCell {
	const asset::MapCell &def;
};

}; // namespace tag
