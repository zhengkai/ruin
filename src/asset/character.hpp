#pragma once

#include "pb/pose.pb.h"
#include "sprite.hpp"
#include <unordered_map>

namespace asset {

struct Character {
	std::string name = "";
	std::unordered_map<pb::Pose::Type, std::shared_ptr<Sprite>> sprite = {};
};

}; // namespace asset
