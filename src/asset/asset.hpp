#pragma once

#include "../pb/asset.pb.h"
#include "character.hpp"
#include "tileset.hpp"
#include <unordered_map>

namespace asset {

struct Asset {
	std::unordered_map<std::string, std::shared_ptr<Character>> character = {};
	std::unordered_map<pb::Tileset_Name, std::shared_ptr<Tileset>> tileset = {};
};
}; // namespace asset
