#pragma once

#include "../asset/map.hpp"

namespace game {

struct TagPlayer {};

struct TagMonster {};

struct AssetMapCell {
	const asset::MapCell &def;
};

}; // namespace game
