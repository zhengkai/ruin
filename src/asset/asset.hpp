#pragma once

#include "../name/mob.hpp"
#include "../name/sprite.hpp"
#include "../name/zone.hpp"
#include "../pb/asset.pb.h"
#include "../pb/map.pb.h"
#include "config.hpp"
#include "map.hpp"
#include "mob.hpp"
#include "sprite.hpp"
#include "tileset.hpp"
#include "zone.hpp"
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace asset {

struct Asset {
	Config config = {};
	std::unordered_map<name::Sprite, SpriteBox> sprite = {};
	std::unordered_map<pb::Tileset_Name, Tileset> tileset = {};
	std::unordered_map<pb::Map_Name, Map> map = {};
	std::unordered_map<name::Zone, Zone> zone = {};
	std::unordered_map<name::MobType, Mob> mob = {};

	const SpriteBox &getSprite(const name::Sprite &name) const {
		return sprite.at(name);
	};
};
}; // namespace asset
