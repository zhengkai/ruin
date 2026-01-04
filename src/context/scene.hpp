#pragma once

#include "../asset/map.hpp"
#include "entity.hpp"
#include <vector>

namespace context {

struct Scene {
	Player player = {};
	std::vector<Enemy> enemy;
	std::shared_ptr<asset::Map> map;
};

}; // namespace context
