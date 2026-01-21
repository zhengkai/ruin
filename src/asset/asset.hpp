#pragma once

#include "../pb/asset.pb.h"
#include "config.hpp"
#include "map.hpp"
#include "monster.hpp"
#include "sprite.hpp"
#include "tileset.hpp"
#include <spdlog/spdlog.h>
#include <unordered_map>

namespace asset {

struct Asset {
	Config config = {};
	std::unordered_map<std::string, SpriteBox> sprite = {};
	std::unordered_map<pb::Tileset_Name, Tileset> tileset = {};
	std::unordered_map<std::string, Map> map = {};
	std::unordered_map<std::string, Monster> monster = {};
};
}; // namespace asset
