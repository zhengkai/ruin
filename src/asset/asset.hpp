#pragma once

#include "character.hpp"
#include <unordered_map>

namespace asset {

struct Asset {
	std::unordered_map<std::string, std::shared_ptr<Character>> character = {};
};
}; // namespace asset
