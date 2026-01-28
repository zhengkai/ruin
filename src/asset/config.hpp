#pragma once

#include "../name/sprite.hpp"
#include "map.hpp"

namespace asset {

struct Config {
	name::Sprite playerSprite = {pb::SpriteBox_Name::SpriteBox_Name_samurai};
	MapTarget zoneStart;
};

}; // namespace asset
