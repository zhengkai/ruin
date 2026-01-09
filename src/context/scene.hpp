#pragma once

#include "../asset/map.hpp"
#include "entity.hpp"
#include <vector>

namespace context {

struct Scene {
	Player player = {};
	std::vector<Character> monster;
	std::shared_ptr<asset::Map> map;
	asset::MapTarget enterMap = {};
};

}; // namespace context
