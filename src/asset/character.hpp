#pragma once

#include "sprite.hpp"
#include <unordered_map>

namespace asset {

struct Character {
	std::string name = "";
	std::unordered_map<std::string, Sprite> sprite = {};
};

}; // namespace asset
