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
	std::unordered_map<std::string, std::shared_ptr<SpriteBox>> sprite = {};
	std::unordered_map<pb::Tileset_Name, std::shared_ptr<Tileset>> tileset = {};
	std::unordered_map<std::string, std::shared_ptr<Map>> map = {};
	std::unordered_map<std::string, Monster> monster = {};

	void addSprite(std::shared_ptr<SpriteBox> b) {

		if (sprite.contains(b->name)) {
			spdlog::warn("duplicate sprite name: {}", b->name);
			return;
		}

		sprite[b->name] = b;
	}
};
}; // namespace asset
